#include "CustomLens.h"
#include "CustomLensVersion.h"



cr::lens::CustomLens::CustomLens()
{
    // Reset connection flags.
    m_params.isOpen = false;
    m_params.isConnected = false;
}



cr::lens::CustomLens::~CustomLens()
{

}



std::string cr::lens::CustomLens::getVersion()
{
    return CUSTOM_LENS_VERSION;
}



bool cr::lens::CustomLens::openLens(std::string initString)
{
    // Set connection flags.
    m_params.isOpen = true;
    m_params.isConnected = true;

    return true;
}



bool cr::lens::CustomLens::initLens(cr::lens::LensParams& params)
{
    // Copy params.
    m_params = params;

    // Set connection flags.
    m_params.isOpen = true;
    m_params.isConnected = true;

    return true;
}



void cr::lens::CustomLens::closeLens()
{
    // Reset connection flags.
    m_params.isOpen = false;
    m_params.isConnected = false;
}



bool cr::lens::CustomLens::isLensOpen()
{
    return m_params.isOpen;
}



bool cr::lens::CustomLens::isLensConnected()
{
    return m_params.isConnected;
}



bool cr::lens::CustomLens::setParam(cr::lens::LensParam id, float value)
{
    // Check parameter ID.
    switch (id)
    {
    case cr::lens::LensParam::ZOOM_POS:
    {
        // Save param.
        m_params.zoomPos = (int)value;
        return true;
    }
    case cr::lens::LensParam::ZOOM_HW_POS:
    {
        // Save param.
        m_params.zoomHwPos = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_POS:
    {
        // Save param.
        m_params.focusPos = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_HW_POS:
    {
        // Save param.
        m_params.focusHwPos = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_POS:
    {
        // Save param.
        m_params.irisPos = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_HW_POS:
    {
        // Save param.
        m_params.irisHwPos = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_MODE:
    {
        // Save param.
        m_params.focusMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::FILTER_MODE:
    {
        // Save param.
        m_params.filterMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_ROI_X0:
    {
        // Save param.
        m_params.afRoiX0 = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_ROI_Y0:
    {
        // Save param.
        m_params.afRoiY0 = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_ROI_X1:
    {
        // Save param.
        m_params.afRoiX1 = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_ROI_Y1:
    {
        // Save param.
        m_params.afRoiY1 = (int)value;
        return true;
    }
    case cr::lens::LensParam::ZOOM_SPEED:
    {
        // Save param.
        m_params.zoomSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::ZOOM_HW_SPEED:
    {
        // Save param.
        m_params.zoomHwSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::ZOOM_HW_MAX_SPEED:
    {
        // Save param.
        m_params.zoomHwMaxSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_SPEED:
    {
        // Save param.
        m_params.focusSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_HW_SPEED:
    {
        // Save param.
        m_params.focusHwSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_HW_MAX_SPEED:
    {
        // Save param.
        m_params.focusHwMaxSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_SPEED:
    {
        // Save param.
        m_params.irisSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_HW_SPEED:
    {
        // Save param.
        m_params.irisHwSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_HW_MAX_SPEED:
    {
        // Save param.
        m_params.irisHwMaxSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::ZOOM_HW_TELE_LIMIT:
    {
        // Save param.
        m_params.zoomHwTeleLimit = (int)value;
        return true;
    }
    case cr::lens::LensParam::ZOOM_HW_WIDE_LIMIT:
    {
        // Save param.
        m_params.zoomHwWideLimit = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_HW_FAR_LIMIT:
    {
        // Save param.
        m_params.focusHwNearLimit = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_HW_NEAR_LIMIT:
    {
        // Save param.
        m_params.focusHwNearLimit = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_HW_OPEN_LIMIT:
    {
        // Save param.
        m_params.irisHwOpenLimit = (int)value;
        return true;
    }
    case cr::lens::LensParam::IRIS_HW_CLOSE_LIMIT:
    {
        // Save param.
        m_params.irisHwCloseLimit = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_FACTOR:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::IS_CONNECTED:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::FOCUS_HW_AF_SPEED:
    {
        // Save param.
        m_params.afHwSpeed = (int)value;
        return true;
    }
    case cr::lens::LensParam::FOCUS_FACTOR_THRESHOLD:
    {
        // Save param.
        m_params.focusFactorThreshold = value;
        return true;
    }
    case cr::lens::LensParam::REFOCUS_TIMEOUT_SEC:
    {
        // Save param.
        m_params.refocusTimeoutSec = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_IS_ACTIVE:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::IRIS_MODE:
    {
        // Save param.
        m_params.irisMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::AUTO_AF_ROI_WIDTH:
    {
        // Save param.
        m_params.autoAfRoiWidth = (int)value;
        return true;
    }
    case cr::lens::LensParam::AUTO_AF_ROI_HEIGHT:
    {
        // Save param.
        m_params.autoAfRoiHeight = (int)value;
        return true;
    }
    case cr::lens::LensParam::AUTO_AF_ROI_BORDER:
    {
        // Save param.
        m_params.autoAfRoiBorder = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_ROI_MODE:
    {
        // Save param.
        m_params.afRoiMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::EXTENDER_MODE:
    {
        // Save param.
        m_params.extenderMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::STABILIZER_MODE:
    {
        // Save param.
        m_params.stabiliserMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::AF_RANGE:
    {
        // Save param.
        m_params.afRange = (int)value;
        return true;
    }
    case cr::lens::LensParam::X_FOV_DEG:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::Y_FOV_DEG:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::LOG_MODE:
    {
        // Save param.
        m_params.logMode = (int)value;
        return true;
    }
    case cr::lens::LensParam::TEMPERATURE:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::IS_OPEN:
    {
        // Read only param.
        return false;
    }
    case cr::lens::LensParam::TYPE:
    {
        // Save param.
        m_params.type = (int)value;
        return true;
    }
    case cr::lens::LensParam::CUSTOM_1:
    {
        // Save param.
        m_params.custom1 = value;
        return true;
    }
    case cr::lens::LensParam::CUSTOM_2:
    {
        // Save param.
        m_params.custom2 = value;
        return true;
    }
    case cr::lens::LensParam::CUSTOM_3:
    {
        // Save param.
        m_params.custom3 = value;
        return true;
    }
    default:
    {
        return false;
    }
    }

    return false;
}



float cr::lens::CustomLens::getParam(cr::lens::LensParam id)
{
    // Check parameter ID.
    switch (id)
    {
    case cr::lens::LensParam::ZOOM_POS:
    {
        return (float)m_params.zoomPos;
    }
    case cr::lens::LensParam::ZOOM_HW_POS:
    {
        return (float)m_params.zoomHwPos;
    }
    case cr::lens::LensParam::FOCUS_POS:
    {
        return (float)m_params.focusPos;
    }
    case cr::lens::LensParam::FOCUS_HW_POS:
    {
        return (float)m_params.focusHwPos;
    }
    case cr::lens::LensParam::IRIS_POS:
    {
        return (float)m_params.irisPos;
    }
    case cr::lens::LensParam::IRIS_HW_POS:
    {
        return (float)m_params.irisHwPos;
    }
    case cr::lens::LensParam::FOCUS_MODE:
    {
        return (float)m_params.focusMode;
    }
    case cr::lens::LensParam::FILTER_MODE:
    {
        return (float)m_params.filterMode;
    }
    case cr::lens::LensParam::AF_ROI_X0:
    {
        return (float)m_params.afRoiX0;
    }
    case cr::lens::LensParam::AF_ROI_Y0:
    {
        return (float)m_params.afRoiY0;
    }
    case cr::lens::LensParam::AF_ROI_X1:
    {
        return (float)m_params.afRoiX1;
    }
    case cr::lens::LensParam::AF_ROI_Y1:
    {
        return (float)m_params.afRoiY1;
    }
    case cr::lens::LensParam::ZOOM_SPEED:
    {
        return (float)m_params.zoomSpeed;
    }
    case cr::lens::LensParam::ZOOM_HW_SPEED:
    {
        return (float)m_params.zoomHwSpeed;
    }
    case cr::lens::LensParam::ZOOM_HW_MAX_SPEED:
    {
        return (float)m_params.zoomHwMaxSpeed;
    }
    case cr::lens::LensParam::FOCUS_SPEED:
    {
        return (float)m_params.focusSpeed;
    }
    case cr::lens::LensParam::FOCUS_HW_SPEED:
    {
        return (float)m_params.focusHwSpeed;
    }
    case cr::lens::LensParam::FOCUS_HW_MAX_SPEED:
    {
        return (float)m_params.focusHwMaxSpeed;
    }
    case cr::lens::LensParam::IRIS_SPEED:
    {
        return (float)m_params.irisSpeed;
    }
    case cr::lens::LensParam::IRIS_HW_SPEED:
    {
        return (float)m_params.irisHwSpeed;
    }
    case cr::lens::LensParam::IRIS_HW_MAX_SPEED:
    {
        return (float)m_params.irisHwMaxSpeed;
    }
    case cr::lens::LensParam::ZOOM_HW_TELE_LIMIT:
    {
        return (float)m_params.zoomHwTeleLimit;
    }
    case cr::lens::LensParam::ZOOM_HW_WIDE_LIMIT:
    {
        return (float)m_params.zoomHwWideLimit;
    }
    case cr::lens::LensParam::FOCUS_HW_FAR_LIMIT:
    {
        return (float)m_params.focusHwNearLimit;
    }
    case cr::lens::LensParam::FOCUS_HW_NEAR_LIMIT:
    {
        return (float)m_params.focusHwNearLimit;
    }
    case cr::lens::LensParam::IRIS_HW_OPEN_LIMIT:
    {
        return (float)m_params.irisHwOpenLimit;
    }
    case cr::lens::LensParam::IRIS_HW_CLOSE_LIMIT:
    {
        return (float)m_params.irisHwCloseLimit;
    }
    case cr::lens::LensParam::FOCUS_FACTOR:
    {
        return m_params.focusFactor;
    }
    case cr::lens::LensParam::IS_CONNECTED:
    {
        return m_params.isConnected ? 1.0f : 0.0f;
    }
    case cr::lens::LensParam::FOCUS_HW_AF_SPEED:
    {
        return (float)m_params.afHwSpeed;
    }
    case cr::lens::LensParam::FOCUS_FACTOR_THRESHOLD:
    {
        return m_params.focusFactorThreshold;
    }
    case cr::lens::LensParam::REFOCUS_TIMEOUT_SEC:
    {
        return (float)m_params.refocusTimeoutSec;
    }
    case cr::lens::LensParam::AF_IS_ACTIVE:
    {
        return m_params.afIsActive ? 1.0f : 0.0f;
    }
    case cr::lens::LensParam::IRIS_MODE:
    {
        return (float)m_params.irisMode;
    }
    case cr::lens::LensParam::AUTO_AF_ROI_WIDTH:
    {
        return (float)m_params.autoAfRoiWidth;
    }
    case cr::lens::LensParam::AUTO_AF_ROI_HEIGHT:
    {
        return (float)m_params.autoAfRoiHeight;
    }
    case cr::lens::LensParam::AUTO_AF_ROI_BORDER:
    {
        return (float)m_params.autoAfRoiBorder;
    }
    case cr::lens::LensParam::AF_ROI_MODE:
    {
        return (float)m_params.afRoiMode;
    }
    case cr::lens::LensParam::EXTENDER_MODE:
    {
        return (float)m_params.extenderMode;
    }
    case cr::lens::LensParam::STABILIZER_MODE:
    {
        return (float)m_params.stabiliserMode;
    }
    case cr::lens::LensParam::AF_RANGE:
    {
        return (float)m_params.afRange;
    }
    case cr::lens::LensParam::X_FOV_DEG:
    {
        return m_params.xFovDeg;
    }
    case cr::lens::LensParam::Y_FOV_DEG:
    {
        return m_params.yFovDeg;
    }
    case cr::lens::LensParam::LOG_MODE:
    {
        return (float)m_params.logMode;
    }
    case cr::lens::LensParam::TEMPERATURE:
    {
        return m_params.temperature;
    }
    case cr::lens::LensParam::IS_OPEN:
    {
        return m_params.isOpen ? 1.0f : 0.0f;
    }
    case cr::lens::LensParam::TYPE:
    {
        return (float)m_params.type;
    }
    case cr::lens::LensParam::CUSTOM_1:
    {
        return m_params.custom1;
    }
    case cr::lens::LensParam::CUSTOM_2:
    {
        return m_params.custom2;
    }
    case cr::lens::LensParam::CUSTOM_3:
    {
        return m_params.custom3;
    }
    default:
    {
        return -1.0f;
    }
    }

    return -1.0f;
}



void cr::lens::CustomLens::getParams(cr::lens::LensParams& params)
{
    params = m_params;
}



bool cr::lens::CustomLens::executeCommand(cr::lens::LensCommand id, float arg)
{
    // Check command ID.
    switch (id)
    {
    case cr::lens::LensCommand::ZOOM_TELE:
    {
        return true;
    }
    case cr::lens::LensCommand::ZOOM_WIDE:
    {
        return true;
    }
    case cr::lens::LensCommand::ZOOM_TO_POS:
    {
        return true;
    }
    case cr::lens::LensCommand::ZOOM_STOP:
    {
        return true;
    }
    case cr::lens::LensCommand::FOCUS_FAR:
    {
        return true;
    }
    case cr::lens::LensCommand::FOCUS_NEAR:
    {
        return true;
    }
    case cr::lens::LensCommand::FOCUS_TO_POS:
    {
        return true;
    }
    case cr::lens::LensCommand::FOCUS_STOP:
    {
        return true;
    }
    case cr::lens::LensCommand::IRIS_OPEN:
    {
        return true;
    }
    case cr::lens::LensCommand::IRIS_CLOSE:
    {
        return true;
    }
    case cr::lens::LensCommand::IRIS_TO_POS:
    {
        return true;
    }
    case cr::lens::LensCommand::IRIS_STOP:
    {
        return true;
    }
    case cr::lens::LensCommand::AF_START:
    {
        return true;
    }
    case cr::lens::LensCommand::AF_STOP:
    {
        return true;
    }
    case cr::lens::LensCommand::RESTART:
    {
        return true;
    }
    case cr::lens::LensCommand::DETECT_HW_RANGES:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }

    return false;
}



void cr::lens::CustomLens::addVideoFrame(cr::video::Frame& frame)
{

}



bool cr::lens::CustomLens::decodeAndExecuteCommand(uint8_t* data, int size)
{
    // Decode command.
    LensCommand commandId = LensCommand::ZOOM_TELE;
    LensParam paramId = LensParam::ZOOM_SPEED;
    float value = 0.0f;
    switch (Lens::decodeCommand(data, size, paramId, commandId, value))
    {
    // COMMAND.
    case 0:
        // Executte command.
        return executeCommand(commandId);
    // SET_PARAM.
    case 1:
    {
        // Set param.
        return setParam(paramId, value);
    }
    default:
    {
        return false;
    }
    }

    return false;
}
