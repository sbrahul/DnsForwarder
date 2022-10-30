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

DnsFwd::Udp::Server::Server(const char* a_Ip, uint16_t a_Port)
    : m_ListenPort(a_Port),
      m_ListenIp(a_Ip),
      m_RecvBuffer(new uint8_t[MAX_BUF_SIZE])
{
}

DnsFwd::Udp::Server::~Server()
{
    if (-1 != m_ServFd)
    {
        close(m_ServFd);
    }
}

bool
DnsFwd::Udp::Server::CreateAndBind()
{
    //    uint32_t ip = 0;
    //    if (!DnsFwd::Utils::Ipv4ToNetwork(m_ListenIp, &ip))
    //    {
    //        return false;
    //    }

    struct in6_addr ip6 = {0};
    if (!DnsFwd::Utils::Ipv6ToNetwork(m_ListenIp, &ip6))
    {
        return false;
    }

    m_ServFd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (-1 == m_ServFd)
    {
        perror("Failed to create server socket");
        return false;
    }

    // dual stack is supported by default on Linux. Set again just in case its
    // switch off manually
    int off = 0;
    setsockopt(m_ServFd, IPPROTO_IPV6, IPV6_V6ONLY, &off, sizeof(off));

    struct sockaddr_in6 saddr;
    memset(&saddr, 0, sizeof saddr);

    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = ip6;
    saddr.sin6_port = htons(m_ListenPort);

    if (0 != bind(m_ServFd, (struct sockaddr*) &saddr, sizeof saddr))
    {
        perror("Bind failed for server port");
        return false;
    }

    return true;
}

DnsFwd::Packet
DnsFwd::Udp::Server::Recv()
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(m_ServFd, &fds);
    // select can modify timeout val. So, take a copy.
    struct timeval tout = m_Timeout;
    int s_ret = select(m_ServFd + 1, &fds, NULL, NULL, &tout);
    if (-1 == s_ret)
    {
        perror("Select call fail");
        return Packet();
    }
    else if (0 == s_ret)
    {
        // either timed out or interrupt
        return Packet();
    }
    // not performing sanity checks on args
    struct sockaddr_in6 saddr = {0};
    socklen_t saddr_size = sizeof(saddr);
    memset(m_RecvBuffer.get(), 0, MAX_BUF_SIZE);
    int bytes = recvfrom(m_ServFd, m_RecvBuffer.get(), MAX_BUF_SIZE, 0,
                         (struct sockaddr*) &saddr, &saddr_size);
    if (bytes < 1)
    {
        perror("Failed to receive data");
        return Packet();
    }
    else if (0 == bytes)
    {
        return Packet();
    }

    // Valid data
    return Packet(m_RecvBuffer.get(), bytes, saddr);
}

void
DnsFwd::Udp::Server::SendTo(const DnsFwd::Packet& a_Pkt,
                            struct sockaddr_in6 a_Saddr)
{
    if (-1 == sendto(m_ServFd, a_Pkt.Get(), a_Pkt.Size(), 0,
                     (struct sockaddr*) &a_Saddr, sizeof a_Saddr))
    {
        perror("sending packet failed");
    }
}

DnsFwd::Packet
DnsFwd::Udp::SendAndReceive(const DnsFwd::Packet& a_Pkt, uint32_t a_Ip,
                            uint16_t a_Port)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd)
    {
        perror("Failed to create send socket");
        return Packet();
    }
    DnsFwd::Utils::AutoDealloc<int, decltype(&close)> fd_ad(fd, &close);

    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = a_Ip;
    saddr.sin_port = htons(a_Port);

    if (-1 == sendto(fd, a_Pkt.Get(), a_Pkt.Size(), 0,
                     (struct sockaddr*) &saddr, sizeof saddr))
    {
        perror("sending upstream failed");
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
        perror("Select call fail");
        return Packet();
    }
    std::unique_ptr<uint8_t[]> recv_buf(new uint8_t[Server::MAX_BUF_SIZE]);
    socklen_t saddr_size = sizeof saddr;
    int bytes = recvfrom(fd, recv_buf.get(), Server::MAX_BUF_SIZE, 0,
                         (struct sockaddr*) &saddr, &saddr_size);
    if (bytes < 1)
    {
        perror("Failed to receive data");
        return Packet();
    }
    else if (0 == bytes)
    {
        return Packet();
    }

    // Valid data
    return Packet(recv_buf.get(), bytes, saddr);
}
