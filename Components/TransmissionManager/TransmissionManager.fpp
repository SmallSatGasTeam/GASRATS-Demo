module Components {
    @ Everything goes through this component, it manages all transmissions.
    # include "../Definitions.fppi"

    active component TransmissionManager {

        ###############################################################################
        # Ports
        ###############################################################################

        @ This is synchronous because it can return a value if it has successfully received data
        sync input port recvData: FL.serialData
        output port sendData: FL.serialData
        @ Can pull or set the beacon state
        output port beaconState: FL.beaconState

        @ Port that calls the beacon
        sync input port beacon : Svc.Sched


        ###############################################################################
        # Events 
        ###############################################################################

        @ Verifies data has been received
        event success(arg1: U32) \
            severity activity low \
            format "We recieved the data: {x}"

        @ Verifies satellite is sending data
        event sending(data: U32)\
            severity activity low \
            format "Sending {x}"

        @ Announces beacon has been set to a specific state
        event beaconSet(state: GASRATS.beacon) \
            severity activity high \
            format "Beacon has been set to {}"

        event invalidBeaconState \
            severity warning low \
            format "WARNING, beacon state invalid. Resetting state to INITIAL"

        ###############################################################################
        # Commands 
        ###############################################################################

        @ Confirms connection with the ground
        sync command confirmConnection

        @ Command used to set the beacon to a specific state. Only use to set to INITIAL or STANDARD
        async command setBeacon (state: GASRATS.beacon)

        @ Sends data from the satellite to ground
        async command sendTransToGround (data: U32)

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

    }
}