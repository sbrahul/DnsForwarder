#ifndef _SERVER_APP_H_
#define _SERVER_APP_H_

namespace DnsFwd
{
    class ServerApp
    {
        public:
            // Statics
            static void Terminate(int signal);

            // Members
            void Run(uint32_t ip, uint16_t port);
    };
}

#endif // _SERVER_APP_H_
