#pragma once
#include <cstdint>
#include <string>
#include "ConfigReader.h"
#include "LrfVersion.h"



namespace cr
{
namespace lrf
{
/**
 * @brief Enum of laser range finder params.
 *
 * @note CUSTOM_3 is the last value: Lrf::decodeCommand(...) uses it to check
 * the param ID. Keep it the last value when new params are added.
*/
enum class LrfParam
{
    /// [Read only] Current measured distance, meters.
    DISTANCE = 1,
    /// Time from last measurement, us.
    TIME_FROM_LAST_MEASUREMENT_US,
    /// Low power mode. Values depends on implementation. Default: 0 - OFF.
    LOW_POWER_MODE,
    /// Laser pointer mode: 0 - OFF, 1 - ON, 2 - ON timeout.
    POINTER_MODE,
    /// Pointer mode timeout, sec.
    POINTER_MODE_TIMEOUT_SEC,
    /// Arm mode: 0 - Disarmed, 1 - armed.
    ARM_MODE,
    /// Operating mode: 0 - OFF, 1 - Stand-by-mode, 2 - Normal mode.
    OPERATING_MODE,
    /// Continuous measuring mode: 0 - Stop, 1 - 0.5Hz, 2 - 1Hz, 3 - 3Hz,
    /// 4 - 5Hz, 5 - 10Hz.
    CONTINUOUS_MEASURING_MODE,
    /// Continuous mode timeout, sec.
    CONTINUOUS_MODE_TIMEOUT_SEC,
    /// Logging mode: 0 - Disabled, 1 - Console, 2 - File, 3 - Console and file.
    LOG_MODE,
    /// [Read only] Open status.
    IS_OPEN,
    /// [Read only] Connection status.
    IS_CONNECTED,
    /// Min gate distance, meters.
    MIN_GATE_DISTANCE,
    /// Max gate distance, meters.
    MAX_GATE_DISTANCE,
    /// Temperature, deg.
    TEMPERATURE_DEG,
    /// Custom parameter 1. Depends on implementation.
    CUSTOM_1,
    /// Custom parameter 2. Depends on implementation.
    CUSTOM_2,
    /// Custom parameter 3. Depends on implementation.
    CUSTOM_3
};



/**
 * @brief Enum of laser range finder commands.
 *
 * @note MEASURE_DISTANCE_ONCE is the last value: Lrf::decodeCommand(...) uses
 * it to check the command ID. Keep it the last value when new commands are
 * added.
*/
enum class LrfCommand
{
    /// Arm.
    ARM = 1,
    /// Disarm.
    DISARM,
    /// Measure distance once.
    MEASURE_DISTANCE_ONCE
};



/**
 * @brief Mask of laser range finder params.
 *
 * Each flag enables serialization of the LrfParams field with the same name
 * (see LrfParams::encode(...)).
 */
typedef struct LrfParamsMask
{
    /// Serialize distance.
    bool distance{true};
    /// Serialize timeFromLastMeasurementUs.
    bool timeFromLastMeasurementUs{true};
    /// Serialize lowPowerMode.
    bool lowPowerMode{true};
    /// Serialize pointerMode.
    bool pointerMode{true};
    /// Serialize pointerModeTimeoutSec.
    bool pointerModeTimeoutSec{true};
    /// Serialize armMode.
    bool armMode{true};
    /// Serialize operatingMode.
    bool operatingMode{true};
    /// Serialize continuousMeasuringMode.
    bool continuousMeasuringMode{true};
    /// Serialize continuousModeTimeoutSec.
    bool continuousModeTimeoutSec{true};
    /// Serialize logMode.
    bool logMode{true};
    /// Serialize isOpen.
    bool isOpen{true};
    /// Serialize isConnected.
    bool isConnected{true};
    /// Serialize minGateDistance.
    bool minGateDistance{true};
    /// Serialize maxGateDistance.
    bool maxGateDistance{true};
    /// Serialize temperatureDeg.
    bool temperatureDeg{true};
    /// Serialize custom1.
    bool custom1{true};
    /// Serialize custom2.
    bool custom2{true};
    /// Serialize custom3.
    bool custom3{true};
} LrfParamsMask;



/**
 * @brief Laser range finder params class.
 *
 * The values of the fields are only stored and transported by the class: they
 * are not checked against any range. The laser range finder implementation
 * must validate them before use.
 *
 * @note The class is not thread-safe: an object must not be accessed from
 * several threads at the same time without external synchronization.
 */
class LrfParams
{
public:

