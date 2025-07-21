module Components {
    @ component contains functions that configure the UHF transceiver's settings, and allows for data to be sent / received. "
    active component UHFTransceiverManager {

        ##############################################################################
        # Commands
        ##############################################################################
        async command configureSettings() opcode 0x01

        async command sendData($data: string) opcode 0x02 


        ##############################################################################
        # Ports
        ##############################################################################

        ## ---------------------------------------------------------------------------
        ## I2C Ports
        ## ---------------------------------------------------------------------------
        @ I2C READ/WRITE. This port is used to read and write data to device.
        output port i2cReadWrite: Drv.I2cWriteRead

        @ I2C WRITE. This port is used to write data to device.
        output port i2cRead: Drv.I2c

        @ I2C WRITE. This port is used to write data to device.
        output port i2cWrite: Drv.I2c

        ## ---------------------------------------------------------------------------
        ## UART Ports
        ## ---------------------------------------------------------------------------
        @ UART SEND. Output port
        output port uartSend: Drv.ByteStreamSend

        @ UART RECV. Input port.
        async input port uartRecv: Drv.ByteStreamRecv

        @ UART READY. This port is used to check if the UART driver is ready to send data.
        async input port uartReady: Drv.ByteStreamReady

        @ Status of the last radio transmission
        output port comStatus: Fw.SuccessCondition

        @ pingOut : returns health ping
        output port pingOut: Svc.Ping 

        @ bufferSend : This port is used to send buffers (data) to the framer to send over the antenna.
        output port sendBuffer: Fw.BufferSend

        @ framedOut : Port for sending framed input (radio packet) to deframer
        output port framedOut: Drv.ByteStreamRecv


        ###############################################################################
        # Events
        ###############################################################################

        ## ---------------------------------------------------------------------------
        ## I2C Events
        ## ---------------------------------------------------------------------------
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
        
        ## ---------------------------------------------------------------------------
        ## UART Events
        ## ---------------------------------------------------------------------------
        event UHFUartSuccess \
            severity activity high \
            format "UART transmit successful"

        event UHFUartError \
            severity warning high \
            format "UART transmit error"

        event UHFUartRetry \
            severity warning low \
            format "UART retry"
        event UHFUartNotReady \
            severity warning high \
            format "UART is not ready to send data"

        event transmitDataSuccess \
            severity activity high \
            format "Data was successfully transmitted via UART"

        event transmitDataFailure \
            severity warning high \
            format "Data transmission via UART Failed"

        ## ---------------------------------------------------------------------------
        ## Other Events
        ## ---------------------------------------------------------------------------

        event MemoryAllocationFailed() \
            severity warning low \
            format "Failed to allocate memory"

        event debuggingEvent(response: string) \
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

        @ endurosatPacketsReceived [U8] : How many Endurosat Packets have been received
        telemetry endurosatPacketsReceived: U8

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