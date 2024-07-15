// ======================================================================
// \title  I2cDriver.cpp
// \author jjhessing
// \brief  cpp file for I2cDriver component implementation class
// ======================================================================

#include "Components/I2cDriver/I2cDriver.hpp"
#include "FpConfig.hpp"

namespace Drv {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  I2cDriver ::
    I2cDriver(const char* const compName) :
      Drv::LinuxI2cDriver(compName)
  {
    this->open(this->device);
  }

  I2cDriver ::
    ~I2cDriver()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

//   Drv::I2cStatus I2cDriver ::
//     read_handler(
//         NATIVE_INT_TYPE portNum,
//         U32 addr,
//         Fw::Buffer& serBuffer
//     )
//   {
//     // TODO return
//   }

//   Drv::I2cStatus I2cDriver ::
//     write_handler(
//         NATIVE_INT_TYPE portNum,
//         U32 addr,
//         Fw::Buffer& serBuffer
//     )
//   {
//     // TODO return
//   }

//   Drv::I2cStatus I2cDriver ::
//     writeRead_handler(
//         NATIVE_INT_TYPE portNum,
//         U32 addr,
//         Fw::Buffer& writeBuffer,
//         Fw::Buffer& readBuffer
//     )
//   {
//     // TODO return
//   }

}
