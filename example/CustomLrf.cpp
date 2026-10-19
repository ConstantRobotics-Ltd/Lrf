#include "CustomLrf.h"
#include <cmath>
#include <cstdint>
#include <limits>
#include <mutex>
#include <string>



namespace
{
/// Limit of float values which can be converted to int. 2^31 is exactly
/// representable as float, so (-LIMIT <= value < LIMIT) is the exact range of
/// int.
constexpr float INT_RANGE_LIMIT{2147483648.0f};
/// Max value of the low power mode: 0 - OFF, 1 - ON.
constexpr int MAX_LOW_POWER_MODE{1};
/// Max value of the laser pointer mode: 0 - OFF, 1 - ON, 2 - ON timeout.
constexpr int MAX_POINTER_MODE{2};
/// Max value of the arm mode: 0 - Disarmed, 1 - armed.
constexpr int MAX_ARM_MODE{1};
/// Max value of the operating mode: 0 - OFF, 1 - Stand-by-mode, 2 - Normal.
constexpr int MAX_OPERATING_MODE{2};
/// Max value of the continuous measuring mode: 0 - Stop, 1 - 0.5Hz, 2 - 1Hz,
/// 3 - 3Hz, 4 - 5Hz, 5 - 10Hz.
constexpr int MAX_CONTINUOUS_MEASURING_MODE{5};
/// Max value of the logging mode: 0 - Disabled, 1 - Console, 2 - File,
/// 3 - Console and file.
constexpr int MAX_LOG_MODE{3};
/// Max value of the timeouts, sec.
constexpr int MAX_TIMEOUT_SEC{std::numeric_limits<int>::max()};



/// Convert float param value to int. A conversion of NaN or of a value outside
/// of the int range is undefined behavior, so such values are rejected.
bool toInt(float value, int& result)
{
    if (!(value >= -INT_RANGE_LIMIT && value < INT_RANGE_LIMIT))
    {
        return false;
    }
    result = static_cast<int>(value);
    return true;
}



/// Check that the value is inside [minValue, maxValue].
bool inRange(int value, int minValue, int maxValue)
{
    return value >= minValue && value <= maxValue;
}



/// Assign the float param value to the int target if the value is inside
/// [minValue, maxValue]. The target is not changed if not.
bool assignInRange(int& target, float value, int minValue, int maxValue)
{
    int converted = 0;
    if (!toInt(value, converted) || !inRange(converted, minValue, maxValue))
    {
        return false;
    }
    target = converted;
    return true;
}



/// Check that the gate distance is a finite non-negative number.
bool isValidGate(float value)
{
    return std::isfinite(value) && value >= 0.0f;
}



/// Assign the float param value to the target if the value is finite.
bool assignFinite(float& target, float value)
{
    if (!std::isfinite(value))
    {
        return false;
    }
    target = value;
    return true;
}



/// Assign the gate distance to the target if the value is valid.
bool assignGate(float& target, float value)
{
    if (!isValidGate(value))
    {
        return false;
    }
    target = value;
    return true;
}



/// Check values of the writable params.
bool isValidParams(const cr::lrf::LrfParams& p)
{
    return inRange(p.lowPowerMode, 0, MAX_LOW_POWER_MODE) &&
           inRange(p.pointerMode, 0, MAX_POINTER_MODE) &&
           inRange(p.pointerModeTimeoutSec, 0, MAX_TIMEOUT_SEC) &&
           inRange(p.armMode, 0, MAX_ARM_MODE) &&
           inRange(p.operatingMode, 0, MAX_OPERATING_MODE) &&
           inRange(p.continuousMeasuringMode, 0,
                   MAX_CONTINUOUS_MEASURING_MODE) &&
           inRange(p.continuousModeTimeoutSec, 0, MAX_TIMEOUT_SEC) &&
           inRange(p.logMode, 0, MAX_LOG_MODE) &&
           isValidGate(p.minGateDistance) &&
           isValidGate(p.maxGateDistance) &&
           std::isfinite(p.custom1) &&
           std::isfinite(p.custom2) &&
           std::isfinite(p.custom3);
}
}



cr::lrf::CustomLrf::CustomLrf() = default;



cr::lrf::CustomLrf::~CustomLrf() = default;



std::string cr::lrf::CustomLrf::getVersion()
{
    return CUSTOM_LRF_VERSION;
}



bool cr::lrf::CustomLrf::openLrf(std::string)
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    m_params.isOpen = true;
    m_params.isConnected = true;

    return true;
}



bool cr::lrf::CustomLrf::initLrf(cr::lrf::LrfParams& params)
{
    // Check the values before changing anything.
    if (!isValidParams(params))
    {
        return false;
    }

    const std::lock_guard<std::mutex> lock(m_mutex);

    // Read only values are produced by the controller and are not set from
    // outside.
    LrfParams result = params;
    result.distance = m_params.distance;
    result.timeFromLastMeasurementUs = m_params.timeFromLastMeasurementUs;
    result.temperatureDeg = m_params.temperatureDeg;
    result.isOpen = true;
    result.isConnected = true;
    m_params = result;

    return true;
}



void cr::lrf::CustomLrf::closeLrf()
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    m_params.isOpen = false;
    m_params.isConnected = false;
}



bool cr::lrf::CustomLrf::isLrfOpen()
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    return m_params.isOpen;
}



bool cr::lrf::CustomLrf::isLrfConnected()
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    return m_params.isConnected;
}



