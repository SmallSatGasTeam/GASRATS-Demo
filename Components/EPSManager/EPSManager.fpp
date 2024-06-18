module Components {
    @ Component to translate commands from the flight logic to the EPS.
    passive component EPSManager {

        # -----------------------------------------------------------------------------
        # Ports
        # -----------------------------------------------------------------------------

        @ epsHealth: passes the EPS voltage and current to the flight logic
        sync input port returnHealth : FL.epsHealth

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


        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}