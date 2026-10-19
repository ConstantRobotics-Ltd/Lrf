#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include "Lrf.h"



namespace
{
using cr::lrf::Lrf;
using cr::lrf::LrfCommand;
using cr::lrf::LrfParam;
using cr::lrf::LrfParams;
using cr::lrf::LrfParamsMask;

/// Size of the encoded params header: data type, version and mask, bytes.
constexpr int PARAMS_HEADER_SIZE{6};
/// Size of the encoded params with all fields selected, bytes.
constexpr int PARAMS_MAX_SIZE{72};
/// Size of the encoded command, bytes.
constexpr int COMMAND_SIZE{7};
/// Size of the encoded set param command, bytes.
constexpr int SET_PARAM_SIZE{11};
/// Number of the first and the last IDs of LrfParam enum.
constexpr int FIRST_PARAM_ID{1};
constexpr int LAST_PARAM_ID{18};
/// Number of the first and the last IDs of LrfCommand enum.
constexpr int FIRST_COMMAND_ID{1};
constexpr int LAST_COMMAND_ID{3};
/// Number of random masks in the mask test.
constexpr int RANDOM_MASKS_NUMBER{5000};
/// Number of damaged data samples in the damaged data test.
constexpr int DAMAGED_SAMPLES_NUMBER{60000};
/// Number of params in the JSON round trip test.
constexpr int JSON_PARAMS_NUMBER{200};
/// Name of the file written by the JSON test.
constexpr const char* JSON_FILE_NAME{"TestLrfParams.json"};

/// State of the test random generator. The tests use own deterministic
/// generator: results must be reproducible on all platforms.
uint32_t g_randomState{20240915U};

/// Set if a file created by a test could not be removed.
bool g_cleanupFailed{false};



/// Get next random number in [0, limit).
int randomInt(int limit)
{
    g_randomState = g_randomState * 1664525U + 1013904223U;
    return static_cast<int>((g_randomState >> 8) %
                            static_cast<uint32_t>(limit));
}



/// Get random 32 bit value.
uint32_t randomBits()
{
    const uint32_t high = static_cast<uint32_t>(randomInt(65536));
    const uint32_t low = static_cast<uint32_t>(randomInt(65536));
    return (high << 16) | low;
}



/// Get the bits of the float value.
uint32_t bitsOf(float value)
{
    uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}



/// Get the bits of the int value.
uint32_t bitsOf(int value)
{
    return static_cast<uint32_t>(value);
}



/// Get the bits of the bool value.
uint32_t bitsOf(bool value)
{
    return value ? 1U : 0U;
}



/// Get the float value with given bits.
float floatFromBits(uint32_t bits)
{
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}



/// Get random float value, NaN, infinity and negative zero included. NaN is
/// always quiet: a signaling NaN can be changed by the hardware when the value
/// is copied.
float randomFloat()
{
    switch (randomInt(8))
    {
    case 0:
        return floatFromBits(0x7FC00000U);
    case 1:
        return floatFromBits(0x7F800000U);
    case 2:
        return -0.0f;
    default:
        break;
    }
    uint32_t bits = randomBits();
    if ((bits & 0x7F800000U) == 0x7F800000U)
    {
        bits |= 0x00400000U;
    }
    return floatFromBits(bits);
}



/// Get random int value, boundary values included.
int randomValue()
{
    switch (randomInt(8))
    {
    case 0:
        return 0;
    case 1:
        return std::numeric_limits<int>::min();
    case 2:
        return std::numeric_limits<int>::max();
    default:
        return static_cast<int>(randomBits());
    }
}



/// Get random finite float value which has an exact short JSON representation.
float randomFiniteFloat()
{
    return static_cast<float>(randomInt(20001) - 10000) / 100.0f;
}



/// Prepare params with random values. JSON can not store NaN and infinity: if
/// finiteOnly is set the float fields are finite.
void prepareRandomParams(LrfParams& params, bool finiteOnly)
{
    const auto floatValue = [finiteOnly]()
    {
        return finiteOnly ? randomFiniteFloat() : randomFloat();
    };
    params.initString = std::to_string(randomInt(255)) +
                        std::to_string(randomInt(255));
    params.distance = floatValue();
    params.timeFromLastMeasurementUs = randomValue();
    params.lowPowerMode = randomValue();
    params.pointerMode = randomValue();
    params.pointerModeTimeoutSec = randomValue();
    params.armMode = randomValue();
    params.operatingMode = randomValue();
    params.continuousMeasuringMode = randomValue();
    params.continuousModeTimeoutSec = randomValue();
    params.logMode = randomValue();
    params.isOpen = randomInt(2) != 0;
    params.isConnected = randomInt(2) != 0;
    params.minGateDistance = floatValue();
    params.maxGateDistance = floatValue();
    params.temperatureDeg = floatValue();
    params.custom1 = floatValue();
    params.custom2 = floatValue();
    params.custom3 = floatValue();
}



/// Get the mask: bit N of the argument is the flag of the field N (the order
/// of the fields of LrfParamsMask).
LrfParamsMask maskFromBits(uint32_t bits)
{
    const auto bit = [bits](int n)
    {
        return ((bits >> n) & 1U) != 0;
    };
    LrfParamsMask mask;
    mask.distance = bit(0);
    mask.timeFromLastMeasurementUs = bit(1);
    mask.lowPowerMode = bit(2);
    mask.pointerMode = bit(3);
    mask.pointerModeTimeoutSec = bit(4);
    mask.armMode = bit(5);
    mask.operatingMode = bit(6);
    mask.continuousMeasuringMode = bit(7);
    mask.continuousModeTimeoutSec = bit(8);
    mask.logMode = bit(9);
    mask.isOpen = bit(10);
    mask.isConnected = bit(11);
    mask.minGateDistance = bit(12);
    mask.maxGateDistance = bit(13);
    mask.temperatureDeg = bit(14);
    mask.custom1 = bit(15);
    mask.custom2 = bit(16);
    mask.custom3 = bit(17);
    return mask;
}



/// Get the size of the params encoded with the mask: each int and float
/// field takes 4 bytes, each flag takes 1 byte.
int expectedSize(const LrfParamsMask& m)
{
    int size = PARAMS_HEADER_SIZE;
    for (const bool selected : {m.distance, m.timeFromLastMeasurementUs,
                                m.lowPowerMode, m.pointerMode,
                                m.pointerModeTimeoutSec, m.armMode,
                                m.operatingMode, m.continuousMeasuringMode,
                                m.continuousModeTimeoutSec, m.logMode,
                                m.minGateDistance, m.maxGateDistance,
                                m.temperatureDeg, m.custom1, m.custom2,
                                m.custom3})
    {
        size += selected ? 4 : 0;
    }
    for (const bool selected : {m.isOpen, m.isConnected})
    {
        size += selected ? 1 : 0;
    }
    return size;
}



/// Compare a field: the output must be equal to the input if the field is
/// selected and equal to zero if not. The number of mismatches is incremented
/// for each wrong field.
template <class T>
void checkField(const char* name, T in, T out, bool selected, int& errors)
{
    const T expected = selected ? in : T{};
    if (bitsOf(expected) != bitsOf(out))
    {
        std::cout << "Mismatch in field " << name << std::endl;
        ++errors;
    }
}



/// Compare params. Fields which are not selected by the mask must be zero in
/// the output. The initString of the output must be equal to expectedInit.
bool compareParams(const LrfParams& in, const LrfParams& out,
                   const LrfParamsMask& mask, const std::string& expectedInit)
{
    int errors = 0;
    if (out.initString != expectedInit)
    {
        std::cout << "Mismatch in field initString" << std::endl;
        ++errors;
    }
    checkField("distance", in.distance, out.distance, mask.distance, errors);
    checkField("timeFromLastMeasurementUs", in.timeFromLastMeasurementUs,
               out.timeFromLastMeasurementUs, mask.timeFromLastMeasurementUs,
               errors);
    checkField("lowPowerMode", in.lowPowerMode, out.lowPowerMode,
               mask.lowPowerMode, errors);
    checkField("pointerMode", in.pointerMode, out.pointerMode,
               mask.pointerMode, errors);
    checkField("pointerModeTimeoutSec", in.pointerModeTimeoutSec,
               out.pointerModeTimeoutSec, mask.pointerModeTimeoutSec, errors);
    checkField("armMode", in.armMode, out.armMode, mask.armMode, errors);
    checkField("operatingMode", in.operatingMode, out.operatingMode,
               mask.operatingMode, errors);
    checkField("continuousMeasuringMode", in.continuousMeasuringMode,
               out.continuousMeasuringMode, mask.continuousMeasuringMode,
               errors);
    checkField("continuousModeTimeoutSec", in.continuousModeTimeoutSec,
               out.continuousModeTimeoutSec, mask.continuousModeTimeoutSec,
               errors);
    checkField("logMode", in.logMode, out.logMode, mask.logMode, errors);
    checkField("isOpen", in.isOpen, out.isOpen, mask.isOpen, errors);
    checkField("isConnected", in.isConnected, out.isConnected,
               mask.isConnected, errors);
    checkField("minGateDistance", in.minGateDistance, out.minGateDistance,
               mask.minGateDistance, errors);
    checkField("maxGateDistance", in.maxGateDistance, out.maxGateDistance,
               mask.maxGateDistance, errors);
    checkField("temperatureDeg", in.temperatureDeg, out.temperatureDeg,
               mask.temperatureDeg, errors);
    checkField("custom1", in.custom1, out.custom1, mask.custom1, errors);
    checkField("custom2", in.custom2, out.custom2, mask.custom2, errors);
    checkField("custom3", in.custom3, out.custom3, mask.custom3, errors);
    return errors == 0;
}



/// Check that two objects are equal in all fields (bit by bit for floats).
bool sameParams(const LrfParams& a, const LrfParams& b)
{
    const LrfParamsMask all;
    return compareParams(a, b, all, a.initString);
}



/// Print a failed check and return false.
bool fail(const char* message)
{
    std::cout << message << std::endl;
    return false;
}



/// Run one test and print the result.
bool runTest(const char* title, bool (*test)())
{
    std::cout << title << ":" << std::endl;
    const bool passed = test();
    std::cout << (passed ? "OK" : "ERROR") << std::endl << std::endl;
    return passed;
}



/// Removes a file in the destructor. A file which still exists after the
/// removal is reported and makes the test application fail.
class FileRemover
{
public:
    explicit FileRemover(std::string name) : m_name(std::move(name))
    {
    }

