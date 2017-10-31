#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H

void initLayer(char myAddress);
void sendNetworkBuildDLLPacket(char messageType, char distance, char from, char *payload);
void sendMeasurementDLLPacket(char messageType, char to, char from, char payloadLength, char *payload);
void sendMyMeasurementDLPacket(char messageType, char parentnode, char source, char distance, char* measurementData, char payloadLength, char *payload);

#endif
