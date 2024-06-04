// ======================================================================
// \title  AntennaDeploy.hpp
// \author jjhessing
// \brief  hpp file for AntennaDeploy component implementation class
// ======================================================================

#ifndef Components_AntennaDeploy_HPP
#define Components_AntennaDeploy_HPP

#include "Components/AntennaDeploy/AntennaDeployComponentAc.hpp"

namespace Components {

  class AntennaDeploy :
    public AntennaDeployComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct AntennaDeploy object
      AntennaDeploy(
          const char* const compName //!< The component name
      );

      //! Destroy AntennaDeploy object
      ~AntennaDeploy();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for deploy
      //!
      //! deploy: Deploys the antenna
      bool deploy_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

  };

}

#endif
