// ======================================================================
// \title  CameraDeploy.cpp
// \author jjhessing
// \brief  cpp file for CameraDeploy component implementation class
// ======================================================================

#include "Components/CameraDeploy/CameraDeploy.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  CameraDeploy ::
    CameraDeploy(const char* const compName) :
      CameraDeployComponentBase(compName)
  {

  }

  CameraDeploy ::
    ~CameraDeploy()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  bool CameraDeploy ::
    deploy_handler(NATIVE_INT_TYPE portNum)
  {
    // TODO
    this->tlmWrite_cameraDeployed(GASRATS::deployed::DEPLOYED);
    this->log_ACTIVITY_HI_cameraDeployed();

    return true;
  }

  void CameraDeploy ::
    takePic_handler(NATIVE_INT_TYPE portNum)
  {
    // TODO
    this->picCount++;
    this->log_ACTIVITY_HI_picTaken(this->picCount);
    this->tlmWrite_picsTaken(this->picCount);
  }

}
