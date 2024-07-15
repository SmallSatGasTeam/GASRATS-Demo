// ======================================================================
// \title  I2cDriver.hpp
// \author jjhessing
// \brief  hpp file for I2cDriver component implementation class
// ======================================================================

#ifndef Drv_I2cDriver_HPP
#define Drv_I2cDriver_HPP

#include "Components/I2cDriver/I2cDriverComponentAc.hpp"
#include "fprime/Drv/LinuxI2cDriver/LinuxI2cDriver.hpp"

namespace Drv {

  class I2cDriver :
    public LinuxI2cDriver
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct I2cDriver object
      I2cDriver(
          const char* const compName //!< The component name
      );

      //! Destroy I2cDriver object
      ~I2cDriver();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

    //   //! Handler implementation for read
    //   Drv::I2cStatus read_handler(
    //       NATIVE_INT_TYPE portNum, //!< The port number
    //       U32 addr, //!< I2C slave device address
    //       Fw::Buffer& serBuffer //!< Buffer with data to read/write to/from
    //   ) override;

    //   //! Handler implementation for write
    //   Drv::I2cStatus write_handler(
    //       NATIVE_INT_TYPE portNum, //!< The port number
    //       U32 addr, //!< I2C slave device address
    //       Fw::Buffer& serBuffer //!< Buffer with data to read/write to/from
    //   ) override;

    //   //! Handler implementation for writeRead
    //   Drv::I2cStatus writeRead_handler(
    //       NATIVE_INT_TYPE portNum, //!< The port number
    //       U32 addr, //!< I2C slave device address
    //       Fw::Buffer& writeBuffer, //!< Buffer to write data to the i2c device
    //       Fw::Buffer& readBuffer //!< Buffer to read back data from the i2c device, must set size when passing in read buffer
    //   ) override;
        const char* device = "/dev/i2c-1";

  };

}

#endif
