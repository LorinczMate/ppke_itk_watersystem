#include "DataLinkLayer.h"
#include "utility.h"
#include "PhysicLayer.h"
#include "UART.h"
#include "include.h"

char myAddress;
char myDistance= 9;
char const MEASUREMENTDATATYPE = 0;
char const NETWORKBUILDDATATYPE = 1;
char const BROADCASTPACKET = 0;
char to;
char from;
char messageType;
char distance;
char parentnode;
char source;
char rssi[20];
char measurement[10];
char rssilength;
char measurementlength = 10;

void initLayer(char _myAddress){
   myAddress = _myAddress;
}

void receiveDLPacket(char length, char *payload, char rssi){
   to   = payload[0];
   from = payload[1];
   messageType = payload[2];
   distance = payload[3];

   if (to == myAddress || to == BROADCASTPACKET){
     // payload[length]=0;
      if ((messageType == NETWORKBUILDDATATYPE) && (myDistance > (distance + 1)) ){
    	  receiveNetworkBuildDLLPacketFromSerialPort(payload);
    	  myDistance = distance + 1;
    	  parentnode=from;
		  from = myAddress;
		  distance = myDistance;
		  sendNetworkBuildDLPacket(NETWORKBUILDDATATYPE, distance, from, payload);
		  TURN_ON_BOTH_LED;
      } else if ((messageType == MEASUREMENTDATATYPE) && (to == myAddress)) {
			parentnode = payload[4];
			source = payload[5];
			from = myAddress;
			to = parentnode;
			receiveMeasurementDLLPacketFromSerialPort(length, payload);
			//így ebben a legutolsó RSSI még nem kerül bele!
			arrayShiftRight(length-6, payload+6, rssi);
			length++;
			sendMeasurementDLPacket(MEASUREMENTDATATYPE, to, from, length-6, payload+6);
      }
   }
}


void sendNetworkBuildDLPacket(char messageType, char distance, char from, char *payload){ //címzett, üzenet hossza + az üzenet.
	/*--------------------------
	A CSOMAG FELÉPÍTÉSE:
	BROADCASTPACKET|myAddress|messageType|distance|
	*/
	int payloadLength = 0;
   arrayShiftRight(payloadLength++, payload, distance);
   arrayShiftRight(payloadLength++, payload, messageType);
   arrayShiftRight(payloadLength++, payload, myAddress);
   arrayShiftRight(payloadLength++, payload, BROADCASTPACKET);
   sendNetworkPacketToSerialPort(payload);
   sendPPacket(payloadLength, payload);
}

void sendMeasurementDLPacket(char messageType, char to, char from, char payloadLength, char *payload){
	for(int i=0; i<rssilength;i++){arrayShiftRight(payloadLength++, payload, rssi);}
	memcpy(rssi, payload, rssilength);
	for(int i=0; i<measurementlength;i++){arrayShiftRight(payloadLength++, payload, measurement);}
   	memcpy(measurement, payload, measurementlength);
	arrayShiftRight(payloadLength++, payload, source);
	arrayShiftRight(payloadLength++, payload, parentnode);
	arrayShiftRight(payloadLength++, payload, distance);
	arrayShiftRight(payloadLength++, payload, messageType);
	arrayShiftRight(payloadLength++, payload, from);
	arrayShiftRight(payloadLength++, payload, to);
	sendPPacket(payloadLength, payload);

}

void sendMyMeasurementDLPacket(char messageType, char parentnode, char source, char distance, char *measurementData, char payloadLength, char *payload){
	TURN_ON_RED_LED;

	/*--------------------------
	A CSOMAG FELÉPÍTÉSE:
	parentnode|myAddress|messageType|distance|parentnode|myAddress|measurementData|rssi

	HB_DLLayerben kapott csomag:
	to|from|messagetype|distance|parentnode|source|measurementData|rssi
	----------------------------*/

	for(int i=0; i<rssilength;i++){arrayShiftRight(payloadLength++, payload, rssi);}
	memcpy(payload, rssi, rssilength);
	for(int i=0; i<measurementlength;i++){arrayShiftRight(payloadLength++, payload, measurementData);}
   	memcpy(payload, measurementData, measurementlength);
	arrayShiftRight(payloadLength++, payload, myAddress);
	arrayShiftRight(payloadLength++, payload, parentnode);
	arrayShiftRight(payloadLength++, payload, distance);
	arrayShiftRight(payloadLength++, payload, messageType);
	arrayShiftRight(payloadLength++, payload, myAddress);
	arrayShiftRight(payloadLength++, payload, parentnode);
	sendMyMeasurementToSerialPort(payload, measurementData, rssi);
	sendPPacket(payloadLength, payload);

}



char getParentNode(){
	return parentnode;
}

char getDistance(){
	return distance;
}

void sendNetworkPacketToSerialPort(char *buffer){
   LINE_BREAK;
   sendString("MeasurementNode DataLinkLayer tovabbkuldte a NetworkBuild-et!");
   LINE_BREAK;
   sendString("BROADCASTPACKET|myAddress|messageType|distance|");
   LINE_BREAK;
   sendString("DLL payload: ");
   LINE_BREAK;
   sendChar(buffer[0]+'0');
   sendChar(buffer[1]+'0');
   sendChar(buffer[2]+'0');
   sendChar(buffer[3]+'0');
   LINE_BREAK;
}

