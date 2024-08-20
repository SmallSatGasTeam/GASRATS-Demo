// ======================================================================
// \title  WatchDog.hpp
// \author jjhessing
// \brief  hpp file for WatchDog component implementation class
// ======================================================================

#ifndef Components_WatchDog_HPP
#define Components_WatchDog_HPP

#include "Components/WatchDog/WatchDogComponentAc.hpp"

namespace Components {

class WatchDog : public WatchDogComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct WatchDog object
    WatchDog(const char* const compName  //!< The component name
    );

    //! Destroy WatchDog object
    ~WatchDog();

  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for healthIn
    //!
    //! Accepts the code coming in from the health component
    void healthIn_handler(NATIVE_INT_TYPE portNum,  //!< The port number
                          U32 code                  //!< Watchdog stroke code
                          ) override;

    bool stroke;
};

}  // namespace Components

#endif
