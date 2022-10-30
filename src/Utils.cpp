// LOCAL INCLUDES
#include "Utils.h"

// SYSTEM INCLUDES
#include <arpa/inet.h>
#include <iostream>

bool
DnsFwd::Utils::Ipv4ToNetwork(const char* a_Ip, uint32_t* a_IpNetwork)
{
    if (nullptr == a_Ip)
    {
        return false;
    }

    // extract IP
    if (inet_pton(AF_INET, a_Ip, a_IpNetwork) != 1)
    {
        PRINTER_ERNO("Ip conversion failed for " << a_Ip);
        return false;
    }

    return true;
}

bool
DnsFwd::Utils::Ipv6ToNetwork(const char* a_Ip, struct in6_addr* a_IpNetwork)
{
    if (nullptr == a_Ip)
    {
        return false;
    }

    // extract IP
    if (inet_pton(AF_INET6, a_Ip, a_IpNetwork) != 1)
    {
        PRINTER_ERNO("Ip6 conversion failed for " << a_Ip);
        return false;
    }

    return true;
}
