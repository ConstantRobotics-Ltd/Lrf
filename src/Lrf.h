#pragma once
#include <string.h>
#include <cstdint>
#include <cstdint>
#include "ConfigReader.h"
#include "LrfVersion.h"



namespace cr
{
namespace lrf
{
/**
 * @brief Enum of laser range finder params.
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
    /// Continuous measuring mode: 0 - Stop, 1 - 0.5Hz, 2 - 1 - 1Hz, 3 - 3Hz, 4 - 5Hz, 5 - 10Hz.
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
 * @brief Laser range finder params class.
 */
typedef struct LrfParamsMask
{
    bool distance{true};
    bool timeFromLastMeasurementUs{true};
    bool lowPowerMode{true};
    bool pointerMode{true};
    bool pointerModeTimeoutSec{true};
    bool armMode{true};
    bool operatingMode{true};
    bool continuousMeasuringMode{true};
    bool continuousModeTimeoutSec{true};
    bool logMode{true};
    bool isOpen{true};
    bool isConnected{true};
    bool minGateDistance{true};
    bool maxGateDistance{true};
    bool temperatureDeg{true};
    bool custom1{true};
    bool custom2{true};
    bool custom3{true};
} LrfParamsMask;



/**
 * @brief Laser range finder params class.
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
    /// Continuous measuring mode: 0 - Stop, 1 - 0.5Hz, 2 - 1 - 1Hz, 3 - 3Hz, 4 - 5Hz, 5 - 10Hz.
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
     * @brief operator =
     * @param src Source object.
     * @return LrfParams object.
     */
    LrfParams& operator= (const LrfParams& src);

    /**
     * @brief Encode params. The method doesn't encode initString.
     * @param data Pointer to data buffer.
     * @param size Size of data.
     * @param mask Pointer to params mask.
     * @return TRUE if params encoded or FALSE if not.
     */
    bool encode(uint8_t* data, int bufferSize, int& size,
                LrfParamsMask* mask = nullptr);

    /**
     * @brief Decode params. The method doesn't decode initString.
     * @param data Pointer to data.
     * @brief dataSize Size of data.
     * @return TRUE is params decoded or FALSE if not.
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
     * @brief Init lrf controller by structure. Can be used instead
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
     * @return TRUE if the lrf is open or FALSE.
     */
    virtual bool isLrfConnected() = 0;

    /**
     * @brief Set the lrf controller param.
     * @param id Param ID.
     * @param value Param value.
     * @return TRUE if the property set or FALSE.
     */
    virtual bool setParam(LrfParam id, float value) = 0;

    /**
     * @brief Get the lrf controller param.
     * @param id Param ID.
     * @return float Param value or -1 of the param not exists.
     */
    virtual float getParam(LrfParam id) = 0;

    /**
     * @brief Get the lrf controller params.
     * @param params Reference to LrfParams object.
     */
    virtual void getParams(LrfParams& params) = 0;

    /**
     * @brief Execute lrf command.
     * @param id Command ID.
     * @return TRUE if the command executed or FALSE.
     */
    virtual bool executeCommand(LrfCommand id) = 0;

    /**
     * @brief Encode set param command.
     * @param data Pointer to data buffer. Must have size >= 11.
     * @param size Size of encoded data.
     * @param id Lrf parameter id.
     * @param value Lrf parameter value.
     */
    static void encodeSetParamCommand(
        uint8_t* data, int& size, LrfParam id, float value);

    /**
     * @brief Encode command.
     * @param data Pointer to data buffer. Must have size >= 11.
     * @param size Size of encoded data.
     * @param id Lrf command ID.
     */
    static void encodeCommand(uint8_t* data, int& size, LrfCommand id);

    /**
     * @brief Decode command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @param paramId Output command ID.
     * @param commandId Output command ID.
     * @return 0 - command decoded, 1 - set param command decoded, -1 - error.
     */
    static int decodeCommand(
        uint8_t* data, int size, LrfParam& paramId,
        LrfCommand& commandId, float& value);

    /**
     * @brief Decode and execute command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;
};
}
}