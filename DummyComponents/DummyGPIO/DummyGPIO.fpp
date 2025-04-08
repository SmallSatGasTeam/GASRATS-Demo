module DummyComponents {
    @ Exists to connect the port to so that it won't explode.
    passive component DummyGPIO {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        sync input port heartBeatDummyIn: Drv.GpioWrite

        telemetry HIGH_LOW: Fw.Logic

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