void sendMyMeasurementToSerialPort(char *buffer, char *measurementData, char *rssi){


	// bug result: measurementData: 1530
	// rssi : 530 - és statikusan az is marad.

	sendString("--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("MeasurementNode DataLinkLayer elkulte a MyMeasurementPacket-et!");
	LINE_BREAK;
	sendString("to|from|messagetype|distance|parentnode|source|measurementData|rssi");
	LINE_BREAK;
	sendString("DLL payload: ");
	LINE_BREAK;
	sendChar(buffer[0]+'0');
	sendChar(buffer[1]+'0');
	sendChar(buffer[2]+'0');
	sendChar(buffer[3]+'0');
	sendChar(buffer[4]+'0');
	sendChar(buffer[5]+'0');
	LINE_BREAK;
	sendString(measurementData);
	LINE_BREAK;
	sendString(rssi);
	LINE_BREAK;

	sendString("To: ");
	sendChar(buffer[0]+'0');
	LINE_BREAK;
	sendString("From: ");
	sendChar(buffer[1]+'0');
	LINE_BREAK;
	sendString("Message Type (1 - NETWORKBUILDPACKET; 2 - MEASUREMENTPACKET): ");
	sendChar(buffer[2]+'0');
	LINE_BREAK;
	sendString("Distance: ");
	sendChar(buffer[3]+'0');
	LINE_BREAK;
	sendString("Parent Node: ");
	sendChar(buffer[4]+'0');
	LINE_BREAK;
	sendString("Source: ");
	sendChar(buffer[5]+'0');
	LINE_BREAK;
	sendString("Measurement Data: ");
	sendString(measurementData);
	LINE_BREAK;
	sendString("RSSI: ");
	sendString(rssi);
	LINE_BREAK;
	sendString("--------------------------------------------------------------------------------------------------");
	LINE_BREAK;


}

void receiveNetworkBuildDLLPacketFromSerialPort(char *buffer){
   sendString("--------------------------------------------------------------------------------------------------");
   LINE_BREAK;
   sendString("DataLinkLayer NetworkBuild uzenetet kapott!");
   LINE_BREAK;
   sendString("BROADCASTPACKET|myAddress|messageType|distance|");
   LINE_BREAK;
   sendString("DLL payload: ");
   LINE_BREAK;
   sendChar(buffer[0]+'0');
   sendChar(buffer[1]+'0');
   sendChar(buffer[2]+'0');
   sendChar(buffer[3]+'0');
   LINE_BREAK;

   sendString("BROADCASTPACKET: ");
   sendChar(buffer[0]+'0');
   LINE_BREAK;
   sendString("My Address: ");
   sendChar(buffer[1]+'0');
   LINE_BREAK;
   sendString("Message Type (1 - NETWORKBUILDPACKET; 2 - MEASUREMENTPACKET): ");
   sendChar(buffer[2]+'0');
   LINE_BREAK;
   sendString("Distance: ");
   sendChar(buffer[3]+'0');
   LINE_BREAK;
   sendString("--------------------------------------------------------------------------------------------------");
   LINE_BREAK;
}

void receiveMeasurementDLLPacketFromSerialPort(char length, char *buffer){
	sendString("--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("DataLinkLayer Measurement uzenetet kapott!");
	LINE_BREAK;
	sendString("to|from|messagetype|distance|parentnode|source|measurementData|rssi");
	LINE_BREAK;
	sendString("DLL payload: ");
	LINE_BREAK;
	sendChar(buffer[0]+'0');
	sendChar(buffer[1]+'0');
	sendChar(buffer[2]+'0');
	sendChar(buffer[3]+'0');
	sendChar(buffer[4]+'0');
	sendChar(buffer[5]+'0');

	memcpy(measurement, buffer + 6, 10);
	rssilength = length-16;
	memcpy(rssi, buffer+16, rssilength);

	sendString(measurement);
	sendString(rssi);
	char measurementToSerial;
	char rssiToSerial;
	itoa(rssi, rssiToSerial, 10);
	itoa(measurement, measurementToSerial, 10);

	sendString(measurementToSerial);
	sendString(rssiToSerial);

	sendString("To: ");
	sendChar(buffer[0]+'0');
	LINE_BREAK;
	sendString("From: ");
	sendChar(buffer[1]+'0');
	LINE_BREAK;
	sendString("Message Type (1 - NETWORKBUILDPACKET; 2 - MEASUREMENTPACKET): ");
	sendChar(buffer[2]+'0');
	LINE_BREAK;
	sendString("Distance: ");
	sendChar(buffer[3]+'0');
	LINE_BREAK;
	sendString("Parent Node: ");
	sendChar(buffer[4]+'0');
	LINE_BREAK;
	sendString("Source: ");
	sendChar(buffer[5]+'0');
	LINE_BREAK;
	sendString("Measurement Data: ");
	sendString(measurementToSerial);
	LINE_BREAK;
	sendString("RSSI: ");
	sendString(rssiToSerial);
	LINE_BREAK;
	sendString("--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
}
