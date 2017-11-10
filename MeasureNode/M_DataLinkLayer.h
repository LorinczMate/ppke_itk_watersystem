#ifndef DATALINKLAYER_H
#define DATALINKLAYER_H

#define UNDEDINED_PARENT_NODE 255

void putADCInBuffer(char *buf);
int getADC();
void initLayer(char myAddress);
void sendNetworkBuildDLLPacket(char messageType, char distance, char from, char *payload);
void sendBullshit(char payloadLength, char *payload);
void sendMeasurementDLLPacket(char messageType, char to, char from, char payloadLength, char *payload, unsigned char rssi);
void sendMyMeasurementDLPacket(char messageType, char parentnode, char source, char distance, char* measurementData, char payloadLength, char *payload);

#endif
