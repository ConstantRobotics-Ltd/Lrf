#pragma once
#include <string>
#include <cstdint>
#include "Lens.h"



namespace cr
{
namespace lens
{
/**
 * @brief Custom lens controller interface class.
 */
class CustomLens: public Lens
{
public:

    /**
     * @brief Class constructor.
     */
    CustomLens();

    /**
     * @brief Class destructor.
     */
    ~CustomLens();

    /**
     * @brief Get lens class version.
     * @return Lens class version string in format "Major.Minor.Patch".
     */
    static std::string getVersion();

    /**
     * @brief Open lens controller. Can be used instead initLens(...) method.
     * @param initString Init string. Format depends on lens controller.
     * @return TRUE if the lens controller is init or FALSE.
     */
    bool openLens(std::string initString);

    /**
     * @brief Init lens controller by structure. Can be used instead
     * openLens(...) method.
     * @param initString Init string. Format depends on lens controller.
     * @return TRUE if the lens controller is init or FALSE.
     */
    bool initLens(LensParams& params);

    /**
     * @brief Close connection.
     */
    void closeLens();

    /**
     * @brief Get lens open status.
     * @return TRUE if the lens is open or FALSE.
     */
    bool isLensOpen();

    /**
     * @brief Get lens connection status. Lens can be open but no response from
     * lens hardware.
     * @return TRUE if the lens is open or FALSE.
     */
    bool isLensConnected();

    /**
     * @brief Set the lens controller param.
     * @param id Param ID.
     * @param value Param value.
     * @return TRUE if the property set or FALSE.
     */
    bool setParam(LensParam id, float value);

    /**
     * @brief Get the lens controller param.
     * @param id Param ID.
     * @return float Param value or -1 of the param not exists.
     */
    float getParam(LensParam id);

    /**
     * @brief Get the lens controller params.
     * @param params Reference to LensParams object.
     */
    void getParams(LensParams& params);

    /**
     * @brief Execute command.
     * @param id Command ID.
     * @param arg Command argument.
     * @return TRUE if the command executed or FALSE.
     */
    bool executeCommand(LensCommand id, float arg = 0);

    /**
     * @brief Add video frame for auto focus purposes. Some lens controllers
     * may not support this functions.
     * @param frame Video frame object.
     */
    void addVideoFrame(cr::video::Frame& frame);

    /**
     * @brief Decode and execute command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    bool decodeAndExecuteCommand(uint8_t* data, int size);

private:

    /// Lens parameters structure (Default params).
    LensParams m_params;
};
}
}
