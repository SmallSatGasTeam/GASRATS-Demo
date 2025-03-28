// ======================================================================
// \title  LinuxInterruptTimer.hpp
// \author jjhessing
// \brief  hpp file for LinuxInterruptTimer component implementation class
// ======================================================================

#ifndef Components_LinuxInterruptTimer_HPP
#define Components_LinuxInterruptTimer_HPP

#include "Components/LinuxInterruptTimer/LinuxInterruptTimerComponentAc.hpp"
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

namespace Components {

class LinuxInterruptTimer : public LinuxInterruptTimerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct LinuxInterruptTimer object
    LinuxInterruptTimer(const char* const compName  //!< The component name
    );

    //! Destroy LinuxInterruptTimer object
    ~LinuxInterruptTimer();

    // -----
    // Seperate functions for other things
    // -----

    void startTimer();

    void doCycle();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    static void signalHandler(
      int sig,
      siginfo_t *si, 
      void *uc
    );

    //! Handler implementation for pingIn
    //!
    //! pingIn : receives health pings
    void pingIn_handler(FwIndexType portNum, //!< The port number
      U32 key              //!< Value to return to pinger
      ) override;


    struct sigevent sev;
    struct sigaction sa;

    timer_t timerId = 0;

    long ticks;
};

}  // namespace Components

#endif
