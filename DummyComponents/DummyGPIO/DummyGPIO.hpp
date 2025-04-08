// ======================================================================
// \title  DummyGPIO.hpp
// \author jjhessing
// \brief  hpp file for DummyGPIO component implementation class
// ======================================================================

#ifndef DummyComponents_DummyGPIO_HPP
#define DummyComponents_DummyGPIO_HPP

#include "DummyComponents/DummyGPIO/DummyGPIOComponentAc.hpp"

namespace DummyComponents {

class DummyGPIO : public DummyGPIOComponentBase {

public:
  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  //! Construct DummyGPIO object
  DummyGPIO(const char *const compName //!< The component name
  );

  //! Destroy DummyGPIO object
  ~DummyGPIO();

PRIVATE:
  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  //! Handler implementation for heartBeatDummyIn
  Drv::GpioStatus
  heartBeatDummyIn_handler(FwIndexType portNum, //!< The port number
                           const Fw::Logic &state) override;
};

} // namespace DummyComponents

#endif
