# include "../Definitions.fppi"
module Components {
    @ The application layer of the FSW.
    active component FlightLogic {

        #-----------------------------------------------------------------------
        # FlightLogic Ports 
        #-----------------------------------------------------------------------

        @ deployAntenna: deploys the Antenna
        output port deployAntenna : FL.deploy

        @ deployCamera: deploys the Camera
        output port deployCamera : FL.deploy

        @ epsHealth: special port for checking the health of the EPS on startup
        output port epsHealth : FL.epsHealth

        @ takePic: pings the camera to take a picture
        output port takePic : FL.ping

        @ sendTransmission: pings the transceiver to send a transmission
        output port sendTrans : FL.data

        @ fakeData: just a stream of the same number being sent to the Data Collector
        output port fakeData: FL.serialData

        @ recvTransmission: Flight Logic is pinged to know that a message has been received
        sync input port recvTransmission: FL.data

        @ startup: runs the startup code
        sync input port startup : Svc.Sched

        @ pingIn : receives health pings
        async input port pingIn: Svc.Ping

        @ dataRequest: receives a ping from the data collector to send out data
        sync input port dataRequest: FL.data

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


        #-----------------------------------------------------------------------
        # Events
        #-----------------------------------------------------------------------

        # @ watchdog: denotes that the watchdog has been pinged
        # event watchdog severity activity low format "Pinged watchdog"

        @ deployFailure : Warning a component failed to deploy
        event deployFailure(comp: string) \
            severity warning high \
            format "{} failed to deploy."

        @ runningStartup: Startup is being run (For debugging only)
        event runningStartup(num: U32) severity warning low format "Running startup {}."

        @ detumble: Notifies that the OBC has told the ADCS micro controller to detumble
        event detumble severity activity low format "Running detumbling."

        @ rebooting: Notifies that the OBC is rebooting the satellite
        event rebooting severity fatal format "Rebooting!"


        #-----------------------------------------------------------------------
        # Telemetry
        #-----------------------------------------------------------------------

        @ antennaState: tells if antenna is deployed or undeployed
        telemetry antennaState: GASRATS.deployed 

        @ cameraState: tells if camera is deployed or not
        telemetry cameraState: GASRATS.deployed 

        @ beaconState: tells the state of the beacon
        telemetry beaconState: GASRATS.beacon 

        @ lowPower: Shows whether or not we're in low power mode
        telemetry lowPower: bool

        @ detumbled: Shows whether or not the OBC thinks we're detumbled
        telemetry detumbled: bool


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