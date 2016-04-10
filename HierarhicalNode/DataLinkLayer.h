#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H

void initLayer(char myAddress);
void sendNetworkBuildDLPacket(char messageType, char parentnode, char distance, char from,  char payloadLength, char *payload);
void sendMeasurementDLPacket(char messageType, char to, char from, char payloadLength, char *payload);
void sendMyMeasurementDLPacket(char messageType, char parentnode, char source, char distance, char payloadLength, char *payload);

#endif
