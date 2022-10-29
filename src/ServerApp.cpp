// LOCAL INCLUDES
#include "ServerApp.h"
#include "DnsPacket.h"
#include "Udp.h"

// SYSTEM INCLUDES
#include <algorithm>
#include <cstring>
#include <iostream>
#include <unistd.h>

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

    while (1)
    {
        DnsPacket pkt = m_Server.Recv();

        // Check if terminated
        if (m_Terminate)
        {
            std::cout << "Closing server\n";
            break;
        }

        if (pkt.IsEmpty())
        {
            // Either timeout or some interrupt
            continue;
        }

        auto tx_id = pkt.GetTxId();
        if (std::find(m_TxQ.begin(), m_TxQ.end(), tx_id) != m_TxQ.end())
        {
            std::cout << "Duplicate request. Dropping\n";
            continue;
        }
        // Add to Q
        m_TxQ.push_back(tx_id);

        // Forward upstream and get back response
        auto resp_pkt = Udp::SendAndReceive(pkt, ip, port);

        m_Server.SendTo(resp_pkt, pkt.GetSaddr6());
    }
    std::cout << "Finished run\n";
}

