// LOCAL INCLUDES
#include "ArgsParse.h"
#include "ServerApp.h"
#include "SignalHnd.h"

// SYSTEM INCLUDES
#include <iostream>
#include <signal.h>

namespace
{
    void Usage(const char* progName)
    {
        std::cout << progName << " <Upstream IP> <port>\n";
    }
}  // namespace


int main(int argc, char* argv[])
{
    auto [ip, port] = DnsFwd::ArgsParse::Parse(argc, argv);

    if (!ip || !port)
    {
        Usage(argv[0]);
        exit(1);
    }

    // Register SIGINT for graceful termination on Ctrl+C
    DnsFwd::SignalHnd::Register(SIGINT, &DnsFwd::ServerApp::Terminate);
    DnsFwd::ServerApp s;
    s.Run(ip, port);

    return 0;
}

