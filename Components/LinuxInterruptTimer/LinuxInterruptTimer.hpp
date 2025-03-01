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
    void stopTimer();

    void doCycle();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command StartTimer
    void StartTimer_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                               U32 cmdSeq            //!< The command sequence number
                               ) override;

    //! Handler implementation for command StopTimer
    void StopTimer_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                              U32 cmdSeq            //!< The command sequence number
                              ) override;

    static void signalHandler(
      int sig,
      siginfo_t *si, 
      void *uc
    );


    struct sigevent sev;
    struct sigaction sa;

    timer_t timerId = 0;

    uint64_t ticks;
};

}  // namespace Components

#endif
