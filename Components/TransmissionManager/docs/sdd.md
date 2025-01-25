# Components::TransmissionManager

Component that will call drivers to send transmissions and be called by tranceiver drivers to handle commands when receiving a transmission. 

NOTE: This component is currently constructed only as a proof of concept for the startup beacon and its transition to an initial beacon.

## Usage Examples

### Diagrams

``` Mermaid
graph LR
    TM[Transmission Manager] --sendData--> TD
    TD[Tranceiver Driver] --recvData--> TM
    TM --beaconState--> FL[Flight Logic]
    RG[Rate Group 4] -- beacon --> TM

```

### Typical Usage

- Receive transmission packet from the ground through tranceiver driver and translate into commands
- Collect telemetry data from system through flight logic, translate into packet, and send to drivers


<!-- ## Class Diagram -->


## Port Descriptions
| Name | Type | Description |
|---|---|---|
|sendData|Output|Collects data from ground, prepares it to be sent, then sends it to the tranceiver driver to be sent to ground. |
|recvData| Async Input | Receives data originating from the ground station from the tranceiver driver, translates the packet into readable commands, and then sends out those commands.
|beaconState|Output|Sends either INITIAL, STANDARD, or RETURN_STATE to Flight Logic then receives the current beacon state. If INITIAL or STANDARD are sent, Flight Logic will set the beacon to that state. Otherwise, Flight Logic makes no change to the state and just returns the current value.|
|beacon| Sync Input | Connects to a rate group running at 1/60hz that will call sendData to send out a beacon every 1 minute.|

## Component States
This component does not use any state machines

## Sequence Diagrams
This component does not use any state machines

## Parameters
This component does not have any Parameters

## Commands
| Name | Description |
|---|---|
|confirmConnection|Sent by the ground station to confirm beacon has been received successfully. This prompts the system to change beacon to the STANDARD state.|
| setBeacon | Can be used by the ground station to manually set the state of the beacon if something goes wrong. |
| sendTransToGround | This is a temporary command to test sending a transmission from the satellite to the ground station. |

## Events
| Name | Description |
|---|---|
| success | Used for testing. Signifies that the satellite has received data from the ground station. |
| sending | used for testing. Announces the satellite is attempting to send data to the grounds station. |
| beaconSet | Announces that the beacon state has been changed by a call from the Transmission Manager. |
| invalidBeaconState | Warns the beacon has somehow entered an invalid state and is automatically being changed to INITIAL. |

## Telemetry
This port does not send any telemetry to the ground. the beaconState telemetry is handled by Flight Logic.

## Unit Tests
Add unit test descriptions in the chart below
| Name | Description | Output | Coverage |
|---|---|---|---|
|---|---|---|---|

## Requirements
| Name | Description | Validation Method | Requirement Met |
|---|---|---|---|
|Post Antenna|Initial beacon transmits after Endurosat antenna deployment| GDS Manual Test | Yes |
| Initial Beacon | Initial beacon transmits exactly once each minute until acknowledged from the ground | GDS Manual Test | Yes |
| Initial Date & Time | Initial beacon transmission includes the time and date of satellite's clock |---|---|
| Initial Charge | Initial beacon transmission includes the battery charge |---|---|
| Initial Health | Initial beacon transmission includes the battery health |---|---|
| Initial Boot Time | Initial beacon transmission includes the first satellite boot time |---|---|
| Initial Deployment Time | Initial beacon transmission includes Endurosat antenna deployment time |---|---|
| Initial Scotsman | Initial beacon transmission includes the Scotsman |---|---|
| Initial End | Initial beacon transmission stops after receiving ping |---|---|
| Status Beacon | The status beacon is transmitted exactly once each minute |GDS Manual Test | Yes |
| Status Battery Charge| The status beacon includes battery percentage |---|---|
| Status Battery Health | The status beacon includes battery health if applicable |---|---|
| Status Code Base | The status beacon indicates running code base if applicable |---|---|
| Status Last Reboot | The status beacon includes time of last reboot |---|---|
| Status Reboot Count | The status beacon includes total number of system reboots |---|---|
| Ax25 | The satellite will transmit in AX25 format |---|---|
| Error Beacon | The error beacon is transmitted immediately following the status beacon (exactly once each minute) | GDS Manual Test | Yes |
| Error Component| The error beacon includes what component was running at the time of the error |---|---|
| Error Input | The error beacon includes the input to the component triggering the error |---|---|
| Error Time | The error beacon includes the exact time of the error |---|---|

#### Note:
At some point GDS Manual Tests will be run using integration tests.

## Change Log
| Date | Description |
|---|---|
|7/8/24| Initial Draft |