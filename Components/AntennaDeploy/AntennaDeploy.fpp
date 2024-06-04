module Components {
    @ Component designed to deploy antenna
    passive component AntennaDeploy {

        #-----------------------------------------------------------------------
        # Events
        #-----------------------------------------------------------------------

        @ antennaDeployed: Opening the antenna door
        event antennaDeployed severity activity high format "Antenna deployed"


        #-----------------------------------------------------------------------
        # Telemetry
        #-----------------------------------------------------------------------

        @ antennaState: displays whether or not antenna is deployed
        telemetry antennaState: GASRATS.deployed update on change


        #-----------------------------------------------------------------------
        # Ports
        #-----------------------------------------------------------------------

        @ deploy: Deploys the antenna
        sync input port deploy : FL.deploy

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

    }
}