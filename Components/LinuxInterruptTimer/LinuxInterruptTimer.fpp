module Components {
    @ Creates a timer that causes an interrupt. Urate groups.
    active component LinuxInterruptTimer {

        ###### Commands

        ###### Events
        event TimerCreated severity activity high format "The timer was created"
        event TimerTick severity activity high format "Timer tick"
        event Error severity warning high format "An error has occured"

        ###### Output ports
        ## This port shall send timer ticks, not sure if it is over I2c though.
        output port CycleOut: Svc.Cycle

        ###### Telemetry

        telemetry ticks: U32 update on change ### Must be U32 or bigger

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

        @ pingIn : receives health pings
        async input port pingIn: Svc.Ping

        @ pingOut : Returns health ping
        output port pingOut: Svc.Ping

    }
}