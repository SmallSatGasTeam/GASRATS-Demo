module Components {
    @ component contains functions that configure the UHF transceiver's settings, and allows for data to be sent / received. "
    active component UHFTransceiverManager {

        async command configureFrequency(
            $frequency: F32
        ) opcode 0x01


        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        @ Writes I2CData: Drv.I2c
        output port i2cReadWrite: Drv.I2cWriteRead

        @ pingOut : returns health ping
        output port pingOut: Svc.Ping 

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
        # Events
        ###############################################################################

        event UHFSuccess \
            severity activity high \
            format "The UHF sent the data"
        
        event UHFAddressFailure \
            severity warning high \
            format "UHF Invalid Address"

        event UHFWriteError \
            severity warning high \
            format "UHF Write Failed"

        event UHFReadError \
            severity warning high \
            format "UHF Read Failed"

        event UHFOpenError \
            severity warning high \
            format "UHF Failed to open device"
        
        event UHFOtherError \
            severity warning high \
            format "UHF Other"
        @ Allocation failed event
        event MemoryAllocationFailed() \
            severity warning low \
            format "Failed to allocate memory"

        event RadioFrequencyConfiguredOK(frequency: F32) \
            severity activity high \
            format "The radio frequency was configured to {}"


        ###############################################################################
        # Telemetry
        ###############################################################################

        @ readConfiguration: The voltage being supplied to the board from the EPS in volts
        telemetry readConfiguration1: U8 

        telemetry readConfiguration2: U8 


        telemetry commandString: string

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

        @ Allocation port for a buffer
        output port allocate: Fw.BufferGet

        @ Deallocation port for buffers
        output port deallocate: Fw.BufferSend

    }
}