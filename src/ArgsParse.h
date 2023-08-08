#ifndef _ARGS_PARSE_H_
#define _ARGS_PARSE_H_

#include <cstdint>
#include <tuple>

namespace DnsFwd::ArgsParse
{
    /*
     * Parse the cmd line args to get the upstream server IP and port
     * First element in return type is the IP address of upstream server in
     * network format. Second element is the port number in network format too.
     */
    std::tuple<uint32_t, uint16_t> Parse(int a_Argc, char** a_Argv);
}  // namespace DnsFwd::ArgsParse

#endif  // _ARGS_PARSE_H_
