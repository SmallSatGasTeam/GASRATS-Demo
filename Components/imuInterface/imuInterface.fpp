module Components {
    @ This will be the component that draws information from the IMU we have
    active component imuInterface {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO
        async command TODO opcode 0

        ###############################################################################
        # Ports
        ###############################################################################

        @ dataRequest: receives a ping from the data collector to send out data
        sync input port dataRequest: FL.data

        @ output data for the data collector
        output port collector: FL.serialData

        @ Gets the data from the I2C device through pass by refrence
        output port requestI2CData: Drv.I2c

        ###############################################################################
        # Events 
        ###############################################################################

        event imuSuccess \
            severity activity high \
            format "The IMU sent the data"

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