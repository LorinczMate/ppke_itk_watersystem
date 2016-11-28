#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H

void initLayer(char myAddress);
void sendNetworkBuildDLPacket(char messageType, char distance, char myAddress, char *payload);


#endif