    /// Initialization string. Format depends on implementation.
    std::string initString;
    /// Current measured distance, meters.
    float distance{0.0f};
    /// Time from last measurement, us.
    int timeFromLastMeasurementUs{0};
    /// Low power mode. Values depends on implementation. Default: 0 - OFF.
    int lowPowerMode{0};
    /// Laser pointer mode: 0 - OFF, 1 - ON, 2 - ON timeout.
    int pointerMode{0};
    /// Pointer mode timeout, sec.
    int pointerModeTimeoutSec{0};
    /// Arm mode: 0 - Disarmed, 1 - armed.
    int armMode{0};
    /// Operating mode: 0 - OFF, 1 - Stand-by-mode, 2 - Normal mode.
    int operatingMode{0};
    /// Continuous measuring mode: 0 - Stop, 1 - 0.5Hz, 2 - 1Hz, 3 - 3Hz,
    /// 4 - 5Hz, 5 - 10Hz.
    int continuousMeasuringMode{0};
    /// Continuous mode timeout, sec.
    int continuousModeTimeoutSec{0};
    /// Logging mode: 0 - Disabled, 1 - Console, 2 - File, 3 - Console and file.
    int logMode{0};
    /// Open status.
    bool isOpen{false};
    /// Connection status.
    bool isConnected{false};
    /// Min gate distance, meters.
    float minGateDistance{0};
    /// Max gate distance, meters.
    float maxGateDistance{0};
    /// Temperature, deg.
    float temperatureDeg{0.0f};
    /// Custom parameter 1. Depends on implementation.
    float custom1{0.0f};
    /// Custom parameter 2. Depends on implementation.
    float custom2{0.0f};
    /// Custom parameter 3. Depends on implementation.
    float custom3{0.0f};

    JSON_READABLE(LrfParams, initString, lowPowerMode, pointerModeTimeoutSec,
                  operatingMode, continuousModeTimeoutSec, logMode,
                  minGateDistance, maxGateDistance, custom1, custom2, custom3)

    /**
     * @brief Default constructor.
     */
    LrfParams() = default;

    /**
     * @brief Copy constructor.
     * @param src Source object.
     */
    LrfParams(const LrfParams& src) = default;

    /**
     * @brief operator =
     * @param src Source object.
     * @return LrfParams object.
     */
    LrfParams& operator= (const LrfParams& src) = default;

    /**
     * @brief Encode (serialize) params. The method doesn't encode initString.
     *
     * Encoded data contains: data type (1 byte), major and minor version of
     * the library (2 bytes), mask of the encoded fields (3 bytes) and values
     * of the selected fields in the order of the fields of LrfParamsMask.
     * Values of int and float fields are stored as 4 bytes in the byte order
     * of the host, flags as 1 byte (0 or 1).
     *
     * @param data Pointer to data buffer. Must not be nullptr. The buffer
     * must be valid for bufferSize bytes.
     * @param bufferSize Size of the data buffer, bytes. Must be >= 72 (the
     * size of data encoded with all fields selected).
     * @param size Size of encoded data, bytes. Set to 0 if the method
     * returns FALSE.
     * @param mask Pointer to params mask. If nullptr all fields are encoded.
     * @return TRUE if params encoded or FALSE if not (data is nullptr or the
     * buffer is too small). Nothing is written to the buffer if FALSE.
     */
    bool encode(uint8_t* data, int bufferSize, int& size,
                LrfParamsMask* mask = nullptr);

    /**
     * @brief Decode (deserialize) params encoded by encode(...) method.
     *
     * The method checks data type, version and size of the data before
     * reading each field. Fields which are not encoded are set to zero
     * (FALSE for flags), initString is set to empty string. A flag is TRUE if
     * its byte is not zero. Bytes after the last encoded field are ignored.
     * Values of fields are not checked against any range.
     *
     * @param data Pointer to encoded data. Must not be nullptr.
     * @param dataSize Size of the encoded data, bytes.
     * @return TRUE if params decoded or FALSE if not. If FALSE is returned
     * the object is not changed.
     */
    bool decode(uint8_t* data, int dataSize);
};



/**
 * @brief Laser range finder controller interface class.
 */
class Lrf
{
public:

    /**
     * @brief Class destructor.
     */
    virtual ~Lrf();

