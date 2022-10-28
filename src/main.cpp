// LOCAL INCLUDES
#include "ArgsParse.h"
#include "ServerApp.h"
#include "SignalHnd.h"

// SYSTEM INCLUDES
#include <iostream>
#include <signal.h>

namespace
{
    void Usage_a(const char* progName)
    {
        std::cout << progName << " <Upstream IP> <port>\n";
    }

    //const char* LISTEN_IP = "127.0.0.1";
    const char* LISTEN_IP = "::1";
    const uint16_t LISTEN_PORT = 9000;
}  // namespace


int main(int argc, char* argv[])
{
    auto [ip, port] = DnsFwd::ArgsParse::Parse(argc, argv);

    if (!ip || !port)
    {
        Usage_a(argv[0]);
        exit(1);
    }

    // Register SIGINT for graceful termination on Ctrl+C
    DnsFwd::SignalHnd::Register(SIGINT, &DnsFwd::ServerApp::Terminate);
    DnsFwd::ServerApp s(LISTEN_IP, LISTEN_PORT);
    s.Run(ip, port);

    return 0;
}

