module Components {
    @ component to connect timer cycle to rate groups.
    active component LinuxInterruptTimer {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port

        ###### Commands

        async command StartTimer()
        async command StopTimer()

        ###### Events
        event TimerCreated severity activity high format "The timer was created"
        event TimerTick severity activity high format "Timer tick"
        event Error severity warning high format "An error has occured"


        ###### Output ports
        ## This port shall send timer ticks, not sure if it is over I2c though.
        output port CycleIn: Svc.Cycle

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

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

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}