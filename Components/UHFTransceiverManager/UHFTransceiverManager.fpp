module Components {
    @ component contains functions that configure the UHF transceiver's settings, and allows for data to be sent / received. "
    active component UHFTransceiverManager {

        ##############################################################################
        # Commands
        ##############################################################################
        async command configureRFSettings() opcode 0x01

        async command transmitData($data: string) opcode 0x02 


        ##############################################################################
        # Ports
        ##############################################################################

        @ Writes I2CData: Drv.I2c
        output port i2cReadWrite: Drv.I2cWriteRead

        output port i2cRead: Drv.I2c

        output port i2cWrite: Drv.I2c

        @ pingOut : returns health ping
        output port pingOut: Svc.Ping 


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

        event somethingHappened(response: string) \
            severity activity high \
            format "{}"
        


        ###############################################################################
        # Telemetry
        ###############################################################################

        @ uptime [U8] : The amount of time in seconds since startup.
        telemetry uptime: U8 

        @ packetsReceived [U8] : The amount of packets received .
        telemetry packetsReceived: U8 

        @ packetsTransmitted [U8] : The amount of packets transmitted.
        telemetry packetsTransmitted: U8 

        @ temperature [STRING] : The temperature of transceiver.
        telemetry temperature: U8

        @ powerMode [BOOL] : Power mode of transceiver. If true, the device is in low power mode. 
        telemetry lowPowerMode: bool

        @ response [STRING] : Full response that was returned from a READ / WRITE command.
        telemetry response1: string

        @ response [STRING] : Full response that was returned from a READ / WRITE command.
        telemetry response2: string

        @ responseStatus [STRING] : Status of response. Either OK or ERR
        telemetry responseStatus: bool


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