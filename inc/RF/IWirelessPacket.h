#ifndef LIBSPINE_COMMON_WIRELESS_PACKET_H_
#define LIBSPINE_COMMON_WIRELESS_PACKET_H_

#include "Utility.h"

INTERFACE(IWirelessPacket) {
public:
  uint16_t * begin() { return STATIC_DISPATCH(begin()); }
  uint16_t * end() { return STATIC_DISPATCH(end()); }
private:
};

#endif

