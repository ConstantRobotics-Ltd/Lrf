#pragma once
#include <cstdint>
#include <mutex>
#include <string>
#include "Lrf.h"
#include "CustomLrfVersion.h"



namespace cr
{
namespace lrf
{
/**
 * @brief Custom lrf controller.
 *
 * The class keeps the params in memory and has no hardware. All methods are
 * thread-safe: params are protected by a mutex.
 */
class CustomLrf: public Lrf
{
public:

    /**
     * @brief Class constructor.
     */
    CustomLrf();

    /**
     * @brief Class destructor.
     */
    ~CustomLrf() override;

    /**
     * @brief Copy constructor. The class owns a mutex: copying is deleted.
     */
    CustomLrf(const CustomLrf&) = delete;

    /**
     * @brief Move constructor. The class owns a mutex: moving is deleted.
     */
    CustomLrf(CustomLrf&&) = delete;

    /**
     * @brief Copy assignment. The class owns a mutex: copying is deleted.
     * @return Reference to this object.
     */
    CustomLrf& operator= (const CustomLrf&) = delete;

    /**
     * @brief Move assignment. The class owns a mutex: moving is deleted.
     * @return Reference to this object.
     */
    CustomLrf& operator= (CustomLrf&&) = delete;

    /**
     * @brief Get class version.
     * @return Class version string in format "Major.Minor.Patch".
     */
    static std::string getVersion();

    /**
     * @brief Open lrf controller. Can be used instead initLrf(...) method.
     * @param initString Init string. Format depends on lrf controller. The
     * example does not use it.
     * @return TRUE if the lrf controller is init or FALSE.
     */
    bool openLrf(std::string initString) override;

    /**
     * @brief Init lrf controller by structure. Can be used instead
     * openLrf(...) method.
     * @param params Lrf params class. Values of the params are checked: if a
     * value is out of its range the method returns FALSE and the controller
     * is not changed. Read only params (distance, time from last measurement
     * and temperature) are not set from outside.
     * @return TRUE if the lrf controller is init or FALSE.
     */
    bool initLrf(LrfParams& params) override;

    /**
     * @brief Close connection.
     */
    void closeLrf() override;

    /**
     * @brief Get lrf open status.
     * @return TRUE if the lrf is open or FALSE.
     */
    bool isLrfOpen() override;

    /**
     * @brief Get lrf connection status. Lrf can be open but no response from
     * lrf hardware.
     * @return TRUE if the lrf is connected or FALSE.
     */
    bool isLrfConnected() override;

    /**
     * @brief Set the lrf controller param.
     * @param id Param ID.
     * @param value Param value. The value must be inside the range of the
     * param (see Lrf.h), NaN and infinity are rejected.
     * @return TRUE if the param set or FALSE (read only param, unknown ID or
     * value out of range). The param is not changed if FALSE is returned.
     */
    bool setParam(LrfParam id, float value) override;

    /**
     * @brief Get the lrf controller param.
     * @param id Param ID.
     * @return float Param value or -1 of the param not exists.
     */
    float getParam(LrfParam id) override;

    /**
     * @brief Get the lrf controller params.
     * @param params Reference to LrfParams object.
     */
    void getParams(LrfParams& params) override;

    /**
     * @brief Execute lrf command.
     * @param id Command ID.
     * @return TRUE if the command executed or FALSE (unknown ID).
     */
    bool executeCommand(LrfCommand id) override;

    /**
     * @brief Decode and execute command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    bool decodeAndExecuteCommand(uint8_t* data, int size) override;

private:

    /// Parameters. Protected by m_mutex.
    LrfParams m_params;
    /// Mutex to protect m_params.
    std::mutex m_mutex;
};
}
}
