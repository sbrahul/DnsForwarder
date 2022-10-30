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

DnsFwd::ServerApp::ServerApp(const char* a_Ip, uint16_t a_Port)
    : m_Server(a_Ip, a_Port)
{
}

DnsFwd::ServerApp::~ServerApp()
{
}

void
DnsFwd::ServerApp::Terminate(int a_Signal)
{
    std::cout << "Received signal " << a_Signal << "\n";
    m_Terminate = true;
}

void
DnsFwd::ServerApp::Run(uint32_t a_Ip, uint16_t a_Port)
{
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
        auto resp_pkt = Udp::SendAndReceive(pkt, a_Ip, a_Port);

        m_Server.SendTo(resp_pkt, pkt.GetSaddr6());
    }
    std::cout << "Finished run\n";
}
