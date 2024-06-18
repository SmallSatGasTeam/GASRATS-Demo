// ======================================================================
// \title  EPSManager.cpp
// \author jjhessing
// \brief  cpp file for EPSManager component implementation class
// ======================================================================

#include "Components/EPSManager/EPSManager.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  EPSManager ::
    EPSManager(const char* const compName) :
      EPSManagerComponentBase(compName)
  {

  }

  EPSManager ::
    ~EPSManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void EPSManager ::
    returnHealth_handler(
        NATIVE_INT_TYPE portNum,
        F32& voltage,
        F32& current
    )
  {
    // TODO return
    //!!! This is code to simulate the EPS. The code will only be run if VIRTUAL is defined
    #ifdef VIRTUAL
    std::normal_distribution<float> currentDistribution(3,.5);
    std::normal_distribution<float> voltageDistribution(7,1);
    current = currentDistribution(generator);
    voltage = voltageDistribution(generator);

    #endif

    this->tlmWrite_current(current);
    this->tlmWrite_voltage(voltage);
  }

}
