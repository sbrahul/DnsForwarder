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
    void Usage_a(const char* a_ProgName)
    {
        PRINTER(a_ProgName << " <Upstream IP> <port>\n");
    }

    // const char* LISTEN_IP = "127.0.0.1";
    const char* LISTEN_IP = "::1";
    const uint16_t LISTEN_PORT = 9000;
}  // namespace

int
main(int a_Argc, char* a_Argv[])
{
    auto [ip, port] = DnsFwd::ArgsParse::Parse(a_Argc, a_Argv);

    if (!ip || !port)
    {
        Usage_a(a_Argv[0]);
        exit(1);
    }

    // Register SIGINT for graceful termination on Ctrl+C
    DnsFwd::SignalHnd::Register(SIGINT, &DnsFwd::ServerApp::Terminate);
    DnsFwd::ServerApp s(LISTEN_IP, LISTEN_PORT);
    s.Run(ip, port);

    return 0;
}
