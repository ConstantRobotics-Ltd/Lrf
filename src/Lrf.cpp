#include "Lrf.h"



/// Link namespaces.
using namespace std;
using namespace cr::lrf;



LrfParams& LrfParams::operator= (const LrfParams& src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    initString = src.initString;
    distance = src.distance;
    timeFromLastMeasurementUs = src.timeFromLastMeasurementUs;
    lowPowerMode = src.lowPowerMode;
    pointerMode = src.pointerMode;
    pointerModeTimeoutSec = src.pointerModeTimeoutSec;
    armMode = src.armMode;
    operatingMode = src.operatingMode;
    continuousMeasuringMode = src.continuousMeasuringMode;
    continuousModeTimeoutSec = src.continuousModeTimeoutSec;
    logMode = src.logMode;
    isOpen = src.isOpen;
    isConnected = src.isConnected;
    minGateDistance = src.minGateDistance;
    maxGateDistance = src.maxGateDistance;
    temperatureDeg = src.temperatureDeg;
    custom1 = src.custom1;
    custom2 = src.custom2;
    custom3 = src.custom3;

    return *this;
}



bool LrfParams::encode(uint8_t* data, int bufferSize, int& size, LrfParamsMask* mask)
{
    // Check buffer size.
    if (bufferSize < 69)
        return false;

    // Encode version.
    int pos = 0;
    data[pos] = 0x02; pos += 1;
    data[pos] = LRF_MAJOR_VERSION; pos += 1;
    data[pos] = LRF_MINOR_VERSION; pos += 1;

    // Set mask.
    LrfParamsMask defaultMask;
    if (mask == nullptr)
        mask = &defaultMask;
    
    // Prepare mask.
    data[pos] = 0;
    data[pos] = data[pos] | (mask->distance ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->timeFromLastMeasurementUs ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->lowPowerMode ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->pointerMode ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->pointerModeTimeoutSec ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->armMode ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->operatingMode ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->continuousMeasuringMode ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->continuousModeTimeoutSec ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->logMode ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->isOpen ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->isConnected ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->minGateDistance ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->maxGateDistance ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->temperatureDeg ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->custom1 ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->custom2 ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->custom3 ? (uint8_t)64 : (uint8_t)0);
    pos += 1;

    // Encode data.
    if (mask->distance)
    {
        memcpy(&data[pos], &distance, 4); pos += 4;
    }
    if (mask->timeFromLastMeasurementUs)
    {
        memcpy(&data[pos], &timeFromLastMeasurementUs, 4); pos += 4;
    }
    if (mask->lowPowerMode)
    {
        memcpy(&data[pos], &lowPowerMode, 4); pos += 4;
    }
    if (mask->pointerMode)
    {
        memcpy(&data[pos], &pointerMode, 4); pos += 4;
    }
    if (mask->pointerModeTimeoutSec)
    {
        memcpy(&data[pos], &pointerModeTimeoutSec, 4); pos += 4;
    }
    if (mask->armMode)
    {
        memcpy(&data[pos], &armMode, 4); pos += 4;
    }
    if (mask->operatingMode)
    {
        memcpy(&data[pos], &operatingMode, 4); pos += 4;
    }
    if (mask->continuousMeasuringMode)
    {
        memcpy(&data[pos], &continuousMeasuringMode, 4); pos += 4;
    }


    if (mask->continuousModeTimeoutSec)
    {
        memcpy(&data[pos], &continuousModeTimeoutSec, 4); pos += 4;
    }
    if (mask->logMode)
    {
        memcpy(&data[pos], &logMode, 4); pos += 4;
    }
    if (mask->isOpen)
    {
        data[pos] = isOpen ? 0x01 : 0x00; pos += 1;
    }
    if (mask->isConnected)
    {
        data[pos] = isConnected ? 0x01 : 0x00; pos += 1;
    }
    if (mask->minGateDistance)
    {
        memcpy(&data[pos], &minGateDistance, 4); pos += 4;
    }
    if (mask->maxGateDistance)
    {
        memcpy(&data[pos], &maxGateDistance, 4); pos += 4;
    }
    if (mask->temperatureDeg)
    {
        memcpy(&data[pos], &temperatureDeg, 4); pos += 4;
    }
    if (mask->custom1)
    {
        memcpy(&data[pos], &custom1, 4); pos += 4;
    }


    if (mask->custom2)
    {
        memcpy(&data[pos], &custom2, 4); pos += 4;
    }
    if (mask->custom3)
    {
        memcpy(&data[pos], &custom3, 4); pos += 4;
    }

    size = pos;

    return true;
}



