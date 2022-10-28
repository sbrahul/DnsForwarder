// LOCAL INCLUDES
#include "ServerApp.h"

// SYSTEM INCLUDES
#include <cstring>
#include <iostream>
#include <unistd.h>

namespace
{
    uint8_t _recvBuffer[1500];
}

bool DnsFwd::ServerApp::m_Terminate = false;

DnsFwd::ServerApp::ServerApp(const char* ip, uint16_t port)
    : m_Server(ip, port)
{
}

DnsFwd::ServerApp::~ServerApp()
{
}

void DnsFwd::ServerApp::Terminate(int signal)
{
    std::cout << "Received signal " << signal << "\n";
    m_Terminate = true;
}

void DnsFwd::ServerApp::Run(uint32_t ip, uint16_t port)
{
    m_UpstreamIp = ip;
    m_UpstreamPort = port;

    if (!m_Server.CreateAndBind())
    {
        return;
    }

    struct sockaddr_in6 saddr = {0};
    auto bytes = m_Server.Recv(_recvBuffer, sizeof _recvBuffer, &saddr);
    
    // Check if terminated
    if (m_Terminate)
    {
        std::cout << "Closing server\n";
        return;
    }

    pause();
}

