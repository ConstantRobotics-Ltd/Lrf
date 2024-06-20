#include <iostream>
#include "Lrf.h"



/// Link namespaces.
using namespace cr::lrf;
using namespace std;



/// Prepare random params.
void prepareRandomParams(LrfParams& params);



/// Copy test.
bool copyTest();

/// Encode/decode params test.
bool encodeDecodeParamsTest();

/// Encode/decode test with params mask.
bool encodeDecodeParamsWithMaskTest();

/// Encode/decode commands test.
bool encodeDecodeCommandsTest();

/// JSON read/write test.
bool jsonReadWriteTest();

/// Compare params.
bool compareParams(LrfParams& in, LrfParams& out, LrfParamsMask& mask);



/// Entry point.
int main(void)
{
    cout << "#####################################" << endl;
    cout << "#                                   #" << endl;
    cout << "# Lrf data structures test          #" << endl;
    cout << "#                                   #" << endl;
    cout << "#####################################" << endl;
    cout << endl;

    cout << "Copy test:" << endl;
    if (copyTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode params test:" << endl;
    if (encodeDecodeParamsTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode test with params mask:" << endl;
    if (encodeDecodeParamsWithMaskTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode commands test:" << endl;
    if (encodeDecodeCommandsTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "JSON read/write test:" << endl;
    if (jsonReadWriteTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    return 1;
}



// Copy test.
bool copyTest()
{
    // Prepare random params.
    LrfParams in;
    prepareRandomParams(in);

    // Copy params.
    LrfParams out = in;

    // Compare params.
    LrfParamsMask mask;
    return compareParams(in, out, mask);
}



// Encode/decode test.
bool encodeDecodeParamsTest()
{
    // Prepare random params.
    LrfParams in;
    prepareRandomParams(in);

    // Encode data.
    uint8_t data[1024];
    int size = 0;
    in.encode(data, 1024, size);

    cout << "Encoded data size: " << size << " bytes" << endl;

    // Decode data.
    LrfParams out;
    out.isConnected = false;
    out.isOpen = false;
    if (!out.decode(data, size))
    {
        cout << "Can't decode data" << endl;
        return false;
    }

    // Compare params.
    LrfParamsMask mask;
    return compareParams(in, out, mask);
}



// Encode/decode commands test.
bool encodeDecodeCommandsTest()
{
    // Encode command.
    uint8_t data[1024];
    int size = 0;
    Lrf::encodeCommand(data, size, LrfCommand::ARM);

    // Decode command.
    LrfCommand commandId;
    LrfParam paramId;
    float value = 0.0f;
    if (Lrf::decodeCommand(data, size, paramId, commandId, value) != 0)
    {
        cout << "Command not decoded" << endl;
        return false;
    }

    // Check ID and value.
    if (commandId != LrfCommand::ARM)
    {
        cout << "not a LrfCommand::ARM" << endl;
        return false;
    }

    // Encode param.
    float outValue = (float)(rand() % 20);
    Lrf::encodeSetParamCommand(data, size, LrfParam::ARM_MODE, outValue);

    // Decode command.
    value = 0.0f;
    if (Lrf::decodeCommand(data, size, paramId, commandId, value) != 1)
    {
        cout << "Set param command not decoded" << endl;
        return false;
    }

    // Check ID and value.
    if (paramId != LrfParam::ARM_MODE)
    {
        cout << "not a LrfParam::ARM_MODE" << endl;
        return false;
    }
    if (value != outValue)
    {
        cout << "not equal value" << endl;
        return false;
    }

    return true;
}



/// JSON read/write test.
bool jsonReadWriteTest()
{
    // Prepare random params.
    LrfParams in;
    prepareRandomParams(in);

    // Write params to file.
    cr::utils::ConfigReader inConfig;
    inConfig.set(in, "LrfParams");
    inConfig.writeToFile("TestLrfParams.json");

    // Read params from file.
    cr::utils::ConfigReader outConfig;
    if(!outConfig.readFromFile("TestLrfParams.json"))
    {
        cout << "Can't open config file" << endl;
        return false;
    }

    LrfParams out;
    if(!outConfig.get(out, "LrfParams"))
    {
        cout << "Can't read params from file" << endl;
        return false;
    }

    // Compare params.
    LrfParamsMask mask;
    mask.distance = false;
    mask.timeFromLastMeasurementUs = false;
    mask.lowPowerMode = true;
    mask.pointerMode = false;
    mask.pointerModeTimeoutSec = true;
    mask.armMode = false;
    mask.operatingMode = true;
    mask.continuousMeasuringMode = false;
    mask.continuousModeTimeoutSec = true;
    mask.logMode = true;
    mask.isOpen = false;
    mask.isConnected = false;
    mask.minGateDistance = true;
    mask.maxGateDistance = true;
    mask.temperatureDeg = false;
    mask.custom1 = true;
    mask.custom2 = true;
    mask.custom3 = true;
    return compareParams(in, out, mask);
}




/// Encode/decode test with params mask.
bool encodeDecodeParamsWithMaskTest()
{
    // Prepare random params.
    LrfParams in;
    prepareRandomParams(in);

    // Prepare mask.
    LrfParamsMask mask;
    mask.distance = true;
    mask.timeFromLastMeasurementUs = true;
    mask.lowPowerMode = false;
    mask.pointerMode = true;
    mask.pointerModeTimeoutSec = false;
    mask.armMode = true;
    mask.operatingMode = true;
    mask.continuousMeasuringMode = false;
    mask.continuousModeTimeoutSec = false;
    mask.logMode = false;
    mask.isOpen = true;
    mask.isConnected = true;
    mask.minGateDistance = false;
    mask.maxGateDistance = true;
    mask.temperatureDeg = false;
    mask.custom1 = false;
    mask.custom2 = true;
    mask.custom3 = false;

    // Encode data.
    uint8_t data[1024];
    int size = 0;
    in.encode(data, 1024, size, &mask);

    cout << "Encoded data size: " << size << " bytes" << endl;

    // Decode data.
    LrfParams out;
    if (!out.decode(data, size))
    {
        cout << "Can't decode data" << endl;
        return false;
    }

    // Compare params.
    return compareParams(in, out, mask);
}



bool compareParams(LrfParams& in, LrfParams& out, LrfParamsMask& mask)
{
    bool result = true;
    if (in.initString != out.initString)
    {
        cout << "in.initString" << endl;
    }
    if (in.distance != out.distance && mask.distance)
    {
        cout << "in.distance" << endl;
        result = false;
    }
    if (in.timeFromLastMeasurementUs != out.timeFromLastMeasurementUs && mask.timeFromLastMeasurementUs)
    {
        cout << "in.timeFromLastMeasurementUs" << endl;
        result = false;
    }
    if (in.lowPowerMode != out.lowPowerMode && mask.lowPowerMode)
    {
        cout << "in.lowPowerMode" << endl;
        result = false;
    }
    if (in.pointerMode != out.pointerMode && mask.pointerMode)
    {
        cout << "in.pointerMode" << endl;
        result = false;
    }
    if (in.pointerModeTimeoutSec != out.pointerModeTimeoutSec && mask.pointerModeTimeoutSec)
    {
        cout << "in.pointerModeTimeoutSec" << endl;
        result = false;
    }
    if (in.armMode != out.armMode && mask.armMode)
    {
        cout << "in.armMode" << endl;
        result = false;
    }
    if (in.operatingMode != out.operatingMode && mask.operatingMode)
    {
        cout << "in.operatingMode" << endl;
        result = false;
    }
    if (in.continuousMeasuringMode != out.continuousMeasuringMode && mask.continuousMeasuringMode)
    {
        cout << "in.continuousMeasuringMode" << endl;
        result = false;
    }
    if (in.continuousModeTimeoutSec != out.continuousModeTimeoutSec && mask.continuousModeTimeoutSec)
    {
        cout << "in.continuousModeTimeoutSec" << endl;
        result = false;
    }
    if (in.logMode != out.logMode && mask.logMode)
    {
        cout << "in.logMode" << endl;
        result = false;
    }
    if (in.isOpen != out.isOpen && mask.isOpen)
    {
        cout << "in.isOpen" << endl;
        result = false;
    }
    if (in.isConnected != out.isConnected && mask.isConnected)
    {
        cout << "in.isConnected" << endl;
        result = false;
    }
    if (in.minGateDistance != out.minGateDistance && mask.minGateDistance)
    {
        cout << "in.minGateDistance" << endl;
        result = false;
    }
    if (in.maxGateDistance != out.maxGateDistance && mask.maxGateDistance)
    {
        cout << "in.maxGateDistance" << endl;
        result = false;
    }
    if (in.temperatureDeg != out.temperatureDeg && mask.temperatureDeg)
    {
        cout << "in.temperatureDeg" << endl;
        result = false;
    }
    if (in.custom1 != out.custom1 && mask.custom1)
    {
        cout << "in.custom1" << endl;
        result = false;
    }
    if (in.custom2 != out.custom2 && mask.custom2)
    {
        cout << "in.custom2" << endl;
        result = false;
    }
    if (in.custom3 != out.custom3 && mask.custom3)
    {
        cout << "in.custom3" << endl;
        result = false;
    }

    return result;
}



void prepareRandomParams(LrfParams& params)
{
    params.initString = to_string(rand() % 255) + to_string(rand() % 255);
    params.distance = (float)(rand() % 255) / 100.0f;
    params.timeFromLastMeasurementUs = rand() % 255;
    params.lowPowerMode = rand() % 255;
    params.pointerMode = rand() % 255;
    params.pointerModeTimeoutSec = rand() % 255;
    params.armMode = rand() % 255;
    params.operatingMode = rand() % 255;
    params.continuousMeasuringMode = rand() % 255;
    params.continuousModeTimeoutSec = rand() % 255;
    params.logMode = rand() % 255;
    params.isOpen = true;
    params.isConnected = true;
    params.minGateDistance = (float)(rand() % 255) / 100.0f;
    params.maxGateDistance = (float)(rand() % 255) / 100.0f;
    params.temperatureDeg = (float)(rand() % 255) / 100.0f;
    params.custom1 = (float)(rand() % 255) / 100.0f;
    params.custom2 = (float)(rand() % 255) / 100.0f;
    params.custom3 = (float)(rand() % 255) / 100.0f;
}