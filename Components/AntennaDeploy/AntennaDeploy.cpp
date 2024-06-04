// ======================================================================
// \title  AntennaDeploy.cpp
// \author jjhessing
// \brief  cpp file for AntennaDeploy component implementation class
// ======================================================================

#include "Components/AntennaDeploy/AntennaDeploy.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  AntennaDeploy ::
    AntennaDeploy(const char* const compName) :
      AntennaDeployComponentBase(compName)
  {

  }

  AntennaDeploy ::
    ~AntennaDeploy()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  bool AntennaDeploy ::
    deploy_handler(NATIVE_INT_TYPE portNum)
  {
    // TODO return
    
    this->log_ACTIVITY_HI_antennaDeployed();
    this->tlmWrite_antennaState(GASRATS::deployed::DEPLOYED);
    return true;
  }

}