    ~FileRemover()
    {
        if (std::remove(m_name.c_str()) != 0 &&
            std::ifstream(m_name).is_open())
        {
            std::cout << "Can't remove file " << m_name << std::endl;
            g_cleanupFailed = true;
        }
    }

    FileRemover(const FileRemover&) = delete;
    FileRemover(FileRemover&&) = delete;
    FileRemover& operator= (const FileRemover&) = delete;
    FileRemover& operator= (FileRemover&&) = delete;

private:

    /// Name of the file to remove.
    std::string m_name;
};



/// Copy test.
bool copyTest()
{
    // Prepare random params.
    LrfParams in;
    prepareRandomParams(in, false);

    // Copy constructor.
    LrfParams copy = in;
    if (!sameParams(in, copy))
    {
        return fail("Copy constructor result is not equal");
    }

    // Copy assignment.
    LrfParams assigned;
    prepareRandomParams(assigned, false);
    assigned = in;
    if (!sameParams(in, assigned))
    {
        return fail("Copy assignment result is not equal");
    }

    // Self assignment.
    LrfParams& self = assigned;
    assigned = self;
    if (!sameParams(in, assigned))
    {
        return fail("Self assignment changed the object");
    }

    return true;
}



/// Encode/decode params test: all fields.
bool encodeDecodeParamsTest()
{
    // Prepare random params.
    LrfParams in;
    prepareRandomParams(in, false);

    // Encode data.
    std::array<uint8_t, 1024> data{};
    int size = 0;
    if (!in.encode(data.data(), 1024, size))
    {
        return fail("Can't encode data");
    }
    std::cout << "Encoded data size: " << size << " bytes" << std::endl;
    if (size != PARAMS_MAX_SIZE)
    {
        return fail("Wrong size of encoded data");
    }

    // Decode data. The output object has other values to check that all
    // fields and the init string are set.
    LrfParams out;
    prepareRandomParams(out, false);
    if (!out.decode(data.data(), size))
    {
        return fail("Can't decode data");
    }

    // Compare params. The init string is not encoded.
    const LrfParamsMask all;
    return compareParams(in, out, all, "");
}



/// Check the layout of the encoded params on hand-made data: the data type,
/// the version, the bit of each field in the mask and the order of the fields.
bool encodeLayoutTest()
{
    // Params with the fields logMode, isOpen and isConnected selected.
    LrfParams in;
    in.logMode = 0x01020304;
    in.isOpen = true;
    in.isConnected = false;
    LrfParamsMask mask = maskFromBits(0);
    mask.logMode = true;
    mask.isOpen = true;
    mask.isConnected = true;

    std::array<uint8_t, PARAMS_MAX_SIZE> expected{};
    expected[0] = 0x02;
    expected[1] = static_cast<uint8_t>(LRF_MAJOR_VERSION);
    expected[2] = static_cast<uint8_t>(LRF_MINOR_VERSION);
    // logMode is the bit 6 of the second mask byte, isOpen is the bit 5,
    // isConnected is the bit 4.
    expected[3] = 0x00;
    expected[4] = 0x40 | 0x20 | 0x10;
    expected[5] = 0x00;
    std::memcpy(&expected[6], &in.logMode, sizeof(in.logMode));
    expected[10] = 0x01;
    expected[11] = 0x00;

    std::array<uint8_t, PARAMS_MAX_SIZE> data{};
    int size = 0;
    if (!in.encode(data.data(), PARAMS_MAX_SIZE, size, &mask))
    {
        return fail("Can't encode data");
    }
    if (size != 12 || std::memcmp(data.data(), expected.data(), 12) != 0)
    {
        return fail("Encoded data differs from the expected bytes");
    }

    // The last two fields use the first two bits of the third mask byte.
    LrfParams in2;
    in2.custom2 = 1.5f;
    in2.custom3 = -2.0f;
    LrfParamsMask mask2 = maskFromBits(0);
    mask2.custom2 = true;
    mask2.custom3 = true;
    if (!in2.encode(data.data(), PARAMS_MAX_SIZE, size, &mask2))
    {
        return fail("Can't encode data");
    }
    const float custom2 = 1.5f;
    const float custom3 = -2.0f;
    std::array<uint8_t, 14> expected2{0x02,
                                      static_cast<uint8_t>(LRF_MAJOR_VERSION),
                                      static_cast<uint8_t>(LRF_MINOR_VERSION),
                                      0x00, 0x00, 0xC0};
    std::memcpy(&expected2[6], &custom2, sizeof(custom2));
    std::memcpy(&expected2[10], &custom3, sizeof(custom3));
    if (size != 14 || std::memcmp(data.data(), expected2.data(), 14) != 0)
    {
        return fail("Encoded data of custom2 and custom3 is wrong");
    }

    // All fields: the mask has 18 bits.
    LrfParams in3;
    if (!in3.encode(data.data(), PARAMS_MAX_SIZE, size) ||
        size != PARAMS_MAX_SIZE || data[3] != 0xFF || data[4] != 0xFF ||
        data[5] != 0xC0)
    {
        return fail("Mask of all fields is wrong");
    }

    return true;
}



/// Encode/decode test with params mask.
bool encodeDecodeParamsWithMaskTest()
{
    // Mask with some fields excluded.
    LrfParams in;
    prepareRandomParams(in, false);
    LrfParamsMask mask;
    mask.lowPowerMode = false;
    mask.pointerModeTimeoutSec = false;
    mask.continuousMeasuringMode = false;
    mask.continuousModeTimeoutSec = false;
    mask.logMode = false;
    mask.minGateDistance = false;
    mask.temperatureDeg = false;
    mask.custom1 = false;
    mask.custom3 = false;

    std::array<uint8_t, 1024> data{};
    int size = 0;
    if (!in.encode(data.data(), 1024, size, &mask))
    {
        return fail("Can't encode data");
    }
    std::cout << "Encoded data size: " << size << " bytes" << std::endl;
    LrfParams out;
    prepareRandomParams(out, false);
    if (!out.decode(data.data(), size))
    {
        return fail("Can't decode data");
    }
    if (!compareParams(in, out, mask, ""))
    {
        return false;
    }

    // Many random masks including empty and full masks. The size of the
    // encoded data and the data type are checked.
    for (int i = 0; i < RANDOM_MASKS_NUMBER; ++i)
    {
        LrfParams randomIn;
        prepareRandomParams(randomIn, false);
        const uint32_t bits = (i == 0) ? 0U :
                              (i == 1) ? 0x3FFFFU : randomBits() & 0x3FFFFU;
        const LrfParamsMask randomMask = maskFromBits(bits);
        LrfParamsMask encodeMask = randomMask;
        if (!randomIn.encode(data.data(), 1024, size, &encodeMask))
        {
            return fail("Can't encode data with random mask");
        }
        if (size != expectedSize(randomMask))
        {
            return fail("Wrong size of data encoded with random mask");
        }
        LrfParams randomOut;
        prepareRandomParams(randomOut, false);
        if (!randomOut.decode(data.data(), size))
        {
            return fail("Can't decode data encoded with random mask");
        }
        if (!compareParams(randomIn, randomOut, randomMask, ""))
        {
            return false;
        }
    }

    return true;
}



/// Buffer boundaries of the encode method.
bool encodeBufferBoundariesTest()
{
    LrfParams in;
    prepareRandomParams(in, false);

    // Null pointer.
    int size = 777;
    if (in.encode(nullptr, 1024, size) || size != 0)
    {
        return fail("Null pointer is not rejected");
    }

    // Buffers which are too small: nothing is written, size is zero.
    for (const int bufferSize : {std::numeric_limits<int>::min(), -1, 0, 1, 6,
                                 PARAMS_MAX_SIZE - 1})
    {
        std::array<uint8_t, 128> buffer{};
        buffer.fill(0xA5);
        size = 777;
        if (in.encode(buffer.data(), bufferSize, size) || size != 0)
        {
            return fail("Too small buffer is not rejected");
        }
        for (const uint8_t value : buffer)
        {
            if (value != 0xA5)
            {
                return fail("Buffer is changed after rejected encode");
            }
        }
    }

    // Exact size: the buffer is allocated with the exact size to let a memory
    // checker see any access outside of it.
    std::vector<uint8_t> exact(PARAMS_MAX_SIZE);
    size = 0;
    if (!in.encode(exact.data(), PARAMS_MAX_SIZE, size) ||
        size != PARAMS_MAX_SIZE)
    {
        return fail("Exact size buffer is not accepted");
    }

    // Nothing is written after the encoded data.
    LrfParamsMask mask = maskFromBits(0x00401U);
    std::array<uint8_t, PARAMS_MAX_SIZE + 16> guarded{};
    guarded.fill(0xA5);
    if (!in.encode(guarded.data(), PARAMS_MAX_SIZE, size, &mask) ||
        size != expectedSize(mask))
    {
        return fail("Can't encode with mask into guarded buffer");
    }
    for (size_t i = static_cast<size_t>(size); i < guarded.size(); ++i)
    {
        if (guarded[i] != 0xA5)
        {
            return fail("Data is written after the encoded data");
        }
    }

    return true;
}



/// Decode of invalid data: every failure leaves the object unchanged.
bool decodeInvalidDataTest()
{
    LrfParams source;
    prepareRandomParams(source, false);
    std::array<uint8_t, 1024> data{};
    int size = 0;
    if (!source.encode(data.data(), 1024, size))
    {
        return fail("Can't encode data");
    }
    LrfParams garbage;
    prepareRandomParams(garbage, false);

    // Null pointer and sizes which can not hold the header.
    LrfParams out = garbage;
    if (out.decode(nullptr, size) || !sameParams(out, garbage))
    {
        return fail("Null pointer is not rejected");
    }
    for (const int badSize : {std::numeric_limits<int>::min(), -1, 0, 1, 5})
    {
        if (out.decode(data.data(), badSize) || !sameParams(out, garbage))
        {
            return fail("Too small size is not rejected");
        }
    }

    // Every truncation of the data: the data is in an exact size buffer.
    for (int length = 0; length < size; ++length)
    {
        std::vector<uint8_t> copy(data.begin(), data.begin() + length);
        if (out.decode(copy.data(), length) || !sameParams(out, garbage))
        {
            std::cout << "Truncated data accepted, length " << length
                      << std::endl;
            return false;
        }
    }

    // Wrong data type, major version and minor version.
    const std::array<uint8_t, 3> expectedHeader{
        0x02, static_cast<uint8_t>(LRF_MAJOR_VERSION),
        static_cast<uint8_t>(LRF_MINOR_VERSION)};
    for (size_t pos = 0; pos < expectedHeader.size(); ++pos)
    {
        for (int value = 0; value < 256; ++value)
        {
            if (value == expectedHeader[pos])
            {
                continue;
            }
            std::array<uint8_t, 1024> copy = data;
            copy[pos] = static_cast<uint8_t>(value);
            if (out.decode(copy.data(), size) || !sameParams(out, garbage))
            {
                return fail("Wrong header is not rejected");
            }
        }
    }

    // The mask selects more fields than the data contains.
    std::array<uint8_t, 1024> shortData = data;
    shortData[3] = 0xFF;
    shortData[4] = 0xFF;
    shortData[5] = 0xFF;
    if (out.decode(shortData.data(), PARAMS_HEADER_SIZE) ||
        !sameParams(out, garbage))
    {
        return fail("Mask without data is not rejected");
    }

    // Bytes after the last encoded field are ignored.
    std::array<uint8_t, 1024> longData = data;
    longData.fill(0xFF);
    std::memcpy(longData.data(), data.data(), static_cast<size_t>(size));
    LrfParams longOut = garbage;
    if (!longOut.decode(longData.data(), size + 5))
    {
        return fail("Trailing bytes are not accepted");
    }
    const LrfParamsMask all;
    if (!compareParams(source, longOut, all, ""))
    {
        return false;
    }

    // A flag is TRUE if its byte is not zero.
    std::array<uint8_t, 1024> flagData = data;
    const int isOpenPos = 6 + 10 * 4;
    flagData[static_cast<size_t>(isOpenPos)] = 0x02;
    flagData[static_cast<size_t>(isOpenPos) + 1] = 0x00;
    LrfParams flagOut = garbage;
    if (!flagOut.decode(flagData.data(), size) || !flagOut.isOpen ||
        flagOut.isConnected)
    {
        return fail("Flag bytes are decoded wrong");
    }

    return true;
}



/// Damaged data: random byte replacement, truncation and extension of valid
/// data. The decoder must not crash, the result must be deterministic, the
/// object must be unchanged after a rejected decode and the init string empty
/// after an accepted one.
bool decodeDamagedDataTest()
{
    int accepted = 0;
    for (int i = 0; i < DAMAGED_SAMPLES_NUMBER; ++i)
    {
        LrfParams in;
        prepareRandomParams(in, false);
        LrfParamsMask mask = maskFromBits(randomBits() & 0x3FFFFU);
        std::array<uint8_t, 1024> data{};
        int size = 0;
        if (!in.encode(data.data(), 1024, size, &mask))
        {
            return fail("Can't encode data");
        }

        // Damage the data.
        std::vector<uint8_t> damaged(data.begin(), data.begin() + size);
        const int kind = randomInt(4);
        if (kind == 0 || kind == 3)
        {
            for (int n = 1 + randomInt(3); n > 0; --n)
            {
                damaged[static_cast<size_t>(randomInt(size))] =
                    static_cast<uint8_t>(randomInt(256));
            }
        }
        if (kind == 1 || kind == 3)
        {
            damaged.resize(static_cast<size_t>(randomInt(size + 1)));
        }
        if (kind == 2)
        {
            for (int n = randomInt(80); n > 0; --n)
            {
                damaged.push_back(static_cast<uint8_t>(randomInt(256)));
            }
        }

        // Decode twice into equal objects.
        LrfParams garbage;
        prepareRandomParams(garbage, false);
        LrfParams first = garbage;
        LrfParams second = garbage;
        std::vector<uint8_t> copy1 = damaged;
        std::vector<uint8_t> copy2 = damaged;
        const int damagedSize = static_cast<int>(damaged.size());
        const bool ok1 = first.decode(copy1.data(), damagedSize);
        const bool ok2 = second.decode(copy2.data(), damagedSize);
        if (ok1 != ok2 || !sameParams(first, second))
        {
            return fail("Decode result is not deterministic");
        }
        if (!ok1 && !sameParams(first, garbage))
        {
            return fail("Object is changed after rejected decode");
        }
        if (ok1)
        {
            ++accepted;
            if (!first.initString.empty())
            {
                return fail("Init string is not empty after decode");
            }
        }
    }
    std::cout << "Samples: " << DAMAGED_SAMPLES_NUMBER << ", accepted: "
              << accepted << std::endl;

    return true;
}



/// Encode/decode commands test: every valid ID and many values.
bool commandsTest()
{
    // Commands: every ID.
    for (int id = FIRST_COMMAND_ID; id <= LAST_COMMAND_ID; ++id)
    {
        std::array<uint8_t, COMMAND_SIZE> data{};
        int size = 0;
        Lrf::encodeCommand(data.data(), size, static_cast<LrfCommand>(id));
        if (size != COMMAND_SIZE || data[0] != 0x00 ||
            data[1] != LRF_MAJOR_VERSION || data[2] != LRF_MINOR_VERSION ||
            std::memcmp(&data[3], &id, sizeof(id)) != 0)
        {
            return fail("Wrong encoded command");
        }
        LrfCommand commandId = LrfCommand::ARM;
        LrfParam paramId = LrfParam::CUSTOM_2;
        float value = 1.5f;
        if (Lrf::decodeCommand(data.data(), size, paramId, commandId,
                               value) != 0 ||
            static_cast<int>(commandId) != id ||
            paramId != LrfParam::CUSTOM_2 || value != 1.5f)
        {
            return fail("Command is not decoded");
        }
    }

    // Set param commands: every ID, many values (NaN and infinity included,
    // the values are only transported).
    for (int id = FIRST_PARAM_ID; id <= LAST_PARAM_ID; ++id)
    {
        for (int i = 0; i < 50; ++i)
        {
            const float outValue = randomFloat();
            std::array<uint8_t, SET_PARAM_SIZE> data{};
            int size = 0;
            Lrf::encodeSetParamCommand(data.data(), size,
                                       static_cast<LrfParam>(id), outValue);
            if (size != SET_PARAM_SIZE || data[0] != 0x01 ||
                data[1] != LRF_MAJOR_VERSION ||
                data[2] != LRF_MINOR_VERSION ||
                std::memcmp(&data[3], &id, sizeof(id)) != 0 ||
                std::memcmp(&data[7], &outValue, sizeof(outValue)) != 0)
            {
                return fail("Wrong encoded set param command");
            }
            LrfCommand commandId = LrfCommand::DISARM;
            LrfParam paramId = LrfParam::CUSTOM_2;
            float value = 0.0f;
            if (Lrf::decodeCommand(data.data(), size, paramId, commandId,
                                   value) != 1 ||
                static_cast<int>(paramId) != id ||
                bitsOf(value) != bitsOf(outValue) ||
                commandId != LrfCommand::DISARM)
            {
                return fail("Set param command is not decoded");
            }
        }
    }

    // Null pointers.
    int size = 777;
    Lrf::encodeCommand(nullptr, size, LrfCommand::ARM);
    if (size != 0)
    {
        return fail("Null pointer is not rejected by encodeCommand");
    }
    size = 777;
    Lrf::encodeSetParamCommand(nullptr, size, LrfParam::ARM_MODE, 1.0f);
    if (size != 0)
    {
        return fail("Null pointer is not rejected by encodeSetParamCommand");
    }

    return true;
}



/// Decode of invalid commands: every failure returns -1 and leaves the output
/// arguments unchanged.
bool invalidCommandsTest()
{
    std::array<uint8_t, 32> command{};
    std::array<uint8_t, 32> setParam{};
    int commandSize = 0;
    int setParamSize = 0;
    Lrf::encodeCommand(command.data(), commandSize, LrfCommand::DISARM);
    Lrf::encodeSetParamCommand(setParam.data(), setParamSize,
                               LrfParam::ARM_MODE, 1.0f);

    // Check that the call is rejected and the outputs are unchanged.
    const auto rejected = [](uint8_t* data, int size)
    {
        LrfParam paramId = LrfParam::CUSTOM_2;
        LrfCommand commandId = LrfCommand::MEASURE_DISTANCE_ONCE;
        float value = 3.25f;
        const int result = Lrf::decodeCommand(
            data, size, paramId, commandId, value);
        return result == -1 && paramId == LrfParam::CUSTOM_2 &&
               commandId == LrfCommand::MEASURE_DISTANCE_ONCE &&
               value == 3.25f;
    };

    // Null pointer.
    if (!rejected(nullptr, COMMAND_SIZE) || !rejected(nullptr, SET_PARAM_SIZE))
    {
        return fail("Null pointer is not rejected");
    }

    // Wrong sizes: a message has a fixed size, a longer one is an error.
    for (int size = -1; size <= 20; ++size)
    {
        if (size != COMMAND_SIZE && !rejected(command.data(), size))
        {
            return fail("Wrong size of command is not rejected");
        }
        if (size != SET_PARAM_SIZE && !rejected(setParam.data(), size))
        {
            return fail("Wrong size of set param command is not rejected");
        }
    }
    if (!rejected(command.data(), std::numeric_limits<int>::min()) ||
        !rejected(command.data(), std::numeric_limits<int>::max()))
    {
        return fail("Extreme sizes are not rejected");
    }

    // Wrong version and data type.
    for (int value = 0; value < 256; ++value)
    {
        std::array<uint8_t, 32> copy = command;
        if (value != LRF_MAJOR_VERSION)
        {
            copy[1] = static_cast<uint8_t>(value);
            if (!rejected(copy.data(), COMMAND_SIZE))
            {
                return fail("Wrong major version is not rejected");
            }
        }
        copy = command;
        if (value != LRF_MINOR_VERSION)
        {
            copy[2] = static_cast<uint8_t>(value);
            if (!rejected(copy.data(), COMMAND_SIZE))
            {
                return fail("Wrong minor version is not rejected");
            }
        }
        copy = command;
        if (value != 0x00 && value != 0x01)
        {
            copy[0] = static_cast<uint8_t>(value);
            if (!rejected(copy.data(), COMMAND_SIZE) ||
                !rejected(copy.data(), SET_PARAM_SIZE))
            {
                return fail("Wrong data type is not rejected");
            }
        }
    }

    // IDs outside of the enums are rejected.
    for (const int id : {std::numeric_limits<int>::min(), -1, 0,
                         LAST_COMMAND_ID + 1, 255, 256,
                         std::numeric_limits<int>::max()})
    {
        std::array<uint8_t, 32> copy = command;
        std::memcpy(&copy[3], &id, sizeof(id));
        if (!rejected(copy.data(), COMMAND_SIZE))
        {
            return fail("Unknown command ID is not rejected");
        }
    }
    for (const int id : {std::numeric_limits<int>::min(), -1, 0,
                         LAST_PARAM_ID + 1, 255, 256,
                         std::numeric_limits<int>::max()})
    {
        std::array<uint8_t, 32> copy = setParam;
        std::memcpy(&copy[3], &id, sizeof(id));
        if (!rejected(copy.data(), SET_PARAM_SIZE))
        {
            return fail("Unknown param ID is not rejected");
        }
    }

    return true;
}



/// Version test: the version string is equal to the version macros and the
/// encoded data contains the major and the minor version.
bool versionTest()
{
    const std::string expected = std::to_string(LRF_MAJOR_VERSION) + "." +
                                 std::to_string(LRF_MINOR_VERSION) + "." +
                                 std::to_string(LRF_PATCH_VERSION);
    std::cout << "Lrf version: " << Lrf::getVersion() << std::endl;
    if (Lrf::getVersion() != expected)
    {
        return fail("Version string differs from the version macros");
    }

    return true;
}



/// JSON read/write test: params are written to a file and read back.
bool jsonReadWriteTest()
{
    const FileRemover remover(JSON_FILE_NAME);

    // The fields which are stored in JSON (see JSON_READABLE in Lrf.h).
    LrfParamsMask stored = maskFromBits(0);
    stored.lowPowerMode = true;
    stored.pointerModeTimeoutSec = true;
    stored.operatingMode = true;
    stored.continuousModeTimeoutSec = true;
    stored.logMode = true;
    stored.minGateDistance = true;
    stored.maxGateDistance = true;
    stored.custom1 = true;
    stored.custom2 = true;
    stored.custom3 = true;

    for (int i = 0; i < JSON_PARAMS_NUMBER; ++i)
    {
        // Write params to file.
        LrfParams in;
        prepareRandomParams(in, true);
        cr::utils::ConfigReader inConfig;
        if (!inConfig.set(in, "LrfParams") ||
            !inConfig.writeToFile(JSON_FILE_NAME))
        {
            return fail("Can't write config file");
        }

        // Read params from file.
        cr::utils::ConfigReader outConfig;
        if (!outConfig.readFromFile(JSON_FILE_NAME))
        {
            return fail("Can't open config file");
        }
        LrfParams out;
        if (!outConfig.get(out, "LrfParams"))
        {
            return fail("Can't read params from file");
        }

        // Compare params: the fields which are not stored have default values.
        if (!compareParams(in, out, stored, in.initString))
        {
            return false;
        }
    }

    return true;
}



/// Check that the JSON document is rejected and the params are unchanged.
bool jsonRejected(const std::string& document, bool readable)
{
    cr::utils::ConfigReader config;
    if (config.readFromString(document) != readable)
    {
        return false;
    }
    LrfParams garbage;
    prepareRandomParams(garbage, true);
    LrfParams out = garbage;
    return !(readable && config.get(out, "LrfParams")) &&
           sameParams(out, garbage);
}



/// Get JSON document with all stored fields. A field value can be replaced.
std::string jsonDocument(const std::string& logMode,
                         const std::string& custom1,
                         const std::string& initString)
{
    return "{\"LrfParams\":{\"continuousModeTimeoutSec\":1,\"custom1\":" +
           custom1 + ",\"custom2\":2,\"custom3\":3,\"initString\":" +
           initString + ",\"logMode\":" + logMode +
           ",\"lowPowerMode\":4,\"maxGateDistance\":5,\"minGateDistance\":6,"
           "\"operatingMode\":7,\"pointerModeTimeoutSec\":8}}";
}



/// JSON documents which are incomplete or have wrong types are rejected
/// without a crash and without changing the params.
bool jsonInvalidDocumentsTest()
{
    // A valid document is read.
    cr::utils::ConfigReader config;
    LrfParams out;
    if (!config.readFromString(jsonDocument("3", "1.5", "\"a\"")) ||
        !config.get(out, "LrfParams") || out.logMode != 3 ||
        out.custom1 != 1.5f || out.initString != "a" ||
        out.lowPowerMode != 4 || out.pointerModeTimeoutSec != 8)
    {
        return fail("Valid document is not read");
    }

    // Documents which can not be parsed.
    for (const char* document : {"", "{", "{\"LrfParams\":", "[1,"})
    {
        if (!jsonRejected(document, false))
        {
            return fail("Document which can not be parsed is accepted");
        }
    }

    // Documents without the params, with missing fields or with wrong types.
    for (const char* document : {"{}", "[1,2]", "{\"LrfParams\":null}",
                                 "{\"LrfParams\":5}", "{\"LrfParams\":{}}",
                                 "{\"LrfParams\":{\"logMode\":3}}"})
    {
        if (!jsonRejected(document, true))
        {
            return fail("Incomplete document is accepted");
        }
    }
    for (const std::string& document : {
             jsonDocument("\"x\"", "1.5", "\"a\""),
             jsonDocument("3000000000", "1.5", "\"a\""),
             jsonDocument("3", "\"1.5\"", "\"a\""),
             jsonDocument("3", "1.5", "7"),
             jsonDocument("3", "1.5", "null")})
    {
        if (!jsonRejected(document, true))
        {
            return fail("Document with a wrong type is accepted");
        }
    }

    return true;
}
}



