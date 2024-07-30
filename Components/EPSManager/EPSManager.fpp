module Components {
    @ Component to translate commands from the flight logic to the EPS.
    passive component EPSManager {

        # -----------------------------------------------------------------------------
        # Ports
        # -----------------------------------------------------------------------------

        @ epsHealth: passes the EPS voltage and current to the flight logic
        sync input port returnHealth : FL.epsHealth

        @ collectorRequest: receives a ping from the data collector to send out data
        sync input port dataRequest: FL.data

        @ other way to output data for the data collector
        output port epsData: Fw.BufferSend

        @ Writes I2CData: Drv.I2c
        output port i2cReadWrite: Drv.I2cWriteRead

        # -----------------------------------------------------------------------------
        # Telemetry
        # -----------------------------------------------------------------------------

        @ voltage: The voltage being supplied to the board from the EPS in volts
        telemetry voltage: F32 format "{.3f}"\
            low{red 3, orange 4, yellow 5} \
            high {red 10, orange 9}

        @ current: The current being supplied to the board from the EPS in amps
        telemetry current: F32 format "{.3f}"\
            low{red 0, orange 1}

        event epsSuccess \
            severity activity high \
            format "The EPS sent the data"
        
        event epsAddressFailure \
            severity warning high \
            format "EPS Invalid Address"

        event epsWriteError \
            severity warning high \
            format "EPS Write Failed"

        event epsReadError \
            severity warning high \
            format "EPS Read Failed"

        event epsOpenError \
            severity warning high \
            format "EPS Failed to open device"
        
        event epsOtherError \
            severity warning high \
            format "EPS Other"

        @ Allocation failed event
        event MemoryAllocationFailed() severity warning low format "EPS Failed to allocate memory"


        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Allocation port for a buffer
        output port allocate: Fw.BufferGet

        @ Deallocation port for buffers
        output port deallocate: Fw.BufferSend

    }
}