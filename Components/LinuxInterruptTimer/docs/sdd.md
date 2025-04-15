# Components::LinuxInterruptTimer

Creates a software timer using the linux operating system. The timer 

## Usage Examples
Add usage examples here

### Diagrams
Add diagrams here

### Typical Usage
And the typical usage of the component here

## Class Diagram
Add a class diagram here

## Port Descriptions
| Name | Description |
|---|---|
|---|---|

## Component States
Add component states in the chart below
| Name | Description |
|---|---|
|CycleOut| This port is called by the software clock interrupt handler to trigger the rate group drvier.|

## Sequence Diagrams
Add sequence diagrams here

## Parameters
| Name | Description |
|---|---|
|---|---|

## Commands
| Name | Description |
|---|---|
|---|---|

## Events
| Name | Description |
|---|---|
|TimerCreated| Runs during initalization after software timer has been started. Indicates that the interrupt is enabled and working.|
| TimerTick | A debug event used to send a message each time a tick happens.|
|Error| Denotes that some error has kept the timer from starting. |

## Telemetry
| Name | Description |
|---|---|
| ticks | Count of the number of interrupts received by the interrupt handler.|

## Requirements
Add requirements in the chart below
| Name | Description | Validation Method |
|---|---|---|
| Interrupt | The LinuxInterruptTimer shall create a software timer that can interrupt the rest of the flight software.| GDS or Integration Testing |
| Accuracy | The LinuxInterruptTimer shall not skew more than 120 ticks every 24 hours.| GDS |
|Rate Group Driver|The LinuxInterruptTimer shall drive the rate group driver|GDS or Integration Testing|

## Tests
Add unit test descriptions in the chart below
| Name | Description | Result | Date |
|---|---|---|---|
| Interrupt Requirement Test| Uncomment the debugging event (this->log_ACTIVITY_HI_TimerTick()). Run the GDS and verify that the TimeTick message is being broadcast at regular intervals.| Success | 4/10/25|
| Accuracy | Uncomment the debugging event. Run the system on a raspberry pi zero 2w for 24 hours. At the end use the timestamp from the timer created event to calculate the total number of ticks that should have elapsed. Compare this to the value of the tick telemetry and verify that $\|\text{ticks}_{E} - \text{ticks}_{A} \| \leq 120$.|||
| Rate Group Driver Test | Verify that the sofware runs and that components connected to the rate groups are being called. (This test can also be completed during integration testing) | Success | 4/10/25 |

## Change Log
| Date | Description |
|---|---|
|4/7/25| Initial Draft |