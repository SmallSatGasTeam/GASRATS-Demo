# include "../Definitions.fppi"
module Components {
    @ The application layer of the FSW.
    active component FlightLogic {

        #-----------------------------------------------------------------------
        # FlightLogic Ports 
        #-----------------------------------------------------------------------

        @ getClock: pulls the clock from the on board clock
        output port getClock: FL.getClock

        @ deployAntenna: deploys the Antenna
        output port deployAntenna : FL.deploy

        @ deployCamera: deploys the Camera
        output port deployCamera : FL.deploy

        @ hardwareHealth: Checks health of the hardware
        output port hardwareHealth : FL.getHealth

        @ epsHealth: special port for checking the health of the EPS on startup
        output port epsHealth : FL.epsHealth

        @ takePic: pings the camera to take a picture
        output port takePic : FL.ping

        @ sendTransmission: pings the transceiver to send a transmission
        output port sendTrans : FL.data

        @ recvTransmission: Flight Logic is pinged to know that a message has been received
        sync input port recvTransmission: FL.data

        @ startup: runs the startup code
        sync input port startup : Svc.Sched

        @ pingIn : receives health pings
        async input port pingIn: Svc.Ping

        @ pingOut : Returns health ping
        output port pingOut: Svc.Ping

        @ sendBeaconState : returns the state of the beacon
        sync input port sendBeaconState : FL.beaconState
        # Note, currently this doesn't actually handle any of the data to send
        # a transmission, we're probably just going to have the data collector
        # send that data directly to the queue of the transmission handler

        #-----------------------------------------------------------------------
        # Parameters
        #-----------------------------------------------------------------------

        # @ antennaDeployed: Startup flag indicating successful deployment of antenna
        # param antennaDeployed: bool default false

        # @ initialCompleted: Startup flag indicating successful initial beacon
        # param initialCompleted: bool default false

        # @ collectData: Parameter indicating whether or not data should be collected
        # param collectData: bool default true


        #-----------------------------------------------------------------------
        # Commands
        #-----------------------------------------------------------------------

        @ takePic: sends command to camera to take a picture
        async command takePic priority 50 drop

        @ sendTransmission: commands system to transmit for x miliseconds
        async command sendTransmission (
            duration: U32
        ) \
            priority 75 drop

        @ resetFlags: resets startup flags to original condition
        sync command resetFlags

        @ startupCommand: temporary to test startup ports while we still don't have startup working
        sync command startupCommand


        #-----------------------------------------------------------------------
        # Events
        #-----------------------------------------------------------------------

        # @ watchdog: denotes that the watchdog has been pinged
        # event watchdog severity activity low format "Pinged watchdog"

        @ deployFailure : Warning a component failed to deploy
        event deployFailure(comp: string) \
            severity warning high \
            format "{} failed to deploy."

        @ runningStartup: Startup is being run
        event runningStartup severity warning low format "Running startup."


        #-----------------------------------------------------------------------
        # Telemetry
        #-----------------------------------------------------------------------

        @ softwareHealth
        telemetry softwareHealth: U8 update on change \
            low {red 0, orange 1, yellow 2} \
            high {red 6, orange 5, yellow 4}

        @ hardwareHealth
        telemetry hardwareHealth: GASRATS.Health update on change #It's probably worth turning this into an integer so we can put warnings on it

        @ antennaState: tells if antenna is deployed or undeployed
        telemetry antennaState: GASRATS.deployed 

        @cameraState: tells if camera is deployed or not
        telemetry cameraState: GASRATS.deployed 

        @beaconState: tells the state of the beacon
        telemetry beaconState: GASRATS.beacon 



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