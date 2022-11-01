// LOCAL INCLUDES
#include "SignalHnd.h"
#include "Utils.h"

// SYSTEM INCLUDES
#include <iostream>
#include <signal.h>

namespace DnsFwd::SignalHnd
{
    bool Register(int a_Sig, void (*a_Fn)(int))
    {
        struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = a_Fn;
        int rc = sigaction(a_Sig, &sa, NULL) != 0;
        if (rc)
        {
            PRINTER_ERNO("Failed to register signal " << a_Sig);
        }

        // sigaction returns 0 on success
        return (rc) ? false : true;
    }
}  // namespace DnsFwd::SignalHnd

