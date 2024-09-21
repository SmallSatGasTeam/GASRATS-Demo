module Components {
    @ This will be the component that draws information from the IMU we have
    passive component imuInterface {

        ###############################################################################
        # Ports
        ###############################################################################

        @ collectorRequest: receives a ping from the data collector to send out data
        sync input port dataRequest: FL.data #!!!Does this need to be FL.data? we don't seem to have any need to return a value

        @ other way to output data for the data collector
        output port gyroData: Fw.BufferSend

        @ Gets the data from the I2C device through pass by refrence
        output port requestI2CData: Drv.I2c

        @ Writes I2CData: Drv.I2c
        output port i2cWrite: Drv.I2c

        ###############################################################################
        # Events 
        ###############################################################################

        @The next 6 events are used to check for read and write errors when interfacing with an i2c device
        event imuSuccess \
            severity activity high \
            format "The IMU sent the data"
        
        event imuAddressFailure \
            severity warning high \
            format "Invalid Address"

        event imuWriteError \
            severity warning high \
            format "Write Failed"

        event imuReadError \
            severity warning high \
            format "Read Failed"

        event imuOpenError \
            severity warning high \
            format "Failed to open device"
        
        event imuOtherError \
            severity warning high \
            format "Other"

        @ Allocation failed event
        event MemoryAllocationFailed() severity warning low format "Failed to allocate memory"

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

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