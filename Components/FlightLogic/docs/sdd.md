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
| 

## Telemetry
| Name | Description |
|---|---|
|---|---|

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