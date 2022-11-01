// LOCAL INCLUDES
#include "ServerApp.h"
#include "Utils.h"

// SYSTEM INCLUDES
#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>

namespace
{
    constexpr uint8_t _MAX_THREADS = 10;
    constexpr uint32_t _MAX_TXID_SIZE = 1000;
}  // namespace

bool DnsFwd::ServerApp::m_Terminate = false;

DnsFwd::ServerApp::ServerApp(const std::string& a_Ip4, const std::string& a_Ip6,
                             uint16_t a_Port)
    : m_Server(a_Ip4, a_Ip6, a_Port)
{
}

DnsFwd::ServerApp::~ServerApp()
{
}

void
DnsFwd::ServerApp::Terminate(int a_Signal)
{
    PRINTER("Received signal " << a_Signal << "\n");
    m_Terminate = true;
}

void
DnsFwd::ServerApp::InsertToQ(DnsPacket&& a_Pkt)
{
    {
        std::unique_lock lk(m_QMut);
        m_PktQ.emplace(std::move(a_Pkt));
    }
    // Signal worker thread to process Q
    m_QCond.notify_one();
}

void
DnsFwd::ServerApp::Worker()
{
    while (!m_Terminate)
    {
        std::unique_lock lk(m_QMut);
        m_QCond.wait(lk);
        if (m_Terminate)
        {
            break;
        }

        // Process Q
        if (!m_PktQ.empty())
        {
            // Q not empty
            DnsPacket pkt = std::move(m_PktQ.front());
            m_PktQ.pop();
            auto tx_id = pkt.GetTxId();
            // Find in reverse since its more likely that the duplicate request
            // was recently processed
            if (std::find(m_TxQ.rbegin(), m_TxQ.rend(), tx_id) != m_TxQ.rend())
            {
                PRINTER("Duplicate request. Dropping: " << tx_id << "\n");
                continue;
            }
            // Add to Transaction ID queue
            m_TxQ.push_back(tx_id);
            // Cleanup old entries (Least recently used)
            // Can also make this into a separate thread to cleanup based on a
            // time condition.
            if (m_TxQ.size() == _MAX_TXID_SIZE)
            {
                m_TxQ.pop_front();
            }

            // Sending and receiving can now work in parallel. Releasing lock
            lk.unlock();

            // Forward packet upstream and get back response
            auto resp_pkt =
                Udp::SendAndReceive(pkt, m_UpstreamIp, m_UpstreamPort);
            if (!resp_pkt.IsEmpty())
            {
                // Send response back to originator
                m_Server.SendTo(resp_pkt, pkt.GetSaddr());
            }
        }
    }
}

void
DnsFwd::ServerApp::Run(uint32_t a_Ip, uint16_t a_Port)
{
    // Create listen socket and bind to port
    if (!m_Server.CreateAndBind())
    {
        return;
    }

    m_UpstreamIp = a_Ip;
    m_UpstreamPort = a_Port;

    // Start the receiver function as a separate thread
    // Recv() function adds incoming packets to a queue to be processed
    std::thread recv_t(&DnsFwd::Udp::Server::Recv, &m_Server,
                       [this](Packet&& pkt) { InsertToQ(std::move(pkt)); });

    // Worker thread pool to process requests
    std::thread worker_t[_MAX_THREADS];
    for (int i = 0; i < _MAX_THREADS; ++i)
    {
        worker_t[i] = std::thread(&DnsFwd::ServerApp::Worker, this);
    }

    // Check every second if it is terminated so that we can signal the workers
    while (!m_Terminate)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    m_Server.Terminate();
    m_QCond.notify_all();

    recv_t.join();
    for (int i = 0; i < _MAX_THREADS; ++i)
    {
        worker_t[i].join();
    }
    PRINTER("Finished run\n");
}

