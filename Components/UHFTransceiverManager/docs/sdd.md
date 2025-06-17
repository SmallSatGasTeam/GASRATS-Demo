# Components::UHFTransceiverManager

component contains functions that configure the UHF transceiver's settings, and allows for data to be sent / received. "

## Usage Examples
Add usage examples here

### Diagrams
Add diagrams here

### Typical Usage
This component will be used to configure the UHF transceivers settings, some important ones are:
*   Radio Frequency Configuration
*   Beacon Transmission period
*   Low Power Mode
*   Turning ON/OFF pipe mode
*   Reading information like temperature, packets received sent, etc.

## Class Diagram
Add a class diagram here

## Port Descriptions
| Name | Description |
| i2cRead | Port for Reading from an I2C device. |
| i2cWrite | Port for writing to an I2C device. |
| pingOut | Returns a health ping |

## Component States
Add component states in the chart below
| Name | Description |
|---|---|
|---|---|

## Sequence Diagrams
Add sequence diagrams here

## Parameters
| Name | Description |
|---|---|
|---|---|

## Commands
| Name | Description |
| configureRFSettings | Goes through and configures each RF setting. This includes radio frequency, beacon transmission, and pipe mode.|
| transmitData | Command that will be used to send test data via UART. |

## Events
| Name | Description |
|---|---|
|---|---|

## Telemetry
| Name | Description |
| uptime | The amount of time in seconds since startup |
| packetsReceived | The amount of packets received|
| PacketsTransmitted | The amount of packets transmitted |
| temperature | The temperature of the transceiver in celsius |
| lowPowerMode | Power mode of transceiver. If true, the device is in low power mode. |
| response | Full response that was returned from a READ / WRITE command |
| responseStatus | Status of response. Either OK or ERR |

## Unit Tests
Add unit test descriptions in the chart below
| Name | Description | Output | Coverage |
|---|---|---|---|
|---|---|---|---|

## Requirements
Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |

## Stuff I learned
### I2C
When writing and reading with an I2C device you must be very careful of your buffer sizes. We determined that you can have a large read size that allows for different read sizes, but the write size must be the exact length of the command that is being sent, no more no less. 
### Pipe Mode
Pipe mode is a mode that you can turn on from I2C where every byte sent over UART gets immediately sent out over RF, without any additional formatting. *However*, once you activate pipe mode you **can only communicate via UART**.
### Fprime stuff
We dealt with a lot of data type conversions with this component. Here is what we learned:
*   Fw::String = char*
*   U8* data = static_cast<U8*>(readBuffer.getData()); // effective way to get data out of a buffer into an array of type U8.
*   Fw::String var = reinterpret_cast<char*>(readBuffer.getData()); // Convert contents of read buffer to char array which can be stored in an Fw::String variable