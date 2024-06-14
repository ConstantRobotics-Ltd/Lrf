#include "Lens.h"
#include "LensVersion.h"



cr::lens::FovPoint &cr::lens::FovPoint::operator= (const FovPoint &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy params.
    hwZoomPos = src.hwZoomPos;
    xFovDeg = src.xFovDeg;
    yFovDeg = src.yFovDeg;

    return *this;
}



cr::lens::LensParams &cr::lens::LensParams::operator= (const cr::lens::LensParams &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy params.
    initString = src.initString;
    zoomPos = src.zoomPos;
    zoomHwPos = src.zoomHwPos;
    focusPos = src.focusPos;
    focusHwPos = src.focusHwPos;
    irisPos = src.irisPos;
    irisHwPos = src.irisHwPos;
    focusMode = src.focusMode;
    filterMode = src.filterMode;
    afRoiX0 = src.afRoiX0;
    afRoiY0 = src.afRoiY0;
    afRoiX1 = src.afRoiX1;
    afRoiY1 = src.afRoiY1;
    zoomSpeed = src.zoomSpeed;
    zoomHwSpeed = src.zoomHwSpeed;
    zoomHwMaxSpeed = src.zoomHwMaxSpeed;
    focusSpeed = src.focusSpeed;
    focusHwSpeed = src.focusHwSpeed;
    focusHwMaxSpeed = src.focusHwMaxSpeed;
    irisSpeed = src.irisSpeed;
    irisHwSpeed = src.irisHwSpeed;
    irisHwMaxSpeed = src.irisHwMaxSpeed;
    zoomHwTeleLimit = src.zoomHwTeleLimit;
    zoomHwWideLimit = src.zoomHwWideLimit;
    focusHwFarLimit = src.focusHwFarLimit;
    focusHwNearLimit = src.focusHwNearLimit;
    irisHwOpenLimit = src.irisHwOpenLimit;
    irisHwCloseLimit = src.irisHwCloseLimit;
    focusFactor = src.focusFactor;
    isConnected = src.isConnected;
    afHwSpeed = src.afHwSpeed;
    focusFactorThreshold = src.focusFactorThreshold;
    refocusTimeoutSec = src.refocusTimeoutSec;
    afIsActive = src.afIsActive;
    irisMode = src.irisMode;
    autoAfRoiWidth = src.autoAfRoiWidth;
    autoAfRoiHeight = src.autoAfRoiHeight;
    autoAfRoiBorder = src.autoAfRoiBorder;
    afRoiMode = src.afRoiMode;
    extenderMode = src.extenderMode;
    stabiliserMode = src.stabiliserMode;
    afRange = src.afRange;
    xFovDeg = src.xFovDeg;
    yFovDeg = src.yFovDeg;
    logMode = src.logMode;
    temperature = src.temperature;
    isOpen = src.isOpen;
    type = src.type;
    custom1 = src.custom1;
    custom2 = src.custom2;
    custom3 = src.custom3;
    fovPoints = src.fovPoints;

    return *this;
}



