// ======================================================================
// \title  CameraDeploy.hpp
// \author jjhessing
// \brief  hpp file for CameraDeploy component implementation class
// ======================================================================

#ifndef Components_CameraDeploy_HPP
#define Components_CameraDeploy_HPP

#include "Components/CameraDeploy/CameraDeployComponentAc.hpp"

namespace Components {

  class CameraDeploy :
    public CameraDeployComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct CameraDeploy object
      CameraDeploy(
          const char* const compName //!< The component name
      );

      //! Destroy CameraDeploy object
      ~CameraDeploy();

      U16 picCount = 0;

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for deploy
      //!
      //! deploy: port to deploy the camera
      bool deploy_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for takePic
      //!
      //! takePic: takes a picture
      void takePic_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

  };

}

#endif
