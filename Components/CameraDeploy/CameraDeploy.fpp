module Components {
    @ Deploys the camera
    active component CameraDeploy {

        #-----------------------------------------------------------------------
        # Ports 
        #-----------------------------------------------------------------------

        @ deploy: port to deploy the camera
        sync input port deploy : FL.deploy

        @ takePic: takes a picture
        async input port takePic : FL.ping

        #-----------------------------------------------------------------------
        # Events
        #-----------------------------------------------------------------------

        @ cameraDeployed : signals that the camera is deployed
        event cameraDeployed severity activity high format "Camera is deployed"

        @ picTaken : Signals a pic has been taken
        event picTaken(pics: U32) severity activity high format "{} pictures taken"

        #-----------------------------------------------------------------------
        # Telemetry
        #-----------------------------------------------------------------------

        @ cameraDeployed
        telemetry cameraDeployed : GASRATS.deployed

        @ picsTaken
        telemetry picsTaken : U16

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

    }
}