bool cr::lens::LensParams::encode(uint8_t* data, int bufferSize, int& size,
                                  cr::lens::LensParamsMask* mask)
{
    // Check buffer size.
    if (bufferSize < 201)
        return false;

    // Encode version.
    int pos = 0;
    data[pos] = 0x02; pos += 1;
    data[pos] = LENS_MAJOR_VERSION; pos += 1;
    data[pos] = LENS_MINOR_VERSION; pos += 1;

    // Set mask.
    cr::lens::LensParamsMask defaultMask;
    if (mask == nullptr)
        mask = &defaultMask;

    // Prepare mask.
    data[pos] = 0;
    data[pos] = data[pos] | (mask->zoomPos ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->zoomHwPos ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusPos ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusHwPos ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisPos ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisHwPos ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusMode ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->filterMode ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->afRoiX0 ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->afRoiY0 ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->afRoiX1 ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->afRoiY1 ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->zoomSpeed ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->zoomHwSpeed ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->zoomHwMaxSpeed ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusSpeed ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->focusHwSpeed ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusHwMaxSpeed ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisSpeed ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisHwSpeed ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisHwMaxSpeed ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->zoomHwTeleLimit ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->zoomHwWideLimit ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusHwFarLimit ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->focusHwNearLimit ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisHwOpenLimit ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisHwCloseLimit ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusFactor ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->isConnected ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->afHwSpeed ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->focusFactorThreshold ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->refocusTimeoutSec ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->afIsActive ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->irisMode ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->autoAfRoiWidth ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->autoAfRoiHeight ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->autoAfRoiBorder ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->afRoiMode ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->extenderMode ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->stabiliserMode ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->afRange ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->xFovDeg ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->yFovDeg ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->logMode ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->temperature ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->isOpen ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->type ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->custom1 ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->custom2 ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->custom3 ? (uint8_t)64 : (uint8_t)0);
    pos += 1;

    // Encode data.
    if (mask->zoomPos)
    {
        memcpy(&data[pos], &zoomPos, 4); pos += 4;
    }
    if (mask->zoomHwPos)
    {
        memcpy(&data[pos], &zoomHwPos, 4); pos += 4;
    }
    if (mask->focusPos)
    {
        memcpy(&data[pos], &focusPos, 4); pos += 4;
    }
    if (mask->focusHwPos)
    {
        memcpy(&data[pos], &focusHwPos, 4); pos += 4;
    }
    if (mask->irisPos)
    {
        memcpy(&data[pos], &irisPos, 4); pos += 4;
    }
    if (mask->irisHwPos)
    {
        memcpy(&data[pos], &irisHwPos, 4); pos += 4;
    }
    if (mask->focusMode)
    {
        memcpy(&data[pos], &focusMode, 4); pos += 4;
    }
    if (mask->filterMode)
    {
        memcpy(&data[pos], &filterMode, 4); pos += 4;
    }
    if (mask->afRoiX0)
    {
        memcpy(&data[pos], &afRoiX0, 4); pos += 4;
    }
    if (mask->afRoiY0)
    {
        memcpy(&data[pos], &afRoiY0, 4); pos += 4;
    }
    if (mask->afRoiX1)
    {
        memcpy(&data[pos], &afRoiX1, 4); pos += 4;
    }
    if (mask->afRoiY1)
    {
        memcpy(&data[pos], &afRoiY1, 4); pos += 4;
    }
    if (mask->zoomSpeed)
    {
        memcpy(&data[pos], &zoomSpeed, 4); pos += 4;
    }
    if (mask->zoomHwSpeed)
    {
        memcpy(&data[pos], &zoomHwSpeed, 4); pos += 4;
    }
    if (mask->zoomHwMaxSpeed)
    {
        memcpy(&data[pos], &zoomHwMaxSpeed, 4); pos += 4;
    }
    if (mask->focusSpeed)
    {
        memcpy(&data[pos], &focusSpeed, 4); pos += 4;
    }
    if (mask->focusHwSpeed)
    {
        memcpy(&data[pos], &focusHwSpeed, 4); pos += 4;
    }
    if (mask->focusHwMaxSpeed)
    {
        memcpy(&data[pos], &focusHwMaxSpeed, 4); pos += 4;
    }
    if (mask->irisSpeed)
    {
        memcpy(&data[pos], &irisSpeed, 4); pos += 4;
    }
    if (mask->irisHwSpeed)
    {
        memcpy(&data[pos], &irisHwSpeed, 4); pos += 4;
    }
    if (mask->irisHwMaxSpeed)
    {
        memcpy(&data[pos], &irisHwMaxSpeed, 4); pos += 4;
    }
    if (mask->zoomHwTeleLimit)
    {
        memcpy(&data[pos], &zoomHwTeleLimit, 4); pos += 4;
    }
    if (mask->zoomHwWideLimit)
    {
        memcpy(&data[pos], &zoomHwWideLimit, 4); pos += 4;
    }
    if (mask->focusHwFarLimit)
    {
        memcpy(&data[pos], &focusHwFarLimit, 4); pos += 4;
    }
    if (mask->focusHwNearLimit)
    {
        memcpy(&data[pos], &focusHwNearLimit, 4); pos += 4;
    }
    if (mask->irisHwOpenLimit)
    {
        memcpy(&data[pos], &irisHwOpenLimit, 4); pos += 4;
    }
    if (mask->irisHwCloseLimit)
    {
        memcpy(&data[pos], &irisHwCloseLimit, 4); pos += 4;
    }
    if (mask->focusFactor)
    {
        memcpy(&data[pos], &focusFactor, 4); pos += 4;
    }
    if (mask->isConnected)
    {
        data[pos] = isConnected ? 0x01 : 0x00; pos += 1;
    }
    if (mask->afHwSpeed)
    {
        memcpy(&data[pos], &afHwSpeed, 4); pos += 4;
    }
    if (mask->focusFactorThreshold)
    {
        memcpy(&data[pos], &focusFactorThreshold, 4); pos += 4;
    }
    if (mask->refocusTimeoutSec)
    {
        memcpy(&data[pos], &refocusTimeoutSec, 4); pos += 4;
    }
    if (mask->afIsActive)
    {
        data[pos] = afIsActive ? 0x01 : 0x00; pos += 1;
    }
    if (mask->irisMode)
    {
        memcpy(&data[pos], &irisMode, 4); pos += 4;
    }
    if (mask->autoAfRoiWidth)
    {
        memcpy(&data[pos], &autoAfRoiWidth, 4); pos += 4;
    }
    if (mask->autoAfRoiHeight)
    {
        memcpy(&data[pos], &autoAfRoiHeight, 4); pos += 4;
    }
    if (mask->autoAfRoiBorder)
    {
        memcpy(&data[pos], &autoAfRoiBorder, 4); pos += 4;
    }
    if (mask->afRoiMode)
    {
        memcpy(&data[pos], &afRoiMode, 4); pos += 4;
    }
    if (mask->extenderMode)
    {
        memcpy(&data[pos], &extenderMode, 4); pos += 4;
    }
    if (mask->stabiliserMode)
    {
        memcpy(&data[pos], &stabiliserMode, 4); pos += 4;
    }
    if (mask->afRange)
    {
        memcpy(&data[pos], &afRange, 4); pos += 4;
    }
    if (mask->xFovDeg)
    {
        memcpy(&data[pos], &xFovDeg, 4); pos += 4;
    }
    if (mask->yFovDeg)
    {
        memcpy(&data[pos], &yFovDeg, 4); pos += 4;
    }
    if (mask->logMode)
    {
        memcpy(&data[pos], &logMode, 4); pos += 4;
    }
    if (mask->temperature)
    {
        memcpy(&data[pos], &temperature, 4); pos += 4;
    }
    if (mask->isOpen)
    {
        data[pos] = isOpen ? 0x01 : 0x00; pos += 1;
    }
    if (mask->type)
    {
        memcpy(&data[pos], &type, 4); pos += 4;
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



bool cr::lens::LensParams::decode(uint8_t* data, int dataSize)
{
    // Check data size.
    if (dataSize < 11)
        return false;

    // Check header.
    if (data[0] != 0x02)
        return false;

    // Check version.
    if (data[1] != LENS_MAJOR_VERSION ||
        data[2] != LENS_MINOR_VERSION)
        return false;

    int pos = 10;
    if ((data[3] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomPos, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomPos = 0;
    }
    if ((data[3] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomHwPos, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomHwPos = 0;
    }
    if ((data[3] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusPos, &data[pos], 4); pos += 4;
    }
    else
    {
        focusPos = 0;
    }
    if ((data[3] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusHwPos, &data[pos], 4); pos += 4;
    }
    else
    {
        focusHwPos = 0;
    }
    if ((data[3] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisPos, &data[pos], 4); pos += 4;
    }
    else
    {
        irisPos = 0;
    }
    if ((data[3] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisHwPos, &data[pos], 4); pos += 4;
    }
    else
    {
        irisHwPos = 0;
    }
    if ((data[3] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusMode, &data[pos], 4); pos += 4;
    }
    else
    {
        focusMode = 0;
    }
    if ((data[3] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&filterMode, &data[pos], 4); pos += 4;
    }
    else
    {
        filterMode = 0;
    }


    if ((data[4] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afRoiX0, &data[pos], 4); pos += 4;
    }
    else
    {
        afRoiX0 = 0;
    }
    if ((data[4] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afRoiY0, &data[pos], 4); pos += 4;
    }
    else
    {
        afRoiY0 = 0;
    }
    if ((data[4] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afRoiX1, &data[pos], 4); pos += 4;
    }
    else
    {
        afRoiX1 = 0;
    }
    if ((data[4] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afRoiY1, &data[pos], 4); pos += 4;
    }
    else
    {
        afRoiY1 = 0;
    }
    if ((data[4] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomSpeed = 0;
    }
    if ((data[4] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomHwSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomHwSpeed = 0;
    }
    if ((data[4] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomHwMaxSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomHwMaxSpeed = 0;
    }
    if ((data[4] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        focusSpeed = 0;
    }



    if ((data[5] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusHwSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        focusHwSpeed = 0;
    }
    if ((data[5] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusHwMaxSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        focusHwMaxSpeed = 0;
    }
    if ((data[5] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        irisSpeed = 0;
    }
    if ((data[5] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisHwSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        irisHwSpeed = 0;
    }
    if ((data[5] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisHwMaxSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        irisHwMaxSpeed = 0;
    }
    if ((data[5] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomHwTeleLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomHwTeleLimit = 0;
    }
    if ((data[5] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&zoomHwWideLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        zoomHwWideLimit = 0;
    }
    if ((data[5] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusHwFarLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        focusHwFarLimit = 0;
    }



    if ((data[6] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusHwNearLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        focusHwNearLimit = 0;
    }
    if ((data[6] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisHwOpenLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        irisHwOpenLimit = 0;
    }
    if ((data[6] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisHwCloseLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        irisHwCloseLimit = 0;
    }
    if ((data[6] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusFactor, &data[pos], 4); pos += 4;
    }
    else
    {
        focusFactor = 0;
    }
    if ((data[6] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 1)
            return false;
        isConnected = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        isConnected = false;
    }
    if ((data[6] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afHwSpeed, &data[pos], 4); pos += 4;
    }
    else
    {
        afHwSpeed = 0;
    }
    if ((data[6] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&focusFactorThreshold, &data[pos], 4); pos += 4;
    }
    else
    {
        focusFactorThreshold = 0;
    }
    if ((data[6] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&refocusTimeoutSec, &data[pos], 4); pos += 4;
    }
    else
    {
        refocusTimeoutSec = 0;
    }



    if ((data[7] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 1)
            return false;
        afIsActive = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        afIsActive = false;
    }
    if ((data[7] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&irisMode, &data[pos], 4); pos += 4;
    }
    else
    {
        irisMode = 0;
    }
    if ((data[7] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&autoAfRoiWidth, &data[pos], 4); pos += 4;
    }
    else
    {
        autoAfRoiWidth = 0;
    }
    if ((data[7] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&autoAfRoiHeight, &data[pos], 4); pos += 4;
    }
    else
    {
        autoAfRoiHeight = 0;
    }
    if ((data[7] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&autoAfRoiBorder, &data[pos], 4); pos += 4;
    }
    else
    {
        autoAfRoiBorder = 0;
    }
    if ((data[7] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afRoiMode, &data[pos], 4); pos += 4;
    }
    else
    {
        afRoiMode = 0;
    }
    if ((data[7] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&extenderMode, &data[pos], 4); pos += 4;
    }
    else
    {
        extenderMode = 0;
    }
    if ((data[7] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&stabiliserMode, &data[pos], 4); pos += 4;
    }
    else
    {
        stabiliserMode = 0;
    }


    if ((data[8] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&afRange, &data[pos], 4); pos += 4;
    }
    else
    {
        afRange = 0;
    }
    if ((data[8] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&xFovDeg, &data[pos], 4); pos += 4;
    }
    else
    {
        xFovDeg = 0.0f;
    }
    if ((data[8] & (uint8_t)32) == (uint8_t)32)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&yFovDeg, &data[pos], 4); pos += 4;
    }
    else
    {
        yFovDeg = 0.0f;
    }
    if ((data[8] & (uint8_t)16) == (uint8_t)16)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&logMode, &data[pos], 4); pos += 4;
    }
    else
    {
        logMode = 0;
    }
    if ((data[8] & (uint8_t)8) == (uint8_t)8)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&temperature, &data[pos], 4); pos += 4;
    }
    else
    {
        temperature = 0;
    }
    if ((data[8] & (uint8_t)4) == (uint8_t)4)
    {
        if (dataSize < pos + 1)
            return false;
        isOpen = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        isOpen = false;
    }
    if ((data[8] & (uint8_t)2) == (uint8_t)2)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&type, &data[pos], 4); pos += 4;
    }
    else
    {
        type = 0;
    }
    if ((data[8] & (uint8_t)1) == (uint8_t)1)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&custom1, &data[pos], 4); pos += 4;
    }
    else
    {
        custom1 = 0.0f;
    }


    if ((data[9] & (uint8_t)128) == (uint8_t)128)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&custom2, &data[pos], 4); pos += 4;
    }
    else
    {
        custom2 = 0.0f;
    }
    if ((data[9] & (uint8_t)64) == (uint8_t)64)
    {
        if (dataSize < pos + 4)
            return false;
        memcpy(&custom3, &data[pos], 4);
    }
    else
    {
        custom3 = 0.0f;
    }

    initString = "";
    fovPoints.clear();

    return true;
}



cr::lens::Lens::~Lens()
{
    
}



std::string cr::lens::Lens::getVersion()
{
    return LENS_VERSION;
}



void cr::lens::Lens::encodeSetParamCommand(uint8_t* data,
                                           int& size,
                                           cr::lens::LensParam id,
                                           float value)
{
    // Fill header.
    data[0] = 0x01;
    data[1] = LENS_MAJOR_VERSION;
    data[2] = LENS_MINOR_VERSION;

    // Fill data.
    int paramId = (int)id;
    memcpy(&data[3], &paramId, 4);
    memcpy(&data[7], &value, 4);
    size = 11;
}



void cr::lens::Lens::encodeCommand(uint8_t* data,
                                   int& size,
                                   cr::lens::LensCommand id,
                                   float arg)
{
    // Fill header.
    data[0] = 0x00;
    data[1] = LENS_MAJOR_VERSION;
    data[2] = LENS_MINOR_VERSION;

    // Fill data.
    int commandId = (int)id;
    memcpy(&data[3], &commandId, 4);
    memcpy(&data[7], &arg, 4);
    size = 11;
}



int cr::lens::Lens::decodeCommand(uint8_t* data,
                                  int size,
                                  cr::lens::LensParam& paramId,
                                  cr::lens::LensCommand& commandId,
                                  float& value)
{
    // Check size.
    if (size != 11)
        return -1;

    // Check version.
    if (data[1] != LENS_MAJOR_VERSION || data[2] != LENS_MINOR_VERSION)
        return -1;

    // Extract data.
    int id = 0;
    memcpy(&id, &data[3], 4);
    memcpy(&value, &data[7], 4);

    // Check command type.
    if (data[0] == 0x00)
    {
        commandId = (LensCommand)id;
        return 0;
    }
    else if (data[0] == 0x01)
    {
        paramId = (LensParam)id;
        return 1;
    }

    return -1;
}
