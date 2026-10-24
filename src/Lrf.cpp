#include "Lrf.h"
#include "LrfVersion.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>



namespace
{
// Encoded data stores int and float fields as raw 4 bytes values.
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(float) == 4, "float must be 4 bytes");
static_assert(std::numeric_limits<float>::is_iec559,
              "float must be IEEE 754 binary32");

/// Size of an int or float field in the encoded data, bytes.
constexpr int VALUE_SIZE{4};
/// Size of a bool field in the encoded data, bytes.
constexpr int FLAG_SIZE{1};
/// Number of LrfParams fields which are encoded (the same as the number of
/// fields of LrfParamsMask).
constexpr int FIELDS_NUMBER{18};
/// Number of int and float fields which are encoded.
constexpr int VALUES_NUMBER{16};
/// Number of bool fields which are encoded.
constexpr int FLAGS_NUMBER{2};
static_assert(VALUES_NUMBER + FLAGS_NUMBER == FIELDS_NUMBER,
              "wrong number of fields");

/// Data type of the encoded params. Commands use other data types.
constexpr uint8_t PARAMS_DATA_TYPE{0x02};
/// Position of the data type in the encoded data (the next two bytes are the
/// major and the minor version of the library).
constexpr int DATA_TYPE_POS{0};
/// Position of the major version in the encoded data.
constexpr int MAJOR_VERSION_POS{1};
/// Position of the minor version in the encoded data.
constexpr int MINOR_VERSION_POS{2};
/// Position of the mask of the encoded fields in the encoded params.
constexpr int PARAMS_MASK_POS{3};
/// Size of the mask of the encoded fields, bytes. One bit per field, the first
/// field is the most significant bit of the first byte.
constexpr int PARAMS_MASK_SIZE{3};
static_assert(FIELDS_NUMBER <= PARAMS_MASK_SIZE * 8,
              "mask is too small for the fields");
/// Size of the header of the encoded params: data type, version and mask.
constexpr int PARAMS_HEADER_SIZE{PARAMS_MASK_POS + PARAMS_MASK_SIZE};
/// Size of the encoded params with all fields selected. A buffer of this size
/// is enough for any mask.
constexpr int PARAMS_MAX_SIZE{PARAMS_HEADER_SIZE + VALUES_NUMBER * VALUE_SIZE +
                              FLAGS_NUMBER * FLAG_SIZE};
static_assert(PARAMS_MAX_SIZE == 72, "wrong size of encoded params");

/// Data type of the encoded command.
constexpr uint8_t COMMAND_DATA_TYPE{0x00};
/// Data type of the encoded set param command.
constexpr uint8_t SET_PARAM_DATA_TYPE{0x01};
/// Position of the command ID or the param ID in the encoded command.
constexpr int COMMAND_ID_POS{3};
/// Position of the param value in the encoded set param command.
constexpr int PARAM_VALUE_POS{COMMAND_ID_POS + VALUE_SIZE};
/// Size of the encoded command: data type, version and command ID.
constexpr int COMMAND_SIZE{COMMAND_ID_POS + VALUE_SIZE};
/// Size of the encoded set param command: data type, version, param ID and
/// param value.
constexpr int SET_PARAM_SIZE{PARAM_VALUE_POS + VALUE_SIZE};

/// Range of the command IDs: the IDs are enumerated from the first to the last
/// value of LrfCommand. An ID read from the data outside of the range is an
/// error, it is never passed on to the implementation.
constexpr int MIN_COMMAND_ID{static_cast<int>(rapidpixel::LrfCommand::ARM)};
constexpr int MAX_COMMAND_ID{
    static_cast<int>(rapidpixel::LrfCommand::MEASURE_DISTANCE_ONCE)};
/// Range of the param IDs: the IDs are enumerated from the first to the last
/// value of LrfParam.
constexpr int MIN_PARAM_ID{static_cast<int>(rapidpixel::LrfParam::DISTANCE)};
constexpr int MAX_PARAM_ID{static_cast<int>(rapidpixel::LrfParam::CUSTOM_3)};



/// Location of an LrfParams field in the encoded data.
struct FieldRef
{
    /// Pointer to the field of an LrfParams object.
    void* value{nullptr};
    /// Size of the field in the encoded data: VALUE_SIZE or FLAG_SIZE.
    int size{0};
};



