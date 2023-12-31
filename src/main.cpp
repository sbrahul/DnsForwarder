// LOCAL INCLUDES
#include "ArgsParse.h"
#include "ServerApp.h"
#include "SignalHnd.h"
#include "Utils.h"

// SYSTEM INCLUDES
#include <iostream>
#include <signal.h>

namespace
{
    void _Usage(const char* a_ProgName)
    {
        std::cout << a_ProgName << " <Upstream IPv4> <port>\n";
    }

    const std::string LISTEN_IP4("127.0.0.1");
    const std::string LISTEN_IP6("::1");
    const uint16_t LISTEN_PORT(9000);
}  // namespace

int
main(int a_Argc, char* a_Argv[])
{
    // Parse cmd line args
    auto [ip, port] = DnsFwd::ArgsParse::Parse(a_Argc, a_Argv);

    if (!ip || !port)
    {
        _Usage(a_Argv[0]);
        exit(1);
    }

    // Register SIGINT for graceful termination on Ctrl+C
    DnsFwd::SignalHnd::Register(SIGINT, &DnsFwd::ServerApp::Terminate);

    // Start the main server application
    DnsFwd::ServerApp s(LISTEN_IP4, LISTEN_IP6, LISTEN_PORT);
    s.Run(ip, port);

    return 0;
}

