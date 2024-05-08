#ifndef __NET_NETWORK_ADDRESS_H__
#define __NET_NETWORK_ADDRESS_H__

namespace ZDC {
    namespace NET {
        enum class NetAddrType {
            IPV4 = 0,
            IPV6 = 1,
        };

        typedef union NetAddrIPV4 {
            unsigned int AsNumber;
            unsigned char AsArray[4];
        } NetAddrIPV4_t;

        typedef union NetAddrIPV6 {
            unsigned long long int AsNumber;
            unsigned short AsArray[4];
        } NetAddrIPV6_t;

        typedef struct NetAddr {
            int type;
            union {
                NetAddrIPV4_t ipv4;
                NetAddrIPV6_t ipv6;
            };
        } NetAddr_t;
    }
}
#endif//__NET_NETWORK_ADDRESS_H__ GUARD