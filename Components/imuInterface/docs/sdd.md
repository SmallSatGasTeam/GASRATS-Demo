# Components::imuInterface

This component provides all needed i2c signals in order to run the i2c driver.

## Usage Examples
Used to interface with an L3GD20H gyroscope.

### Diagrams
Add diagrams here

### Typical Usage
Must be connected to the data collector and to fprime i2cDriver

## Class Diagram
![IMU Diagram](./IMU%20diagram.png)

## Port Descriptions
| Name | Description |
|---|---|
|allocate| Calls the buffer manager to allocate and return a buffer with a specific length.|
|deallocate| Sends an existing buffer to the buffer manager to deallocate it.| 
|gyroData| Sends the data pulled from the IMU to the data collector|
|i2cWrite|Calls the i2cDriver's write function.|
|requestI2cData|Calls the i2cDriver's read function.|

## Events
| Name | Description |
|---|---|
|imuSuccess|Notifies that the IMU successfully sent data.|
|imuAddressFailure| Warns of and invalid i2c address.|
|imuWriteError| Warns i2c write failed.|
|imuReadError| Warns i2c read failed.|
|imuOpenError| Warns i2c open failure.|
|imuOtherError| Warns something else went wrong with i2c.|
|MemoryAllocationFailed| Warns a buffer failed to be properly allocated|

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
|9/21/24| Initial Draft |