/// Entry point.
int main()
{
    std::cout << "#####################################" << std::endl;
    std::cout << "#                                   #" << std::endl;
    std::cout << "# Lrf data structures test          #" << std::endl;
    std::cout << "#                                   #" << std::endl;
    std::cout << "#####################################" << std::endl;
    std::cout << std::endl;

    bool passed = true;
    passed &= runTest("Copy test", copyTest);
    passed &= runTest("Encode/Decode params test", encodeDecodeParamsTest);
    passed &= runTest("Encoded params layout test", encodeLayoutTest);
    passed &= runTest("Encode/Decode test with params mask",
                      encodeDecodeParamsWithMaskTest);
    passed &= runTest("Encode buffer boundaries test",
                      encodeBufferBoundariesTest);
    passed &= runTest("Decode invalid data test", decodeInvalidDataTest);
    passed &= runTest("Decode damaged data test", decodeDamagedDataTest);
    passed &= runTest("Encode/Decode commands test", commandsTest);
    passed &= runTest("Decode invalid commands test", invalidCommandsTest);
    passed &= runTest("Version test", versionTest);
    passed &= runTest("JSON read/write test", jsonReadWriteTest);
    passed &= runTest("JSON invalid documents test", jsonInvalidDocumentsTest);

    // The file of the JSON test is removed by now.
    return (passed && !g_cleanupFailed) ? 0 : 1;
}
