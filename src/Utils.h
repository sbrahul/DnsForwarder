#ifndef _UTILS_H_
#define _UTILS_H_

// SYSTEM INCLUDES
#include <stdint.h>

// Forward Declarations
struct in6_addr;

namespace DnsFwd::Utils
{
    bool Ipv4ToNetwork(const char *ip, uint32_t* ipNetwork);
    bool Ipv6ToNetwork(const char *ip, struct in6_addr* ipNetwork);

    // Helper to deallocate resources for c functions like socket
    template <typename T, typename Q>
    class AutoDealloc
    {
      public:
        explicit AutoDealloc(const T& resource, Q callback)
            : m_Resource(resource),
              m_Callback(callback)
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
}


#endif // _UTILS_H_
