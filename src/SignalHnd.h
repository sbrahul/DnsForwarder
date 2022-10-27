#ifndef _SIGNAL_HND_H_
#define _SIGNAL_HND_H_

namespace DnsFwd::SignalHnd
{
    bool Register(int sig, void (*fn)(int));
}

#endif // _SIGNAL_HND_H_
