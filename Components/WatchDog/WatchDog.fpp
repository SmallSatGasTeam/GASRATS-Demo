module Components {
    @ Component to take watchdog heartbeat from health component and convert it into GPIO high or low.
    passive component WatchDog {

        ##############################################################################
        #### Events
        ##############################################################################

        @ Event for printing watchdog code received from health
        event print(code: U32) severity activity high format "Code from health component: {}"

        @ Event for notifying when an assert fails and the system restarts
        event assertFailed severity fatal format "Assert failed, restarting system"

        ##############################################################################
        #### ports
        ##############################################################################

        @ Accepts the code coming in from the health component
        sync input port healthIn: Svc.WatchDog

        @ Sends the heartbeat out to the actual watchdog
        output port heartBeatOut: Drv.GpioWrite

        ##############################################################################
        #### Telemetry
        ##############################################################################

        @ Tells if heartbeat is High or low
        telemetry heartBeat:Fw.Logic

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}