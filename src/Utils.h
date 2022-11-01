#ifndef _UTILS_H_
#define _UTILS_H_

// SYSTEM INCLUDES
#include <cstring>
#include <stdint.h>
#include <string>

// Forward Declarations
struct in6_addr;

namespace DnsFwd::Utils
{
    bool Ipv4ToNetwork(const std::string& a_Ip, uint32_t* a_IpNetwork);
    bool Ipv6ToNetwork(const std::string& a_Ip, struct in6_addr* a_IpNetwork);

#define PRINTER(stuff...)                                                  \
    {                                                                      \
        std::cout << "[" << __FILE__ << ":" << __func__ << "]\t" << stuff; \
    }

#define PRINTER_ERNO(stuff...)                                            \
    {                                                                     \
        std::cerr << "[" << __FILE__ << ":" << __func__ << "]\t" << stuff \
                  << ": " << strerror(errno) << "\n";                     \
    }

    // Helper to deallocate resources for c functions like socket
    template<typename T, typename Q>
    class AutoDealloc
    {
      public:
        explicit AutoDealloc(const T& a_Resource, Q a_Callback)
            : m_Resource(a_Resource),
              m_Callback(a_Callback)
        {
        }
        ~AutoDealloc()
        {
            m_Callback(m_Resource);
        }

        T& Get()
        {
            return const_cast<T&>(m_Resource);
        }

        // disable copy and move
        AutoDealloc() = delete;
        AutoDealloc(const AutoDealloc&) = delete;
        AutoDealloc& operator=(const AutoDealloc&) = delete;
        AutoDealloc(const AutoDealloc&&) = delete;
        AutoDealloc& operator=(const AutoDealloc&&) = delete;

      private:
        const T& m_Resource;
        Q m_Callback;
    };
}  // namespace DnsFwd::Utils

#endif  // _UTILS_H_

