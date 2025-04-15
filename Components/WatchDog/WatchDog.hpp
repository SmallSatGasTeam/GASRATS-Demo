// ======================================================================
// \title  WatchDog.hpp
// \author jjhessing
// \brief  hpp file for WatchDog component implementation class
// ======================================================================

#ifndef Components_WatchDog_HPP
#define Components_WatchDog_HPP

#include "Components/WatchDog/WatchDogComponentAc.hpp"

namespace Components {

// Global variable to communicate between asserts and the WatchDog component
extern bool healthy;

class WatchDog : public WatchDogComponentBase {
  public:
      int numPings;
      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct WatchDog object
      WatchDog(const char *const compName //!< The component name
      );

      //! Destroy WatchDog object
      ~WatchDog();

      PRIVATE :
          // ----------------------------------------------------------------------
          // Handler implementations for user-defined typed input ports
          // ----------------------------------------------------------------------

          //! Handler implementation for healthIn
          //!
          //! Accepts the code coming in from the health component
          void
          healthIn_handler(NATIVE_INT_TYPE portNum, //!< The port number
                           U32 code                 //!< Watchdog stroke code
                           ) override;

      bool stroke;
};

class GaspacsAssertHook : public Fw::AssertHook {
  public:
    GaspacsAssertHook() : previousHook(nullptr) {
    }

    ~GaspacsAssertHook() {
    }

    void reportAssert(FILE_NAME_ARG file, NATIVE_UINT_TYPE lineNo, NATIVE_UINT_TYPE numArgs, FwAssertArgType arg1,
                      FwAssertArgType arg2, FwAssertArgType arg3, FwAssertArgType arg4, FwAssertArgType arg5,
                      FwAssertArgType arg6) override;

    void doAssert() override;

    void defaultReportAssert(
        FILE_NAME_ARG file,
        NATIVE_UINT_TYPE lineNo,
        NATIVE_UINT_TYPE numArgs,
        FwAssertArgType arg1,
        FwAssertArgType arg2,
        FwAssertArgType arg3,
        FwAssertArgType arg4,
        FwAssertArgType arg5,
        FwAssertArgType arg6,
        CHAR *destBuffer,
        NATIVE_INT_TYPE buffSize);

private:
    Fw::AssertHook* previousHook;
};

}  // namespace Components

#endif
