#include "CustomLrf.h"



cr::lrf::CustomLrf::CustomLrf()
{

}



cr::lrf::CustomLrf::~CustomLrf()
{

}



std::string cr::lrf::CustomLrf::getVersion()
{
    return CUSTOM_LRF_VERSION;
}



bool cr::lrf::CustomLrf::openLrf(std::string initString)
{
    m_params.isOpen = true;
    m_params.isConnected = true;

    return true;
}



bool cr::lrf::CustomLrf::initLrf(cr::lrf::LrfParams& params)
{
    m_params = params;
    return true;
}



void cr::lrf::CustomLrf::closeLrf()
{
    m_params.isOpen = false;
    m_params.isConnected = false;
}



bool cr::lrf::CustomLrf::isLrfOpen()
{
    return m_params.isOpen;
}



bool cr::lrf::CustomLrf::isLrfConnected()
{
    return m_params.isConnected;
}



bool cr::lrf::CustomLrf::setParam(cr::lrf::LrfParam id, float value)
{
    return true;
}



float cr::lrf::CustomLrf::getParam(cr::lrf::LrfParam id)
{
    return -1.0f;
}



void cr::lrf::CustomLrf::getParams(cr::lrf::LrfParams& params)
{
    params = m_params;
}



bool cr::lrf::CustomLrf::executeCommand(LrfCommand id)
{
    return true;
}



bool cr::lrf::CustomLrf::decodeAndExecuteCommand(uint8_t* data, int size)
{
    // Decode command.
    LrfCommand commandId = LrfCommand::ARM;
    LrfParam paramId = LrfParam::CUSTOM_1;
    float value = 0.0f;
    switch (Lrf::decodeCommand(data, size, paramId, commandId, value))
    {
    // COMMAND.
    case 0:
        // Execute command.
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