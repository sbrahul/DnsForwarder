// LOCAL INCLUDES
#include "Utils.h"

// SYSTEM INCLUDES
#include <arpa/inet.h>
#include <iostream>

bool
DnsFwd::Utils::Ipv4ToNetwork(const std::string& a_Ip, uint32_t* a_IpNetwork)
{
    if (a_Ip.empty())
    {
        PRINTER("IP empty\n");
        return false;
    }

    // extract IP
    if (inet_pton(AF_INET, a_Ip.c_str(), a_IpNetwork) != 1)
    {
        PRINTER_ERNO("Ip conversion failed for " << a_Ip);
        return false;
    }

    return true;
}

bool
DnsFwd::Utils::Ipv6ToNetwork(const std::string& a_Ip,
                             struct in6_addr* a_IpNetwork)
{
    if (a_Ip.empty())
    {
        PRINTER("IP empty\n");
        return false;
    }

    // extract IP
    if (inet_pton(AF_INET6, a_Ip.c_str(), a_IpNetwork) != 1)
    {
        PRINTER_ERNO("Ip6 conversion failed for " << a_Ip);
        return false;
    }

    return true;
}

