![lrf_web_logo](./static/lrf_web_logo.png)



# **Lrf interface C++ library**

**v1.0.1**



# Table of contents

- [Overview](#overview)
- [Versions](#versions)
- [Library files](#library-files)
- [Lrf interface class description](#lrf-interface-class-description)
  - [Lrf class declaration](#lrf-class-declaration)
  - [getVersion method](#getversion-method)
  - [openLrf method](#openlrf-method)
  - [initLrf method](#initlrf-method)
  - [closeLrf method](#closelrf-method)
  - [isLrfOpen method](#islrfopen-method)
  - [isLrfConnected method](#islrfconnected-method)
  - [setParam method](#setparam-method)
  - [getParam method](#getparam-method)
  - [getParams method](#getparams-method)
  - [executeCommand method](#executecommand-method)
  - [encodeSetParamCommand method](#encodesetparamcommand-method)
  - [encodeCommand method](#encodecommand-method)
  - [decodeCommand method](#decodecommand-method)
  - [decodeAndExecuteCommand method](#decodeandexecutecommand-method)
- [Data structures](#data-structures)
  - [LrfCommand enum](#lrfcommand-enum)
  - [LrfParam enum](#lrfparam-enum)
- [LrfParams class description](#lrfparams-class-description)
  - [LrfParams class declaration](#lrfparams-class-declaration)
  - [Serialize LRF params](#serialize-lrf-params)
  - [Deserialize LRF params](#deserialize-lrf-params)
  - [Read params from JSON file and write to JSON file](#read-params-from-json-file-and-write-to-json-file)
- [Build and connect to your project](#build-and-connect-to-your-project)
- [How to make custom implementation](#how-to-make-custom-implementation)



# Overview

**Lrf** C++ library provides standard interface as well defines data structures and rules for different laser range finder (LRF) software controllers. **Lrf** interface class does nothing, just provides interface and provides methods to encode / decode commands and encode / decode params. Different LRF controllers inherit interface form **Lrf** C++ class. **Lrf.h** file contains list of data structures ([LrfCommand](#lrfcommand-enum) enum, [LrfParam](#lrfparam-enum) enum and [LrfParams](#lrfparams-class-description) class) and [Lrf](#lrf-interface-class-description) class declaration. [LrfCommand](#lrfcommand-enum) enum contains IDs of action commands supported by **Lrf** class. [LrfParam](#lrfparam-enum) enum contains IDs of parameters supported by **Lrf** class. [LrfParams](#lrfparam-enum) class contains fields for LRF parameters values and provides methods to encode/decode and read/write LRF parameters from JSON file. All LRF controllers should include params and commands listed in **Lrf.h** file. **Lrf** interface class depends only on [ConfigReader](https://rapidpixel.constantrobotics.com/docs/service-libraries/config-reader.html) library (provides methods to read / write JSON config files, source code included, Apache 2.0 license). It uses C++17 standard. The library is licensed under the **Apache 2.0** license.



# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                          |
| ------- | ------------ | --------------------------------------------------- |
| 1.0.0   | 20.06.2024   | First version                                       |
| 1.0.1   | 30.07.2024   | - CMake updated.                                    |



# Library files

The library supplied by source code only. The user would be given a set of files in the form of a CMake project (repository). The repository structure is shown below:

```xml
CMakeLists.txt ------------- Main CMake file of the library.
3rdparty ------------------- Folder with third-party libraries.
    CMakeLists.txt --------- CMake file to include third-party libraries.
    ConfigReader ----------- Folder with ConfigReader library source code.
example -------------------- Folder with an example LRF controller.
    CMakeLists.txt --------- CMake file for example of LRF controller.
    CustomLrf.cpp ---------- C++ implementation file.
    CustomLrf.h ------------ Header with class declaration.
    CustomLrfVersion.h ----- Header file which includes class version.
    CustomLrfVersion.h.in -- CMake service file to generate version header.
test ----------------------- Folder with test application.
    CMakeLists.txt --------- CMake file for test application.
    main.cpp --------------- Source code file of test application.
src ------------------------ Folder with source code of the library.
    CMakeLists.txt --------- CMake file of the library.
    Lrf.cpp ---------------- C++ implementation file.
    Lrf.h ------------------ Header file which includes Lrf class declaration.
    LrfVersion.h ----------- Header file which includes version of the library.
    LrfVersion.h.in -------- CMake service file to generate version header.
```



# Lrf interface class description



## Lrf class declaration

**Lrf** interface class declared in **Lrf.h** file. Class declaration:

```cpp
namespace cr
{
namespace lrf
{
class Lrf
{
public:

    /// Class destructor.
    virtual ~Lrf();

    /// Get class version.
    static std::string getVersion();

    /// Open lrf controller.
    virtual bool openLrf(std::string initString) = 0;

    /// Init lrf controller by set of parameters.
    virtual bool initLrf(LrfParams& params) = 0;

    /// Close connection.
    virtual void closeLrf() = 0;

    /// Get lrf open status.
    virtual bool isLrfOpen() = 0;

    /// Get lrf connection status.
    virtual bool isLrfConnected() = 0;

    /// Set the lrf controller param.
    virtual bool setParam(LrfParam id, float value) = 0;

    /// Get the lrf controller param.
    virtual float getParam(LrfParam id) = 0;

    /// Get the lrf controller params.
    virtual void getParams(LrfParams& params) = 0;

    /// Execute lrf command.
    virtual bool executeCommand(LrfCommand id) = 0;

    /// Encode set param command.
    static void encodeSetParamCommand(
        uint8_t* data, int& size, LrfParam id, float value);

    /// Encode command.
    static void encodeCommand(uint8_t* data, int& size, LrfCommand id);

    /// Decode command.
    static int decodeCommand(
        uint8_t* data, int size, LrfParam& paramId,
        LrfCommand& commandId, float& value);

    ///  Decode and execute command.
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;
};
}
}
```



## getVersion method

The **getVersion()** method returns string of current class version. Particular LRF controller can have it's own **getVersion()** method. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **Lrf** class instance:

```cpp
std::cout << "Lrf class version: " << Lrf::getVersion() << std::endl;
```

Console output:

```bash
Lrf class version: 1.0.0
```



## openLrf method

The **openLrf(...)** method initializes LRF controller. This method can be used instead of [initLrf(...)](#initlrf-method) method. LRF controller should establish connection to LRF hardware and should set default parameters. Method declaration:

```cpp
virtual bool openLrf(std::string initString) = 0;
```

| Parameter  | Value                                                        |
| ---------- | ------------------------------------------------------------ |
| initString | Initialization string. LRF controller can have unique init string format. But it is recommended to use '**;**' symbol to divide parts of initialization string. Recommended initialization string format for controllers which uses serial port: **"/dev/ttyUSB0;9600;100"** ("/dev/ttyUSB0" - serial port name, "9600" - baudrate, "100" - serial port read timeout). |

**Returns:** TRUE if the LRF controller initialized or FALSE if not.



## initLrf method

The **initLrf(...)** method initializes LRF controller by set of parameters. This method can be used instead of [openLrf(...)](#openlrf-method) method ([LrfParams](#lrfparams-class-description) class includes **initString**) when you need initialize LRF controller with not default parameters. LRF controller should establish connection to LRF hardware and should set necessary parameters according to given [LrfParams](#lrfparams-class-description). Method declaration:

```cpp
virtual bool initLrf(LrfParams& params) = 0;
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| params    | [LrfParams](#lrfparams-class-description) class object. [LrfParams](#lrfparams-class-description) class includes initString which used in [openLrf(...)](#openlrf-method) method. |

**Returns:** TRUE if the LRF controller initialized or FALSE if not.



## closeLrf method

The **closeLrf()** method closes connection to LRF. LRF controller should close connection to LRF hardware and should set connection and open flags to FALSE (see [LrfParams](#lrfparams-class-description) class description). Method declaration:

```cpp
virtual void closeLrf() = 0;
```



## isLrfOpen method

The **isLrfOpen()** method returns LRF initialization status. Open status shows if the LRF controller initialized but doesn't show if LRF controller has communication with LRF equipment. For example, if LRF has serial port LRF controller connects to serial port (opens serial port file in OS) but LRF can be not active (no power). In this case open status just shows that LRF controller has opened serial port. Method declaration:

```cpp
virtual bool isLrfOpen() = 0;
```

**Returns:** TRUE is the LRF controller is initialized or FALSE if not.



## isLrfConnected method

The **isLrfConnected()** method returns LRF connection status. Connection status shows if the LRF controller has data exchange with LRF equipment. For example, if LRF has serial port LRF controller connects to serial port (opens serial port file in OS) but LRF can be not active (no power). In this case connection status shows that LRF controller doesn't have data exchange with LRF equipment (method will return FALSE). If LRF controller has data exchange with LRF equipment the method will return TRUE. If LRF controller not initialize the connection status always FALSE. Method declaration:

```cpp
virtual bool isLrfConnected() = 0;
```

**Returns:** TRUE if the LRF controller has data exchange with LRF equipment or FALSE if not.



## setParam method

The **setParam(...)** method sets new LRF parameters value. LRF controller must provide thread-safe **setParam(...)** method call. This means that the **setParam(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool setParam(LrfParam id, float value) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | LRF parameter ID according to [LrfParam](#lrfparam-enum) enum. |
| value     | LRF parameter value. Value depends on parameter ID (see description of [LrfParam](#lrfparam-enum) enum). |

**Returns:** TRUE if the parameter was set or FALSE if not.



## getParam method

The **getParam(...)** method returns LRF parameter value. LRF controller must provide thread-safe **getParam(...)** method call. This means that the **getParam(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual float getParam(LrfParam id) = 0;
```

| Parameter | Description                                                |
| --------- | ---------------------------------------------------------- |
| id        | LRF parameter ID according to [LrfParam](#lrfparam-enum) enum. |

**Returns:** parameter value or -1 of the parameters doesn't exist (not supported) in LRF controller.



## getParams method

The **getParams(...)** method designed to obtain all LRF parameters. LRF controller must provide thread-safe **getParams(...)** method call. This means that the **getParams(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual void getParams(LrfParams& params) = 0;
```

| Parameter | Description                                                |
| --------- | ---------------------------------------------------------- |
| params    | Reference to [LrfParams](#lrfparams-class-description) object to store params. |



## executeCommand method

The **executeCommand(...)** method designed to execute LRF action command. LRF controller must provide thread-safe **executeCommand(...)** method call. This means that the **executeCommand(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool executeCommand(LrfCommand id) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | LRF command ID according to [LrfCommand](#lrfcommand-enum) enum. |

**Returns:** TRUE is the command was executed (accepted by LRF controller) or FALSE if not.



## encodeSetParamCommand method

The **encodeSetParamCommand(...)** static method designed to encode command to change any remote LRF parameter. To control a LRF remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **Lrf** class contains static methods for encoding the control command. The **Lrf** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). **encodeSetParamCommand(...)** designed to encode SET_PARAM command. Method declaration:

```cpp
static void encodeSetParamCommand(uint8_t* data, int& size, LrfParam id, float value);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer for encoded command. Must have size >= 11. |
| size      | Size of encoded data. Will be 11 bytes.                      |
| id        | Parameter ID according to [LrfParam](#lrfparam-enum) enum.   |
| value     | Parameter value.                                             |

**encodeSetParamCommand(...)** is static and used without **Lrf** class instance. This method used on client side (control system). Example:

```cpp
// Buffer for encoded data.
uint8_t data[11];
// Size of encoded data.
int size = 0;
// Encode command.
Lrf::encodeSetParamCommand(data, size, LrfParam::OPERATING_MODE, 1);
```



## encodeCommand method

The **encodeCommand(...)** static method designed to encode LRF action command (COMMAND). To control a LRF remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **Lrf** class contains static methods for encoding the control command. The **Lrf** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). **encodeCommand(...)** designed to encode COMMAND (action command). Method declaration:

```cpp
static void encodeCommand(uint8_t* data, int& size, LrfCommand id);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer for encoded command. Must have size >= 7. |
| size      | Size of encoded data. Will be 7 bytes.                      |
| id        | Command ID according to [LrfCommand](#lrfcommand-enum) enum. |

**encodeCommand(...)** is static and used without **Lrf** class instance. This method used on client side (control system). Encoding example:

```cpp
// Buffer for encoded data.
uint8_t data[7];
// Size of encoded data.
int size = 0;
// Encode command.
Lrf::encodeCommand(data, size, LrfCommand::DISARM);
```



## decodeCommand method

The **decodeCommand(...)** static method designed to decode command on LRF controller side. To control a LRF remotely, the developer has to develop his own protocol and according to it decode the command on LRF controller side. To simplify this, the **Lrf** interface class contains static method to decode input command (commands should be encoded by methods [encodeSetParamCommand(...)](#encodesetparamcommand-method) or [encodeCommand(...)](#encodecommand-method) methods). The **Lrf** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). Method declaration:

```cpp
static int decodeCommand(uint8_t* data, int size, LrfParam& paramId, LrfCommand& commandId, float& value);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to input command.                                    |
| size      | Size of command. Should be 11 bytes for SET_PARAM and 7 bytes for COMMAND. |
| paramId   | LRF parameter ID according to [LrfParam](#lrfparam-enum) enum. After decoding SET_PARAM command the method will return parameter ID. |
| commandId | LRF command ID according to [LrfCommand](#lrfcommand-enum) enum. After decoding COMMAND the method will return command ID. |
| value     | LRF parameter value after decoding SET_PARAM command. |

**Returns:** **0** - in case decoding COMMAND (action command), **1** - in case decoding SET_PARAM command or **-1** in case errors.



## decodeAndExecuteCommand method

The **decodeAndExecuteCommand(...)** method decodes and executes command on LRF controller side encoded by [encodeSetParamCommand(...)](#encodesetparamcommand-method) or [encodeCommand(...)](#encodecommand-method) methods. LRF controller must provide thread-safe **decodeAndExecuteCommand(...)** method call. This means that the **decodeAndExecuteCommand(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to input command.                                    |
| size      | Size of command. Must be 11 bytes for SET_PARAM and 7 bytes for COMMAND (action command). |

**Returns:** TRUE if command decoded (SET_PARAM or COMMAND) and executed (action command or set param command).



# Data structures



## LrfCommand enum

**LrfCommand** enum describes action LRF command. All commands must be supported by LRF controller. Enum declaration:

```cpp
namespace cr
{
namespace lrf
{
enum class LrfCommand
{
    /// Arm.
    ARM = 1,
    /// Disarm.
    DISARM,
    /// Measure distance once.
    MEASURE_DISTANCE_ONCE
};
}
}
```

**Table 2** - LRF action commands description. Some commands may be unsupported by LRF controller.

| Command               | Description                                          |
| --------------------- | ---------------------------------------------------- |
| ARM                   | Command to arm LRF.                                  |
| DISARM                | Command to disarm LRF.                               |
| MEASURE_DISTANCE_ONCE | Measure distance once.                               |



## LrfParam enum

**LrfParam** enum describes LRF parameters. Enum declaration:

```cpp
namespace cr
{
namespace lrf
{
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
}
}
```

**Table 3** - LRF params description. Some params may be unsupported by LRF controller.

| Parameter                     | Access       | Description                                                  |
| ----------------------------- | ------------ | ------------------------------------------------------------ |
| DISTANCE                      | read only    | Current measured distance, meters. LRF controller should keep this value until distance update. |
| TIME_FROM_LAST_MEASUREMENT_US | read only    | Time from last measurement, microseconds. Used to control how old last measurement. |
| LOW_POWER_MODE                | read / write | Low power mode. Values depends on LRF controller. Default: **0** - OFF. |
| POINTER_MODE                  | read / write | Laser pointer mode: **0** - OFF, **1** - ON, **2** - ON timeout (turn off automatically after timeout). |
| POINTER_MODE_TIMEOUT_SEC      | read / write | Pointer mode timeout, sec.                                   |
| ARM_MODE                      | read / write | Arm mode: **0** - Disarmed, **1** - armed.                   |
| OPERATING_MODE                | read / write | Operating mode. Default values: **0** - OFF, **1** - Stand-by-mode, **2** - Normal mode. |
| CONTINUOUS_MEASURING_MODE     | read / write | Continuous measuring mode. Default values: **0** - Stop, **1** - 0.5Hz, **2** - 1Hz, **3** - 3Hz, **4** - 5Hz, **5** - 10Hz. |
| CONTINUOUS_MODE_TIMEOUT_SEC   | read / write | Continuous mode timeout, sec. Turn off measurements after timeout. If **0** - no timeout. |
| LOG_MODE                      | read / write | Logging mode: **0** - Disabled, **1** - Console, **2** - File, **3** - Console and file. |
| IS_OPEN                       | read only    | Open status: **0** - not init (not open), **1** - open (init). Open status shows if the LRF controller initialized but doesn't show if LRF controller has communication with LRF equipment. For example, if LRF has serial port LRF controller connects to serial port (opens serial port file in OS) but LRF can be not active (no power). In this case open status just shows that LRF controller has opened serial port. |
| IS_CONNECTED                  | read only    | Connection status: **0** - no responses from LRF hardware, **1** - connected. Connection status shows if the LRF controller has data exchange with LRF equipment. For example, if LRF has serial port LRF controller connects to serial port (opens serial port file in OS) but LRF can be not active (no power). In this case connection status shows that LRF controller doesn't have data exchange with LRF equipment. If LRF controller has data exchange with LRF equipment status will be **1**. If LRF controller not initialize the connection status always **0**. |
| MIN_GATE_DISTANCE             | read / write | Min gate distance, meters. If parameter **> 0** the LRF controller must ignore measured distance **< MIN_GATE_DISTANCE**. |
| MAX_GATE_DISTANCE             | read / write | Max gate distance, meters. If parameter **> 0** the LRF controller must ignore measured distance **> MAX_GATE_DISTANCE**. |
| TEMPERATURE_DEG               | read only    | LRF temperature, degree.                                     |
| CUSTOM_1                      | read / write | LRF custom parameter. Value depends on LRF controller. Custom parameters used when LRF equipment has specific unusual parameter. |
| CUSTOM_2                      | read / write | LRF custom parameter. Value depends on LRF controller. Custom parameters used when LRF equipment has specific unusual parameter. |
| CUSTOM_3                      | read / write | LRF custom parameter. Value depends on LRF controller. Custom parameters used when LRF equipment has specific unusual parameter. |



# LrfParams class description

**LrfParams** class used for LRF controller initialization ([initLrf(...)](#initlrf-method) method) or to get all actual params ([getParams(...)](#getparams-method) method). Also **LrfParams** provides structure to write/read params from JSON files (**JSON_READABLE** macro) and provides methods to encode and decode params.



## LrfParams class declaration

**LrfParams** interface class declared in **Lrf.h** file. Class declaration:

```cpp
namespace cr
{
namespace lrf
{
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

    /// operator =
    LrfParams& operator= (const LrfParams& src);

    /// Encode params.
    bool encode(uint8_t* data, int bufferSize, int& size,
                LrfParamsMask* mask = nullptr);

    /// Decode params.
    bool decode(uint8_t* data, int dataSize);
};
}
}
```

**Table 4** - LrfParams class fields description (equivalent to [LrfParam](#lrfparam-enum) enum description).

| Field                     | type   | Description                                                  |
| ------------------------- | ------ | ------------------------------------------------------------ |
| initString                | string | Initialization string. LRF controller can have unique init string format. But it is recommended to use '**;**' symbol to divide parts of initialization string. Recommended initialization string format for controllers which uses serial port: **"/dev/ttyUSB0;9600;100"** ("/dev/ttyUSB0" - serial port name, "9600" - baudrate, "100" - serial port read timeout). The same initialization string is used in [openLrf(...)](#openlrf-method) method. |
| distance                  | float  | Current measured distance, meters. LRF controller should keep this value until distance update. |
| timeFromLastMeasurementUs | int    | Time from last measurement, microseconds. Used to control how old last measurement. |
| lowPowerMode              | int    | Low power mode. Values depends on LRF controller. Default: **0** - OFF. |
| pointerMode               | int    | Laser pointer mode: **0** - OFF, **1** - ON, **2** - ON timeout (turn off automatically after timeout). |
| pointerModeTimeoutSec     | int    | Pointer mode timeout, sec.                                   |
| armMode                   | int    | Arm mode: **0** - Disarmed, **1** - armed.                   |
| operatingMode             | int    | Operating mode. Default values: **0** - OFF, **1** - Stand-by-mode, **2** - Normal mode. |
| continuousMeasuringMode   | int    | Continuous measuring mode. Default values: **0** - Stop, **1** - 0.5Hz, **2** - 1Hz, **3** - 3Hz, **4** - 5Hz, **5** - 10Hz. |
| continuousModeTimeoutSec  | int    | Continuous mode timeout, sec. Turn off measurements after timeout. If **0** - no timeout. |
| logMode                   | int    | Logging mode: **0** - Disabled, **1** - Console, **2** - File, **3** - Console and file. |
| isOpen                    | bool   | Open status: **FALSE** - not init (not open), **TRUE** - open (init). Open status shows if the LRF controller initialized but doesn't show if LRF controller has communication with LRF equipment. For example, if LRF has serial port LRF controller connects to serial port (opens serial port file in OS) but LRF can be not active (no power). In this case open status just shows that LRF controller has opened serial port. |
| isConnected               | bool   | Connection status: **FALSE** - no responses from LRF hardware, **TRUE** - connected. Connection status shows if the LRF controller has data exchange with LRF equipment. For example, if LRF has serial port LRF controller connects to serial port (opens serial port file in OS) but LRF can be not active (no power). In this case connection status shows that LRF controller doesn't have data exchange with LRF equipment. If LRF controller has data exchange with LRF equipment status will be **TRUE**. If LRF controller not initialize the connection status always **FALSE**. |
| minGateDistance           | float  | Min gate distance, meters. If parameter **> 0** the LRF controller must ignore measured distance **< minGateDistance**. |
| maxGateDistance           | float  | Max gate distance, meters. If parameter **> 0** the LRF controller must ignore measured distance **> maxGateDistance**. |
| temperatureDeg            | float  | LRF temperature, degree.                                     |
| custom1                   | float  | LRF custom parameter. Value depends on LRF controller. Custom parameters used when LRF equipment has specific unusual parameter. |
| custom2                   | float  | LRF custom parameter. Value depends on LRF controller. Custom parameters used when LRF equipment has specific unusual parameter. |
| custom3                   | float  | LRF custom parameter. Value depends on LRF controller. Custom parameters used when LRF equipment has specific unusual parameter. |

**None:** *LrfParams class fields listed in Table 4 **must** reflect params set/get by methods setParam(...) and getParam(...).*



## Serialize LRF params

[LrfParams](#lrfparams-class-description) class provides method **encode(...)** to serialize LRF params (fields of [LrfParams](#lrfparams-class-description) class, see Table 4). Serialization of LRF params necessary in case when you need to send LRF params via communication channels. Method doesn't encode **initString** string field. Method provides options to exclude particular parameters from serialization. To do this method inserts binary mask (3 bytes) where each bit represents particular parameter and **decode(...)** method recognizes it. Method declaration:

```cpp
bool encode(uint8_t* data, int bufferSize, int& size, LrfParamsMask* mask = nullptr);
```

| Parameter  | Value                                                        |
| ---------- | ------------------------------------------------------------ |
| data       | Pointer to data buffer.                                      |
| size       | Size of encoded data.                                        |
| bufferSize | Data buffer size. Buffer size must be >= 72 bytes.           |
| mask       | Parameters mask - pointer to **LrfParamsMask** structure. **LrfParamsMask** (declared in **Lrf.h** file) determines flags for each field (parameter) declared in [LrfParams](#lrfparams-class-description) class. If the user wants to exclude any parameters from serialization, he can put a pointer to the mask. If the user wants to exclude a particular parameter from serialization, he should set the corresponding flag in the **LrfParamsMask** structure. |

**LrfParamsMask** structure declaration:

```cpp
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
```

Example without parameters mask:

```cpp
// Encode data.
LrfParams in;
in.logMode = 3;
uint8_t data[1024];
int size = 0;
in.encode(data, 1024, size);
cout << "Encoded data size: " << size << " bytes" << endl;
```

Example with parameters mask:

```cpp
// Prepare params.
LrfParams in;
in.logMode = 3;

// Prepare mask.
LrfParamsMask mask;
mask.logMode = false; // Exclude logMode. Others by default.

// Encode.
uint8_t data[1024];
int size = 0;
in.encode(data, 1024, size, &mask);
cout << "Encoded data size: " << size << " bytes" << endl;
```



## Deserialize LRF params

[LrfParams](#lrfparams-class-description) class provides method **decode(...)** to deserialize LRF params (fields of [LrfParams](#lrfparams-class-description) class, see Table 4). Deserialization of LRF params necessary in case when you need to receive LRF params via communication channels. Method automatically recognizes which parameters were serialized by **encode(...)** method. Method doesn't decode fields: **initString**. Method declaration:

```cpp
bool decode(uint8_t* data, int dataSize);
```

| Parameter | Value                   |
| --------- | ----------------------- |
| data      | Pointer to data buffer. |
| dataSize  | Size of data.           |

**Returns:** TRUE if data decoded (deserialized) or FALSE if not.

Example:

```cpp
// Encode data.
LrfParams in;
uint8_t data[1024];
int size = 0;
in.encode(data, 1024, size);
cout << "Encoded data size: " << size << " bytes" << endl;

// Decode data.
LrfParams out;
if (!out.decode(data, size))
    cout << "Can't decode data" << endl;
```



## Read params from JSON file and write to JSON file

**Lrf** interface class library depends on [ConfigReader](https://rapidpixel.constantrobotics.com/docs/service-libraries/config-reader.html) library which provides method to read params from JSON file and to write params to JSON file. Example of writing and reading params to JSON file:

```cpp
// Write default params to file.
LrfParams in;
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
// Parse params.
LrfParams out;
if(!outConfig.get(out, "LrfParams"))
{
    cout << "Can't read params from file" << endl;
    return false;
}
```

**TestLrfParams.json** will look like:

```json
{
    "LrfParams": {
        "continuousModeTimeoutSec": 35,
        "custom1": 2.490000009536743,
        "custom2": 1.5,
        "custom3": 2.5199999809265137,
        "initString": "17108",
        "logMode": 61,
        "lowPowerMode": 226,
        "maxGateDistance": 2.2300000190734863,
        "minGateDistance": 0.38999998569488525,
        "operatingMode": 207,
        "pointerModeTimeoutSec": 180
    }
}
```



# Build and connect to your project

Typical commands to build **Lrf** library:

```bash
cd Lrf
mkdir build
cd build
cmake ..
make
```

If you want connect **Lrf** library to your CMake project as source code you can make follow. For example, if your repository has structure:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
```

Create folder **3rdparty** in your repository folder and copy **Lrf** repository folder there. New structure of your repository:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
3rdparty
    Lrf
```

Create CMakeLists.txt file in **3rdparty** folder. CMakeLists.txt should contain:

```cmake
cmake_minimum_required(VERSION 3.13)

################################################################################
## 3RD-PARTY
## dependencies for the project
################################################################################
project(3rdparty LANGUAGES CXX)

################################################################################
## SETTINGS
## basic 3rd-party settings before use
################################################################################
# To inherit the top-level architecture when the project is used as a submodule.
SET(PARENT ${PARENT}_YOUR_PROJECT_3RDPARTY)
# Disable self-overwriting of parameters inside included subdirectories.
SET(${PARENT}_SUBMODULE_CACHE_OVERWRITE OFF CACHE BOOL "" FORCE)

################################################################################
## CONFIGURATION
## 3rd-party submodules configuration
################################################################################
SET(${PARENT}_SUBMODULE_LRF                             ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_LRF)
    SET(${PARENT}_LRF                                   ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_LRF_TEST                              OFF CACHE BOOL "" FORCE)
    SET(${PARENT}_LRF_EXAMPLE                           OFF CACHE BOOL "" FORCE)
endif()

################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the 3rd-party configuration
################################################################################
if (${PARENT}_SUBMODULE_LRF)
    add_subdirectory(Lrf)
endif()
```

File **3rdparty/CMakeLists.txt** adds folder **Lrf** to your project and excludes test application and example (Lrf class test applications and example of custom LRF class implementation) from compiling (by default test application and example are excluded from compiling if **Lrf** included as sub-repository). Your repository new structure will be:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
3rdparty
    CMakeLists.txt
    Lrf
```

Next you need include folder 3rdparty in main **CMakeLists.txt** file of your repository. Add string at the end of your main **CMakeLists.txt**:

```cmake
add_subdirectory(3rdparty)
```

Next you have to include **Lrf** library in your **src/CMakeLists.txt** file:

```cmake
target_link_libraries(${PROJECT_NAME} Lrf)
```

Done!



# How to make custom implementation

The **Lrf** class provides only an interface, data structures, and methods for encoding and decoding commands and params. To create your own implementation of the LRF controller, you must include the **Lrf** repository in your project (see [Build and connect to your project](#build-and-connect-to-your-project) section). The catalogue **example** (see [Library files](#library-files) section) includes an example of the design of the custom LRF controller. You must implement all the methods of the **Lrf** interface class. Custom LRF class declaration:

```cpp
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

    /// Class constructor.
    CustomLrf();

    /// Class destructor.
    ~CustomLrf();

    /// Get class version.
    static std::string getVersion();

    /// Open lrf controller.
    bool openLrf(std::string initString) override;

    /// Init lrf controller by structure.
    bool initLrf(LrfParams& params) override;

    /// Close connection.
    void closeLrf() override;

    /// Get lrf open status.
    bool isLrfOpen() override;

    /// Get lrf connection status.
    bool isLrfConnected() override;

    /// Set the lrf controller param.
    bool setParam(LrfParam id, float value) override;

    /// Get the lrf controller param.
    float getParam(LrfParam id) override;

    /// Get the lrf controller params.
    void getParams(LrfParams& params) override;

    /// Execute lrf command.
    bool executeCommand(LrfCommand id) override;

    /// Decode and execute command.
    bool decodeAndExecuteCommand(uint8_t* data, int size) override;

private:

    // Parameters class.
    LrfParams m_params;
};
}
}
```