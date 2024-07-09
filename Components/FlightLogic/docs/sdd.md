# Components::FlightLogic

The application layer of the FSW. Handles system wide level flags such as deployment statuses as well as system state. This component also handles startup of the system.

## Usage Examples

### Diagrams

``` Mermaid
graph LR
    FL[Flight Logic] --epsHealth--> EPS[EPS Manager]
    FL --deployAntenna--> AD[Antenna Deploy]
    FL --fakeData--> DC[Data Collector]
    FL --takePic--> CM[Camera Manager]
    FL --deployCamera--> CM

    DC --dataRequest--> FL
    TM[Transmission Manager] --beaconState--> FL
```

### Typical Usage
This component is specifically designed as the application manager and state handler for the GASRATS cubesat and cannot be easily adapted for other purposes.


## Port Descriptions
| Name | Type | Description |
|---|---|---|
|deployAntenna|Output| Prompts Antenna Deploy to deploy the antenna. Antenna Deploy then returns a boolean based on the success of the deployment. |
| deployCamera | Output | Prompts Camera Manager to deploy the camera. Camera Manager then returns a boolean based on the success of the deployment. |
| epsHealth | Output | Gets the voltage and current of the EPS. |
| fakeData | Output | Used only for testing. Sends fake data to the data collector |
| pingOut | Output | Returns health ping from the health component |
| takePic | Output | Prompts the Camera Manager to take a picture |
| beaconState | Sync Input | If prompted, sets the beacon to a specific state, always returns current beaconState. |
| dataRequest | Sync Input | Allows the data collector to ask for data from the Flight Logic. This is most likely only for testing and will be removed in later versions. |
| pingIn | Async Input | Receives pings from the health component. |
| Startup | sync Input | Runs on the 1hz rate group, handles startup operations as well as routine health checks. |

## Component States
This component does not implement a state machine.

## Sequence Diagrams
This component does not implement a state machine.


## Parameters
This component does not currently use parameters

## Commands
| Name | Description |
|---|---|
| takePic | Prompts the Camera Handler to take another picutre. |
| resetFlags | Resets all system flags to their bootup conditions. |

## Events
| Name | Description |
|---|---|
| deployFailure | Warns that either the camera or the antenna failed to deploy. |
| runningStartup | Used in debugging to ensure that the startup port is being called. |
| detumble | Notifies the ground station that the OBC has the ADCS micro controller to detumble.
| rebooting | Notifies the ground station that the OBC is rebooting. |

## Telemetry
| Name | Description |
|---|---|
| antennaState | Whether or not the antenna is deployed. Uses an enum of DEPLOYED and UNDEPLOYED. |
| beaconState | The current state of the beacon. Uses an enum containg RETURN_STATE, INITIAL, STANDARD, ERROR, and OFF. |
| cameraState | Whether or not the camera is deployed. Uses an enum of DEPLOYED and UNDEPLOYED. |
| detumbled | A boolean flag representing whether or not the OBC believes the satellite has been detumbled. |
| lowPower | A boolean value representing whether or not the satellite is in low power mode. |

## Unit Tests

| Name | Description | Output |
|---|---|---|
|testTakePic| Tests that the takePic command causes Flight Logic to prompt Camera Manager to take a new photo. | OK |
| testResetFlags | Tests that the resetFlags command will successfully reset the flags to their pre-deploy values after startup has changed them. | OK |
| testStartup | Tests the startup port. Ensures the Flight Logic waits 30 minutes before prompting deployment, that it immediately deploys the antenna, starts the initial beacon, deploys the camera, and takes a picture. | OK |
| testLowPower | Tests the startup will throw the low power flag if the eps measurements ever drop below a certain value. | OK |
| testAntennaFailure | Tests that the Flight Logic will throw a fatal error and then reboot the system if the antenna fails to deploy | OK |
| testCameraFailure | Tests that the Flight Logic will throw a fatal error and then reboot the system if the antenna fails to deploy | OK |
| testHealthPing | Tests that Flight Logic is pinging the Health Component | OK |
| testDataRequest | Tests the data request port | OK |
| testSendBeaconState | Tests that the beaconState port returns the correct values | OK |
| testBeacon | Tests the beacon moves from OFF, to INITIAL, to STANDARD, as expected. | OK |

### Coverage

|| Exec | Total | Coverage |
|---|---|---|---|
|Lines|87|89|97.8%|
|Functions|8|8|100%|
|Branches|70|112|62.5%|

## Requirements
Add requirements in the chart below
| Name | Description | Validation Method | Requirement Met |
|---|---|---|---|
| Antenna Deployment | The Flight Logic must prompt the Antenna Deploy to activate the burn wire and deploy the antenna. | Unit Test | Yes |
| Antenna First | The Endurosat antenna deployment is the first startup operation. | GDS Manual Test | Yes |
| Check Charge | The Flight Logic will verify sufficient battery charge before deploying the antenna, deploying the camera, or starting the initial beacon. | GDS Manual Test | Yes |
| Deployment Delay | The Flight Logic must wait 30 minutes after booting up before deploying the antenna, deploying the camera, or starting the initial beacon. | Unit Test | Yes |
| Initial Beacon | After successfully deploying the antenna, the Flight Logic must start the initial beacon. | Unit Test | Yes |
| Payload Photo | The first payload operation performed must be taking a picture of the payload. | Unit Test | Yes |
| Photo After Beacon | The payload photo must be taken after the first initial beacon transmission | Unit Test | Yes |
| Beacon Stops | After receiving confirmation of connection from the ground, the initial beacon should switch to a status beacon. | GDS Manual Test | Yes |
| Error Beacon | If an error occurs the satellite will send an error beacon | --- | --- |
| Disable Checks | Boot software and hardware checks can be disabled from the ground | --- | --- |
| Unit Tests | Boot software checks include unit tests on all components | --- | --- |
| Integration Tests | Boot software checks include integration tests | --- | --- |
| Hardware Checks | Flight Logic will prompt hardware checks on each sensor during bootup | --- | --- |
| Check Flags | On startup Flight Logic will check all system flags to understand last system state. | --- | --- |
| Low Power | The Flight Logic must be able to notify the system to enter low power mode if the battery drops below a certain point. | --- | --- |



## Change Log
| Date | Description |
|---|---|
|7/8/24| Initial Draft |