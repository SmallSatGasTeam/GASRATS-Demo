// ======================================================================
// \title  CameraManager.hpp
// \author jjhessing
// \brief  hpp file for CameraManager component implementation class
// ======================================================================

#ifndef Components_CameraManager_HPP
#define Components_CameraManager_HPP

#include "Components/CameraManager/CameraManagerComponentAc.hpp"

namespace Components {

  class CameraManager :
    public CameraManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct CameraManager object
      CameraManager(
          const char* const compName //!< The component name
      );

      //! Destroy CameraManager object
      ~CameraManager();

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

      //! Handler implementation for pingIn
      //!
      //! pingIn : receives health pings
      void pingIn_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 key //!< Value to return to pinger
      ) override;

      //! Handler implementation for takePic
      //!
      //! takePic: takes a picture
      void takePic_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

    PRIVATE:
      //! Variable picCount
      //!
      //! Tracks how many pictures have been taken
      U32 picCount;

  };

}

#endif