    /**
     * @brief Get class version.
     * @return Class version string in format "Major.Minor.Patch".
     */
    static std::string getVersion();

    /**
     * @brief Open lrf controller. Can be used instead initLrf(...) method.
     * @param initString Init string. Format depends on lrf controller.
     * @return TRUE if the lrf controller is init or FALSE.
     */
    virtual bool openLrf(std::string initString) = 0;

    /**
     * @brief Init lrf controller by set of parameters. Can be used instead
     * openLrf(...) method.
     * @param params Lrf params class.
     * @return TRUE if the lrf controller is init or FALSE.
     */
    virtual bool initLrf(LrfParams& params) = 0;

    /**
     * @brief Close connection.
     */
    virtual void closeLrf() = 0;

    /**
     * @brief Get lrf open status.
     * @return TRUE if the lrf is open or FALSE.
     */
    virtual bool isLrfOpen() = 0;

    /**
     * @brief Get lrf connection status. Lrf can be open but no response from
     * lrf hardware.
     * @return TRUE if the lrf is connected or FALSE.
     */
    virtual bool isLrfConnected() = 0;

    /**
     * @brief Set the lrf controller param.
     * @note The implementation must be thread-safe.
     * @param id Param ID.
     * @param value Param value. The value is not checked by the interface:
     * the implementation must validate it.
     * @return TRUE if the property set or FALSE.
     */
    virtual bool setParam(LrfParam id, float value) = 0;

    /**
     * @brief Get the lrf controller param.
     * @note The implementation must be thread-safe.
     * @param id Param ID.
     * @return float Param value or -1 of the param not exists.
     */
    virtual float getParam(LrfParam id) = 0;

    /**
     * @brief Get the lrf controller params.
     * @note The implementation must be thread-safe.
     * @param params Reference to LrfParams object.
     */
    virtual void getParams(LrfParams& params) = 0;

    /**
     * @brief Execute lrf command.
     * @note The implementation must be thread-safe.
     * @param id Command ID.
     * @return TRUE if the command executed or FALSE.
     */
    virtual bool executeCommand(LrfCommand id) = 0;

    /**
     * @brief Encode set param command.
     * @param data Pointer to data buffer. Must not be nullptr. Must have
     * size >= 11: the method writes 11 bytes.
     * @param size Size of encoded data, bytes. Always 11 if data is not
     * nullptr, 0 if data is nullptr (nothing is written in this case).
     * @param id Lrf parameter id.
     * @param value Lrf parameter value.
     */
    static void encodeSetParamCommand(
        uint8_t* data, int& size, LrfParam id, float value);

    /**
     * @brief Encode command.
     * @param data Pointer to data buffer. Must not be nullptr. Must have
     * size >= 7: the method writes 7 bytes.
     * @param size Size of encoded data, bytes. Always 7 if data is not
     * nullptr, 0 if data is nullptr (nothing is written in this case).
     * @param id Lrf command ID.
     */
    static void encodeCommand(uint8_t* data, int& size, LrfCommand id);

    /**
     * @brief Decode command.
     *
     * The method checks pointer, size, version and ID. The output arguments
     * are changed only if the command is decoded.
     *
     * @param data Pointer to command data. Must not be nullptr.
     * @param size Size of data, bytes. Must be 7 for a command and 11 for a
     * set param command.
     * @param paramId Output param ID (set param command).
     * @param commandId Output command ID (command).
     * @param value Output param value (set param command).
     * @return 0 - command decoded, 1 - set param command decoded, -1 - error
     * (invalid pointer, size, version, type or ID).
     */
    static int decodeCommand(
        uint8_t* data, int size, LrfParam& paramId,
        LrfCommand& commandId, float& value);

    /**
     * @brief Decode and execute command.
     * @note The implementation must be thread-safe.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;

protected:

    /**
     * @brief Default constructor. The class can be used as a base only.
     */
    Lrf() = default;

    /**
     * @brief Copy constructor.
     * @param src Source object.
     */
    Lrf(const Lrf& src) = default;

    /**
     * @brief Move constructor.
     * @param src Source object.
     */
    Lrf(Lrf&& src) = default;

    /**
     * @brief Copy assignment.
     * @param src Source object.
     * @return Reference to this object.
     */
    Lrf& operator= (const Lrf& src) = default;

    /**
     * @brief Move assignment.
     * @param src Source object.
     * @return Reference to this object.
     */
    Lrf& operator= (Lrf&& src) = default;
};
}
}