bool LrfParams::decode(uint8_t* data, int dataSize)
{
    // Check data size.
    if (dataSize < 6)
        return false;

    // Check header.
    if (data[0] != 0x02)
        return false;

    // Check version.
    if (data[1] != LRF_MAJOR_VERSION ||
        data[2] != LRF_MINOR_VERSION)
        return false;

    int pos = 6;
    if ((data[3] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&distance, &data[pos], 4); pos += 4;
    }
    else
    {
        distance = 0.0f;
    }
    if ((data[3] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&timeFromLastMeasurementUs, &data[pos], 4); pos += 4;
    }
    else
    {
        timeFromLastMeasurementUs = 0;
    }
    if ((data[3] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&lowPowerMode, &data[pos], 4); pos += 4;
    }
    else
    {
        lowPowerMode = 0;
    }
    if ((data[3] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&pointerMode, &data[pos], 4); pos += 4;
    }
    else
    {
        pointerMode = 0;
    }
    if ((data[3] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&pointerModeTimeoutSec, &data[pos], 4); pos += 4;
    }
    else
    {
        pointerModeTimeoutSec = 0;
    }
    if ((data[3] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&armMode, &data[pos], 4); pos += 4;
    }
    else
    {
        armMode = 0;
    }
    if ((data[3] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&operatingMode, &data[pos], 4); pos += 4;
    }
    else
    {
        operatingMode = 0;
    }
    if ((data[3] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&continuousMeasuringMode, &data[pos], 4); pos += 4;
    }
    else
    {
        continuousMeasuringMode = 0;
    }


    if ((data[4] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&continuousModeTimeoutSec, &data[pos], 4); pos += 4;
    }
    else
    {
        continuousModeTimeoutSec = 0;
    }
    if ((data[4] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&logMode, &data[pos], 4); pos += 4;
    }
    else
    {
        logMode = 0;
    }
    if ((data[4] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 1)
            return false;
        isOpen = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        isOpen = false;
    }
    if ((data[4] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 1)
            return false;
        isConnected = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        isConnected = false;
    }
    if ((data[4] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&minGateDistance, &data[pos], 4); pos += 4;
    }
    else
    {
        minGateDistance = 0.0f;
    }
    if ((data[4] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&maxGateDistance, &data[pos], 4); pos += 4;
    }
    else
    {
        maxGateDistance = 0.0f;
    }
    if ((data[4] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&temperatureDeg, &data[pos], 4); pos += 4;
    }
    else
    {
        temperatureDeg = 0.0f;
    }
    if ((data[4] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&custom1, &data[pos], 4); pos += 4;
    }
    else
    {
        custom1 = 0.0f;
    }



    if ((data[5] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&custom2, &data[pos], 4); pos += 4;
    }
    else
    {
        custom2 = 0.0f;
    }
    if ((data[5] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&custom3, &data[pos], 4); pos += 4;
    }
    else
    {
        custom3 = 0.0f;
    }
    

    initString = "";

    return true;
}



Lrf::~Lrf()
{
    
}



std::string Lrf::getVersion()
{
    return LRF_VERSION;
}



void Lrf::encodeSetParamCommand(uint8_t* data, int& size, LrfParam id, float value)
{
    // Fill header.
    data[0] = 0x01;
    data[1] = LRF_MAJOR_VERSION;
    data[2] = LRF_MINOR_VERSION;

    // Fill data.
    int paramId = (int)id;
    memcpy(&data[3], &paramId, 4);
    memcpy(&data[7], &value, 4);
    size = 11;
}



void Lrf::encodeCommand(uint8_t* data, int& size, LrfCommand id)
{
    // Fill header.
    data[0] = 0x00;
    data[1] = LRF_MAJOR_VERSION;
    data[2] = LRF_MINOR_VERSION;

    // Fill data.
    int commandId = (int)id;
    memcpy(&data[3], &commandId, 4);
    size = 7;
}



int Lrf::decodeCommand(uint8_t* data, int size, LrfParam& paramId, LrfCommand& commandId, float& value)
{
    // Check size.
    if (size < 7)
        return -1;

    // Check version.
    if (data[1] != LRF_MAJOR_VERSION || data[2] != LRF_MINOR_VERSION)
        return -1;

    // Check command type.
    if (data[0] == 0x00)
    {
        if (size != 7)
            return -1;
        int id = 0;
        memcpy(&id, &data[3], 4);
        commandId = (LrfCommand)id;
        return 0;
    }
    else if (data[0] == 0x01)
    {
        if (size != 11)
            return -1;
        int id = 0;
        memcpy(&id, &data[3], 4);
        paramId = (LrfParam)id;
        memcpy(&value, &data[7], 4);
        return 1;
    }

    return -1;
}