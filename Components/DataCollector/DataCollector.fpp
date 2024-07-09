module Components {
    @ This component will periodically ping for data from all other components, and aggregate and save the data.    
    active component DataCollector {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO
        async command TODO opcode 0

        ###############################################################################
        # Ports 
        ###############################################################################

        @ Receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Recieving data from multiple channels
        sync input port aggregate: serial

        @ Reciving buffer data
        sync input port imuIncoming: Fw.BufferSend

        @ Port meant for pinging all connected ports for their data
        output port ping: [2] FL.data

        ###############################################################################
        # Events 
        ###############################################################################

        event dataSuccess \
            severity activity high \
            format "The data collector has recieved the data!"

        event dataOut \
            severity activity high \
            format "The imu gave us DATA!"

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