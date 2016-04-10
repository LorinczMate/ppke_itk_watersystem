#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H

void initLayer(char myAddress);
void sendNetworkBuildDLPacket(char messageType, char parentnode, char distance, char myAddress,  char payloadLength, char *payload);


#endif
