// ======================================================================
// \title  EPSManager.hpp
// \author jjhessing
// \brief  hpp file for EPSManager component implementation class
// ======================================================================

#ifndef Components_EPSManager_HPP
#define Components_EPSManager_HPP

#include "Components/EPSManager/EPSManagerComponentAc.hpp"
#include "Components/componentConfig/Constants.hpp"
#include <random>

namespace Components {

  class EPSManager :
    public EPSManagerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct EPSManager object
      EPSManager(
          const char* const compName //!< The component name
      );

      //! Destroy EPSManager object
      ~EPSManager();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for returnHealth
      //!
      //! epsHealth: passes the EPS voltage and current to the flight logic
      void returnHealth_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& voltage,
          F32& current
      ) override;

    #ifdef VIRTUAL
    PRIVATE:
      std::default_random_engine generator;
    #endif

  };

}

#endif
