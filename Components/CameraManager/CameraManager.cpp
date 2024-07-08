// ======================================================================
// \title  CameraManager.cpp
// \author jjhessing
// \brief  cpp file for CameraManager component implementation class
// ======================================================================

#include "Components/CameraManager/CameraManager.hpp"
#include "Components/componentConfig/Constants.hpp"
#include "FpConfig.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  CameraManager ::
    CameraManager(const char* const compName) :
      CameraManagerComponentBase(compName)
  {
    this->picCount = 0;
  }

  CameraManager ::
    ~CameraManager()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  bool CameraManager ::
    deploy_handler(NATIVE_INT_TYPE portNum)
  {
    this->tlmWrite_cameraDeployed(GASRATS::deployed::DEPLOYED);
    this->log_ACTIVITY_HI_cameraDeployed();

    return true;
  }

  void CameraManager ::
    pingIn_handler(
        NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    this->pingOut_out(0,key);
  }

  void CameraManager ::
    takePic_handler(NATIVE_INT_TYPE portNum)
  {
    this->picCount++;

    #ifndef VIRTUAL
      std::string stillautocommand = "libcamera-still -r -e png -o /home/gas/photos/fprimeTest" + std::to_string(this->picCount) + ".jpg --autofocus-on-capture --hdr --rawfull --awb auto --denoise auto";
      std::system(stillautocommand.c_str());
    #endif

    this->log_ACTIVITY_HI_picTaken(this->picCount);
    this->tlmWrite_picsTaken(this->picCount);
  }

}
