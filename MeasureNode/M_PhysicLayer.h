#ifndef PHYSICLAYER_H
#define PHYSICLAYER_H

void receivePPacket(char length, char *payload, char rssi);
void sendPPacket(char length, char *payload);

#endif
