// LOCAL INCLUDES
#include "Udp.h"
#include "Utils.h"

// SYSTEM INCLUDES
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

DnsFwd::Udp::Server::Server(const std::string& a_Ip4, const std::string& a_Ip6,
                            uint16_t a_Port)
    : m_ListenPort(a_Port),
      m_ListenIp4(a_Ip4),
      m_ListenIp6(a_Ip6),
      m_RecvBuffer(new uint8_t[MAX_BUF_SIZE])
{
}

DnsFwd::Udp::Server::~Server()
{
    if (-1 != m_ServFd4)
    {
        close(m_ServFd4);
    }
    if (-1 != m_ServFd6)
    {
        close(m_ServFd6);
    }
}

bool
DnsFwd::Udp::Server::CreateAndBind()
{
    // Make sure at least one IP is set
    if (m_ListenIp4.empty() && m_ListenIp6.empty())
    {
        PRINTER("No listen IP set\n");
        return false;
    }

    if (!m_ListenIp4.empty())
    {
        uint32_t ip4 = 0;
        if (!DnsFwd::Utils::Ipv4ToNetwork(m_ListenIp4, &ip4))
        {
            return false;
        }
        m_ServFd4 = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == m_ServFd4)
        {
            PRINTER_ERNO("Failed to create v4 server socket");
            return false;
        }

        struct sockaddr_in saddr;
        memset(&saddr, 0, sizeof saddr);

        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = ip4;
        saddr.sin_port = htons(m_ListenPort);

        if (0 != bind(m_ServFd4, (struct sockaddr*) &saddr, sizeof saddr))
        {
            PRINTER_ERNO("Bind failed for v4 server port");
            return false;
        }
    }

    if (!m_ListenIp6.empty())
    {
        struct in6_addr ip6 = {0};
        if (!DnsFwd::Utils::Ipv6ToNetwork(m_ListenIp6, &ip6))
        {
            return false;
        }

        m_ServFd6 = socket(AF_INET6, SOCK_DGRAM, 0);
        if (-1 == m_ServFd6)
        {
            PRINTER_ERNO("Failed to create v6 server socket");
            return false;
        }

        // dual stack is supported by default on Linux. Set again just in case
        // its switch off manually
        // int off = 0;
        // setsockopt(m_ServFd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off));

        struct sockaddr_in6 saddr;
        memset(&saddr, 0, sizeof saddr);

        saddr.sin6_family = AF_INET6;
        saddr.sin6_addr = ip6;
        saddr.sin6_port = htons(m_ListenPort);

        if (0 != bind(m_ServFd6, (struct sockaddr*) &saddr, sizeof saddr))
        {
            PRINTER_ERNO("Bind failed for v6 server port");
            return false;
        }
    }

    return true;
}

void
DnsFwd::Udp::Server::Recv(std::function<void(DnsFwd::Packet)> a_Inserter)
{
    while (!m_Terminate)
    {
        fd_set fds;
        FD_ZERO(&fds);
        if (-1 != m_ServFd4)
        {
            FD_SET(m_ServFd4, &fds);
        }
        if (-1 != m_ServFd6)
        {
            FD_SET(m_ServFd6, &fds);
        }
        int max_fd = std::max(m_ServFd4, m_ServFd6) + 1;
        // select can modify timeout val. So, take a copy.
        struct timeval tout = m_Timeout;
        int s_ret = select(max_fd, &fds, NULL, NULL, &tout);
        if (-1 == s_ret)
        {
            PRINTER_ERNO("Select call fail");
            return;
        }
        else if (0 == s_ret)
        {
            // either timed out or interrupt
            continue;
        }

        struct sockaddr_storage saddr;
        memset(&saddr, 0, sizeof saddr);
        memset(m_RecvBuffer.get(), 0, MAX_BUF_SIZE);
        if (FD_ISSET(m_ServFd4, &fds))
        {
            // not performing sanity checks on args
            socklen_t saddr_size = sizeof(struct sockaddr_in);
            int bytes = recvfrom(m_ServFd4, m_RecvBuffer.get(), MAX_BUF_SIZE, 0,
                                 (struct sockaddr*) &saddr, &saddr_size);
            if (bytes < 1)
            {
                PRINTER_ERNO("Failed to receive data");
                continue;
            }
            else if (0 == bytes)
            {
                continue;
            }

            // Valid data
            a_Inserter(Packet(m_RecvBuffer.get(), bytes, saddr));
        }
        if (FD_ISSET(m_ServFd6, &fds))
        {
            // not performing sanity checks on args
            socklen_t saddr_size = sizeof(struct sockaddr_in6);
            int bytes = recvfrom(m_ServFd6, m_RecvBuffer.get(), MAX_BUF_SIZE, 0,
                                 (struct sockaddr*) &saddr, &saddr_size);
            if (bytes < 1)
            {
                PRINTER_ERNO("Failed to receive data");
                continue;
            }
            else if (0 == bytes)
            {
                continue;
            }

            // Valid data
            a_Inserter(Packet(m_RecvBuffer.get(), bytes, saddr));
        }
    }
}

void
DnsFwd::Udp::Server::SendTo(const DnsFwd::Packet& a_Pkt,
                            struct sockaddr_storage a_Saddr)
{
    int fd = m_ServFd4;
    if (a_Saddr.ss_family == AF_INET6)
    {
        fd = m_ServFd6;
    }
    if (-1 == sendto(fd, a_Pkt.Get(), a_Pkt.Size(), 0,
                     (struct sockaddr*) &a_Saddr, sizeof a_Saddr))
    {
        PRINTER_ERNO("sending packet failed");
    }
}

DnsFwd::Packet
DnsFwd::Udp::SendAndReceive(const DnsFwd::Packet& a_Pkt, uint32_t a_Ip,
                            uint16_t a_Port)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd)
    {
        PRINTER_ERNO("Failed to create send socket");
        return Packet();
    }
    DnsFwd::Utils::AutoDealloc<int, decltype(&close)> fd_ad(fd, &close);

    struct sockaddr_storage s_saddr;
    memset(&s_saddr, 0, sizeof s_saddr);
    struct sockaddr_in* saddr = (struct sockaddr_in*) &s_saddr;
    saddr->sin_family = AF_INET;
    saddr->sin_addr.s_addr = a_Ip;
    saddr->sin_port = htons(a_Port);

    if (-1 == sendto(fd, a_Pkt.Get(), a_Pkt.Size(), 0, (struct sockaddr*) saddr,
                     sizeof(*saddr)))
    {
        PRINTER_ERNO("sending upstream failed");
        return Packet();
    }

    // Get response
    // TODO: Code is common to Recv. Refactor
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    // Have a 5 second timeout
    struct timeval tout = {5, 0};
    int s_ret = select(fd + 1, &fds, NULL, NULL, &tout);
    if (-1 == s_ret)
    {
        PRINTER_ERNO("Select call fail");
        return Packet();
    }
    else if (0 == s_ret)
    {
        // either timed out or interrupt
        return Packet();
    }
    std::unique_ptr<uint8_t[]> recv_buf(new uint8_t[Server::MAX_BUF_SIZE]);
    socklen_t saddr_size = sizeof saddr;
    int bytes = recvfrom(fd, recv_buf.get(), Server::MAX_BUF_SIZE, 0,
                         (struct sockaddr*) &saddr, &saddr_size);
    if (bytes < 1)
    {
        PRINTER_ERNO("Failed to receive data");
        return Packet();
    }
    else if (0 == bytes)
    {
        return Packet();
    }

    // Valid data
    return Packet(recv_buf.get(), bytes, s_saddr);
}
