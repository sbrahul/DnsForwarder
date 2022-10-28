// LOCAL INCLUDES
#include "SignalHnd.h"

// SYSTEM INCLUDES
#include <signal.h>
#include <stdio.h>


namespace DnsFwd::SignalHnd
{
    bool Register(int sig, void (*fn)(int))
    {
        struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = fn;
        int rc = sigaction(sig, &sa, NULL) != 0;
        if (rc)
        {
            perror("Failed to register signal");
        }

        // sigaction returns 0 on success and this function should return true
        return (rc) ? false : true;
    }
}

