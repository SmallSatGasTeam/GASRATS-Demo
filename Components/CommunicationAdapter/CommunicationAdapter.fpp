module Components {
    @ Bridge between F' and the transciever
    active component CommunicationAdapter {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO
        async command TODO opcode 0 # maybe this can be for sending a specific command to the transciever, idk

        ###############################################################################
        # Stuff for the Communication Adapter interface
        ###############################################################################

        @ Port for recieving Fw:Buffer objects for outgoing transmission
        sync input port comDataIn: Drv.ByteStreamSend # i have no idea if this should be sync or async

        @ Port producing incoming Fw::Buffer objects
        output port comDataOut: Drv.ByteStreamRecv

        @ Port indicating status of outgoing transmission
        output port comStatus: Fw.SuccessCondition

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

        @ Deallocation port for buffers
        output port deallocate: Fw.BufferSend
    }
}