bool cr::lrf::CustomLrf::setParam(cr::lrf::LrfParam id, float value)
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    switch (id)
    {
    case LrfParam::LOW_POWER_MODE:
        return assignInRange(m_params.lowPowerMode, value, 0,
                             MAX_LOW_POWER_MODE);
    case LrfParam::POINTER_MODE:
        return assignInRange(m_params.pointerMode, value, 0, MAX_POINTER_MODE);
    case LrfParam::POINTER_MODE_TIMEOUT_SEC:
        return assignInRange(m_params.pointerModeTimeoutSec, value, 0,
                             MAX_TIMEOUT_SEC);
    case LrfParam::ARM_MODE:
        return assignInRange(m_params.armMode, value, 0, MAX_ARM_MODE);
    case LrfParam::OPERATING_MODE:
        return assignInRange(m_params.operatingMode, value, 0,
                             MAX_OPERATING_MODE);
    case LrfParam::CONTINUOUS_MEASURING_MODE:
        return assignInRange(m_params.continuousMeasuringMode, value, 0,
                             MAX_CONTINUOUS_MEASURING_MODE);
    case LrfParam::CONTINUOUS_MODE_TIMEOUT_SEC:
        return assignInRange(m_params.continuousModeTimeoutSec, value, 0,
                             MAX_TIMEOUT_SEC);
    case LrfParam::LOG_MODE:
        return assignInRange(m_params.logMode, value, 0, MAX_LOG_MODE);
    case LrfParam::MIN_GATE_DISTANCE:
        return assignGate(m_params.minGateDistance, value);
    case LrfParam::MAX_GATE_DISTANCE:
        return assignGate(m_params.maxGateDistance, value);
    case LrfParam::CUSTOM_1:
        return assignFinite(m_params.custom1, value);
    case LrfParam::CUSTOM_2:
        return assignFinite(m_params.custom2, value);
    case LrfParam::CUSTOM_3:
        return assignFinite(m_params.custom3, value);
    // Read only params: they are produced by the controller.
    case LrfParam::DISTANCE:
    case LrfParam::TIME_FROM_LAST_MEASUREMENT_US:
    case LrfParam::IS_OPEN:
    case LrfParam::IS_CONNECTED:
    case LrfParam::TEMPERATURE_DEG:
    default:
        return false;
    }
}



float cr::lrf::CustomLrf::getParam(cr::lrf::LrfParam id)
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    switch (id)
    {
    case LrfParam::DISTANCE:
        return m_params.distance;
    case LrfParam::TIME_FROM_LAST_MEASUREMENT_US:
        return static_cast<float>(m_params.timeFromLastMeasurementUs);
    case LrfParam::LOW_POWER_MODE:
        return static_cast<float>(m_params.lowPowerMode);
    case LrfParam::POINTER_MODE:
        return static_cast<float>(m_params.pointerMode);
    case LrfParam::POINTER_MODE_TIMEOUT_SEC:
        return static_cast<float>(m_params.pointerModeTimeoutSec);
    case LrfParam::ARM_MODE:
        return static_cast<float>(m_params.armMode);
    case LrfParam::OPERATING_MODE:
        return static_cast<float>(m_params.operatingMode);
    case LrfParam::CONTINUOUS_MEASURING_MODE:
        return static_cast<float>(m_params.continuousMeasuringMode);
    case LrfParam::CONTINUOUS_MODE_TIMEOUT_SEC:
        return static_cast<float>(m_params.continuousModeTimeoutSec);
    case LrfParam::LOG_MODE:
        return static_cast<float>(m_params.logMode);
    case LrfParam::IS_OPEN:
        return m_params.isOpen ? 1.0f : 0.0f;
    case LrfParam::IS_CONNECTED:
        return m_params.isConnected ? 1.0f : 0.0f;
    case LrfParam::MIN_GATE_DISTANCE:
        return m_params.minGateDistance;
    case LrfParam::MAX_GATE_DISTANCE:
        return m_params.maxGateDistance;
    case LrfParam::TEMPERATURE_DEG:
        return m_params.temperatureDeg;
    case LrfParam::CUSTOM_1:
        return m_params.custom1;
    case LrfParam::CUSTOM_2:
        return m_params.custom2;
    case LrfParam::CUSTOM_3:
        return m_params.custom3;
    default:
        return -1.0f;
    }
}



void cr::lrf::CustomLrf::getParams(cr::lrf::LrfParams& params)
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    params = m_params;
}



bool cr::lrf::CustomLrf::executeCommand(LrfCommand id)
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    switch (id)
    {
    case LrfCommand::ARM:
        m_params.armMode = 1;
        return true;
    case LrfCommand::DISARM:
        m_params.armMode = 0;
        return true;
    // The example has no hardware: the command is accepted.
    case LrfCommand::MEASURE_DISTANCE_ONCE:
        return true;
    default:
        return false;
    }
}



bool cr::lrf::CustomLrf::decodeAndExecuteCommand(uint8_t* data, int size)
{
    // Decode command. The decoder checks the pointer, the size, the version
    // and the IDs.
    LrfCommand commandId = LrfCommand::ARM;
    LrfParam paramId = LrfParam::CUSTOM_1;
    float value = 0.0f;
    const int result = Lrf::decodeCommand(
        data, size, paramId, commandId, value);

    // Execute the decoded command.
    if (result == 0)
    {
        return executeCommand(commandId);
    }
    if (result == 1)
    {
        return setParam(paramId, value);
    }

    return false;
}
