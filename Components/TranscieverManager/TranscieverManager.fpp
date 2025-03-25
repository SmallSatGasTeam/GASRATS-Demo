module Components {
    # Three different states for the transciever: off, startup, running
    @ Will take in commands and act as the state machine for the transciever
    active component TranscieverManager {

        # One async command/port is required for active components
        @ Startup the transciever
        async command STARTUP()

        @ read i2c data
        output port i2cRead: Drv.I2c

        @ write i2c data
        output port i2cWrite: Drv.I2c

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