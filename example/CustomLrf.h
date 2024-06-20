#pragma once
#include <string>
#include <cstdint>
#include "Lrf.h"
#include "CustomLrfVersion.h"



namespace cr
{
namespace lrf
{
/**
 * @brief Custom lrf controller.
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
    ~CustomLrf();

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
    bool openLrf(std::string initString) override;

    /**
     * @brief Init lrf controller by structure. Can be used instead
     * openLrf(...) method.
     * @param params Lrf params class.
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
     * @return TRUE if the lrf is open or FALSE.
     */
    bool isLrfConnected() override;

    /**
     * @brief Set the lrf controller param.
     * @param id Param ID.
     * @param value Param value.
     * @return TRUE if the property set or FALSE.
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
     * @return TRUE if the command executed or FALSE.
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

    // Parameters class.
    LrfParams m_params;
};
}
}
