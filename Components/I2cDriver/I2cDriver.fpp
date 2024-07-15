module Drv {
    @ Driver for the i2c
    passive component I2cDriver {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        guarded input port write: Drv.I2c

        guarded input port read: Drv.I2c

        guarded input port writeRead: Drv.I2cWriteRead

    }
}