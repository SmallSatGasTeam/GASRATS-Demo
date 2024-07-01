module Components {
    @ Used to simulate the tranceiver before we have one actually plugged in
    active component DummyTranceiverDriver {

        ##############################################################################
        # Ports
        ##############################################################################

        async input port sendTransToGround: FL.serialData

        output port recvTransFromGround: FL.serialData

        ##############################################################################
        # Commands
        ##############################################################################

        async command sendTransToSatellite (data: U32)

        ##############################################################################
        # Events
        ##############################################################################

        event receivingTransGround(data: U32) \
            severity activity high \
            format "Receiving {x} from satellite"

        event sendingTransGround(data: U32) \
            severity activity high \
            format "Sending {x} to the satellite"

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

    }
}