/// Get the table of LrfParams fields in the order they are stored in the
/// encoded data (the same order as the bits of the mask).
std::array<FieldRef, FIELDS_NUMBER> makeFieldTable(rapidpixel::LrfParams& p)
{
    return {{
        {&p.distance, VALUE_SIZE},
        {&p.timeFromLastMeasurementUs, VALUE_SIZE},
        {&p.lowPowerMode, VALUE_SIZE},
        {&p.pointerMode, VALUE_SIZE},
        {&p.pointerModeTimeoutSec, VALUE_SIZE},
        {&p.armMode, VALUE_SIZE},
        {&p.operatingMode, VALUE_SIZE},
        {&p.continuousMeasuringMode, VALUE_SIZE},
        {&p.continuousModeTimeoutSec, VALUE_SIZE},
        {&p.logMode, VALUE_SIZE},
        {&p.isOpen, FLAG_SIZE},
        {&p.isConnected, FLAG_SIZE},
        {&p.minGateDistance, VALUE_SIZE},
        {&p.maxGateDistance, VALUE_SIZE},
        {&p.temperatureDeg, VALUE_SIZE},
        {&p.custom1, VALUE_SIZE},
        {&p.custom2, VALUE_SIZE},
        {&p.custom3, VALUE_SIZE}
    }};
}



/// Get the flags of the mask in the same order as makeFieldTable(...).
std::array<bool, FIELDS_NUMBER> makeMaskTable(
    const rapidpixel::LrfParamsMask& m)
{
    return {{
        m.distance,
        m.timeFromLastMeasurementUs,
        m.lowPowerMode,
        m.pointerMode,
        m.pointerModeTimeoutSec,
        m.armMode,
        m.operatingMode,
        m.continuousMeasuringMode,
        m.continuousModeTimeoutSec,
        m.logMode,
        m.isOpen,
        m.isConnected,
        m.minGateDistance,
        m.maxGateDistance,
        m.temperatureDeg,
        m.custom1,
        m.custom2,
        m.custom3
    }};
}



/// Get the index of the byte of the mask which contains the bit of the field.
constexpr int maskByteIndex(int field)
{
    return field / 8;
}



/// Get the bit of the field inside the byte of the mask.
constexpr uint8_t maskBit(int field)
{
    return static_cast<uint8_t>(128U >> (field % 8));
}
}



bool rapidpixel::LrfParams::encode(
    uint8_t* data, int bufferSize, int& size, LrfParamsMask* mask)
{
    // Check the pointer and the buffer size: the buffer must be able to hold
    // the data encoded with all fields, so nothing is written outside of it.
    if (data == nullptr || bufferSize < PARAMS_MAX_SIZE)
    {
        size = 0;
        return false;
    }

    // Encode data type and version.
    data[DATA_TYPE_POS] = PARAMS_DATA_TYPE;
    data[MAJOR_VERSION_POS] = static_cast<uint8_t>(LRF_MAJOR_VERSION);
    data[MINOR_VERSION_POS] = static_cast<uint8_t>(LRF_MINOR_VERSION);

    // Prepare mask. All fields are encoded if the mask is not set.
    const LrfParamsMask defaultMask;
    const auto selected = makeMaskTable(mask != nullptr ? *mask : defaultMask);
    std::memset(&data[PARAMS_MASK_POS], 0, PARAMS_MASK_SIZE);
    for (int i = 0; i < FIELDS_NUMBER; ++i)
    {
        if (selected[static_cast<size_t>(i)])
        {
            const int index = PARAMS_MASK_POS + maskByteIndex(i);
            data[index] = static_cast<uint8_t>(data[index] | maskBit(i));
        }
    }

    // Encode the selected fields.
    const auto fields = makeFieldTable(*this);
    int pos = PARAMS_HEADER_SIZE;
    for (int i = 0; i < FIELDS_NUMBER; ++i)
    {
        if (!selected[static_cast<size_t>(i)])
        {
            continue;
        }
        const FieldRef& field = fields[static_cast<size_t>(i)];
        if (field.size == FLAG_SIZE)
        {
            data[pos] = *static_cast<const bool*>(field.value) ?
                        static_cast<uint8_t>(0x01) : static_cast<uint8_t>(0x00);
        }
        else
        {
            std::memcpy(&data[pos], field.value, VALUE_SIZE);
        }
        pos += field.size;
    }

    size = pos;

    return true;
}



