// ======================================================================
// \title  DummyGPIO.cpp
// \author jjhessing
// \brief  cpp file for DummyGPIO component implementation class
// ======================================================================

#include "DummyComponents/DummyGPIO/DummyGPIO.hpp"

namespace DummyComponents {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

DummyGPIO ::DummyGPIO(const char *const compName)
    : DummyGPIOComponentBase(compName) {}

DummyGPIO ::~DummyGPIO() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

Drv::GpioStatus DummyGPIO ::heartBeatDummyIn_handler(FwIndexType portNum,
                                                     const Fw::Logic &state) {
  this->tlmWrite_HIGH_LOW(state);

  return Drv::GpioStatus::OP_OK;
}

} // namespace DummyComponents