bool rapidpixel::LrfParams::decode(uint8_t* data, int dataSize)
{
    // Check the pointer, the data size (header must be complete) and the
    // data type.
    if (data == nullptr || dataSize < PARAMS_HEADER_SIZE)
    {
        return false;
    }
    if (data[DATA_TYPE_POS] != PARAMS_DATA_TYPE)
    {
        return false;
    }

    // Check version.
    if (data[MAJOR_VERSION_POS] != LRF_MAJOR_VERSION ||
        data[MINOR_VERSION_POS] != LRF_MINOR_VERSION)
    {
        return false;
    }

    // Decode into a temporary object: this object is changed only if the
    // whole data is valid. Fields not selected by the mask are set to zero.
    LrfParams result;
    const auto fields = makeFieldTable(result);
    int pos = PARAMS_HEADER_SIZE;
    for (int i = 0; i < FIELDS_NUMBER; ++i)
    {
        const FieldRef& field = fields[static_cast<size_t>(i)];
        const bool encoded =
            (data[PARAMS_MASK_POS + maskByteIndex(i)] & maskBit(i)) != 0;
        if (!encoded)
        {
            std::memset(field.value, 0, static_cast<size_t>(field.size));
            continue;
        }

        // Check that the field is inside the data (pos <= dataSize here).
        if (dataSize - pos < field.size)
        {
            return false;
        }
        if (field.size == FLAG_SIZE)
        {
            *static_cast<bool*>(field.value) = data[pos] != 0x00;
        }
        else
        {
            std::memcpy(field.value, &data[pos], VALUE_SIZE);
        }
        pos += field.size;
    }

    *this = result;

    return true;
}



rapidpixel::Lrf::~Lrf() = default;



std::string rapidpixel::Lrf::getVersion()
{
    return LRF_VERSION;
}



void rapidpixel::Lrf::encodeSetParamCommand(
    uint8_t* data, int& size, LrfParam id, float value)
{
    // Reject null pointer, nothing is written.
    if (data == nullptr)
    {
        size = 0;
        return;
    }

    // Fill header.
    data[DATA_TYPE_POS] = SET_PARAM_DATA_TYPE;
    data[MAJOR_VERSION_POS] = static_cast<uint8_t>(LRF_MAJOR_VERSION);
    data[MINOR_VERSION_POS] = static_cast<uint8_t>(LRF_MINOR_VERSION);

    // Fill data.
    const int paramId = static_cast<int>(id);
    std::memcpy(&data[COMMAND_ID_POS], &paramId, VALUE_SIZE);
    std::memcpy(&data[PARAM_VALUE_POS], &value, VALUE_SIZE);
    size = SET_PARAM_SIZE;
}



void rapidpixel::Lrf::encodeCommand(uint8_t* data, int& size, LrfCommand id)
{
    // Reject null pointer, nothing is written.
    if (data == nullptr)
    {
        size = 0;
        return;
    }

    // Fill header.
    data[DATA_TYPE_POS] = COMMAND_DATA_TYPE;
    data[MAJOR_VERSION_POS] = static_cast<uint8_t>(LRF_MAJOR_VERSION);
    data[MINOR_VERSION_POS] = static_cast<uint8_t>(LRF_MINOR_VERSION);

    // Fill data.
    const int commandId = static_cast<int>(id);
    std::memcpy(&data[COMMAND_ID_POS], &commandId, VALUE_SIZE);
    size = COMMAND_SIZE;
}



int rapidpixel::Lrf::decodeCommand(
    uint8_t* data, int size, LrfParam& paramId,
    LrfCommand& commandId, float& value)
{
    // Check pointer and size (header and ID must be complete).
    if (data == nullptr || size < COMMAND_SIZE)
    {
        return -1;
    }

    // Check version.
    if (data[MAJOR_VERSION_POS] != LRF_MAJOR_VERSION ||
        data[MINOR_VERSION_POS] != LRF_MINOR_VERSION)
    {
        return -1;
    }

    // Check command type. Output arguments are changed only if the command is
    // valid. Commands have a fixed size: a longer message is an error.
    if (data[DATA_TYPE_POS] == COMMAND_DATA_TYPE)
    {
        if (size != COMMAND_SIZE)
        {
            return -1;
        }
        int id = 0;
        std::memcpy(&id, &data[COMMAND_ID_POS], VALUE_SIZE);
        if (id < MIN_COMMAND_ID || id > MAX_COMMAND_ID)
        {
            return -1;
        }
        commandId = static_cast<LrfCommand>(id);
        return 0;
    }
    if (data[DATA_TYPE_POS] == SET_PARAM_DATA_TYPE)
    {
        if (size != SET_PARAM_SIZE)
        {
            return -1;
        }
        int id = 0;
        std::memcpy(&id, &data[COMMAND_ID_POS], VALUE_SIZE);
        if (id < MIN_PARAM_ID || id > MAX_PARAM_ID)
        {
            return -1;
        }
        paramId = static_cast<LrfParam>(id);
        std::memcpy(&value, &data[PARAM_VALUE_POS], VALUE_SIZE);
        return 1;
    }

    return -1;
}
