#define VERBOSE
#include "BS_DataLinkLayer.h"
#include "utility.h"
#include "BS_PhysicLayer.h"
#include "UART.h"
#include <string.h>
#include "BS_include.h"

char myAddress;
char myDistance;
char const NETWORKBUILDDATATYPE = 1;
char const MEASUREMENTDATATYPE = 0;
char const BROADCASTPACKET = 0;
char to;
char from;
char messageType;
char distance;
char parentnode;
char source;
char measurementData[10];
//char rssi[20];
unsigned int rssilength;
char rssilengthtmp[10];
unsigned int measurementDataLength = 10;
char measurementDataLengthtmp[10];
char payloadlengthtmp[30];

int distancetmp;

void initLayer(char _myAddress) {
	myAddress = _myAddress;
}
/*
 * végig debugolni, hogy a to és a from hol csúszik el - itoaval együtt nézni
 * minden rétegben iratni hogy hol mit kap meg és mit küld el. - rssi-n bukik?
 */
void receiveDLPacket(char length, char *payload, char rssi) {
	to = payload[0];
	from = payload[1];
	messageType = payload[2];
	distance = payload[3];
	parentnode = payload[4];
	//receiveWhateverMessageInTheAirDLLPacetToSerialPort(length, payload, rssi);
#ifdef VERBOSE
	if (to == BROADCASTPACKET) {
		receiveBroadcastPacket(length, payload);
	}
#endif
	if (to == myAddress) {
		payload[length] = 0;
		if (messageType == MEASUREMENTDATATYPE) {
			source = payload[5];
			blinkLEDsForRecivigMeasurementPacket(from);
			receiveMeasurementDLLPacketToSerialPort(length, payload, rssi);
			TURN_OFF_RED_LED;
		}
	}
}

void blinkLEDsForTurningOnTheNode(){
	/*The BaseStation node's green LED will blink when one will turn it on as much as its own address.
	 *At the end the green led will stay on, which shows from this moment that the device is turned on.
	 * example: source address = 2 : _ | _ | _ | ->     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 */
	for (int i = 0; i < myAddress*2+1; i++) {
		__delay_cycles(500000);
		BLINK_GREEN_LED;
	}
}


void blinkLEDsForRecivigMeasurementPacket(char from) {
	/*The BaseStation node's red LED will blink when its receiving a measurement packet as much as the source node's address.
	 *At the end the red led will stay off.
	 * example: source address = 2 : _ | _ | _ ->     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 * @param		source	 The address of the node who did the measurement
	 */

	for (int i = 0; i < from * 2; i++) {
		__delay_cycles(500000);
		BLINK_RED_LED;
	}
}

void sendNetworkBuildDLLPacket(char messageType, char distance, char myAddress,
		char *buffer) {
	/*
	 A CSOMAG FELÉPÍTÉSE:
	 BROADCASTPACKET|myAddress|messageType|distance
	 */
	int payloadLength = 0;
	arrayShiftRight(payloadLength++, buffer, distance);
	arrayShiftRight(payloadLength++, buffer, messageType);
	arrayShiftRight(payloadLength++, buffer, myAddress);
	arrayShiftRight(payloadLength++, buffer, 2); //broadcastpacket
	TURN_ON_RED_LED;
#ifdef VERBOSE
	sendNetworkPacketToSerialPort(buffer);
#endif
	sendPPacket(payloadLength, buffer);
	__delay_cycles(1000000);
	TURN_OFF_RED_LED;
}

void sendNetworkPacketToSerialPort(char *buffer) {
	LINE_BREAK;
	sendString(
			"BaseStationNode DataLinkLayer elkulte a NetworkBuildPacket-et!");
	LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("Csomag felépítése: ");
	LINE_BREAK;
	sendString("BROADCASTPACKET|myAddress|messageType|distance|");
	LINE_BREAK;
	sendString("DLL payload: ");
	LINE_BREAK;
	sendChar(buffer[0] + '0');
	sendChar(buffer[1] + '0');
	sendChar(buffer[2] + '0');
	sendChar(buffer[3] + '0');
	LINE_BREAK;

	sendString("BROADCASTPACKET: ");
	sendChar(buffer[0] + '0');
	LINE_BREAK;
	sendString("My Address: ");
	sendChar(buffer[1] + '0');
	LINE_BREAK;
	sendString(
			"Message Type (1 - NETWORKBUILDPACKET; 0 - MEASUREMENTPACKET): ");
	sendChar(buffer[2] + '0');
	LINE_BREAK;
	sendString("Distance: ");
	sendChar(buffer[3] + '0');
	LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
}

void receiveWhateverMessageInTheAirDLLPacetToSerialPort(char length,
		char *buffer, unsigned char rssi) {
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	LINE_BREAK;
	sendString(
			"                                       WHATEVER MESSAGE                                           ");
	DOUBLE_LINE_BREAK;
	sendString("A BaseStationDataLinkLayer ELKAPOTT EGY RANDOM UZENETET!!!");
	LINE_BREAK;
	sendString("to|from|messagetype|distance|parentnode|source|");
	LINE_BREAK;
	sendString("DLL payload: ");
	LINE_BREAK;
	sendChar(buffer[0] + '0');
	sendChar(buffer[1] + '0');
	sendChar(buffer[2] + '0');
	sendChar(buffer[3] + '0');
	sendChar(buffer[4] + '0');
	sendChar(buffer[5] + '0');
	DOUBLE_LINE_BREAK;

	sendString("Length: ");
	sendChar(length + '0');
	DOUBLE_LINE_BREAK;

	sendString("To: ");
	sendChar(buffer[0] + '0');
	sendString(" | ");
	sendString("From: ");
	sendChar(buffer[1] + '0');
	sendString(" | ");
	sendString("Message Type: ");
	sendChar(buffer[2] + '0');
	sendString(" | ");
	sendString("Distance: ");
	sendChar(buffer[3] + '0');
	sendString(" | ");
	sendString("Parent Node: ");
	sendChar(buffer[4] + '0');
	sendString(" | ");
	sendString("Source: ");
	sendChar(buffer[5] + '0');
	sendString(" | ");
	DOUBLE_LINE_BREAK;
	sendString(
			"                                       END OF THE WHATEVER MESSAGE                                ");
	LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
}
void receiveMeasurementDLLPacketToSerialPort(char length, char *buffer,
		unsigned char rssi) {
	memcpy(measurementData, buffer + 6, 4);
	measurementData[4] = 0;
	char rssiToSerial[5];
	itoa(rssi, rssiToSerial, 10);
#ifdef VERBOSE
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("A BaseStationDataLinkLayer mérési uzenetet kapott!");
	LINE_BREAK;
	sendString(
			"to|from|messagetype|distance|parentnode|source|measurementData|rssi");
	LINE_BREAK;
	sendString("DLL payload: ");
	LINE_BREAK;
	sendChar(buffer[0] + '0');
	sendChar(buffer[1] + '0');
	sendChar(buffer[2] + '0');
	sendChar(buffer[3] + '0');
	sendChar(buffer[4] + '0');
	sendChar(buffer[5] + '0');
	sendString(measurementData);
	sendString(rssiToSerial);
	DOUBLE_LINE_BREAK;

	sendString("To: ");
	sendChar(buffer[0] + '0');
	sendString(" | ");
	sendString("From: ");
	sendChar(buffer[1] + '0');
	sendString(" | ");
	sendString("Message Type: ");
	sendChar(buffer[2] + '0');
	sendString(" | ");
	sendString("Distance: ");
	sendChar(buffer[3] + '0');
	sendString(" | ");
	sendString("Parent Node: ");
	sendChar(buffer[4] + '0');
	sendString(" | ");
	sendString("Source: ");
	sendChar(buffer[5] + '0');
	sendString(" | ");
	sendString("Measurement Data: ");
	sendString(measurementData);
	sendString(" | ");
	sendString("RSSI: ");
	sendString(rssiToSerial);
	/*for(int i=0;i<distancetmp;i++){
	 itoa(rssi[i],rssilengthtmp,10);
	 sendString(rssilengthtmp);
	 sendString(" | ");
	 }*/LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendChar(buffer[0] + '0');
	sendString(",");
	sendChar(buffer[1] + '0');
	sendString(",");
	sendChar(buffer[2] + '0');
	sendString(",");
	sendChar(buffer[3] + '0');
	sendString(",");
	sendChar(buffer[4] + '0');
	sendString(",");
	sendChar(buffer[5] + '0');
	sendString(",");
	sendString(measurementData);
	sendString(",");
	sendString(rssiToSerial);
	DOUBLE_LINE_BREAK;

#else
	sendChar(buffer[0]+'0');
	sendString(",");
	sendChar(buffer[1]+'0');
	sendString(",");
	sendChar(buffer[2]+'0');
	sendString(",");
	sendChar(buffer[3]+'0');
	sendString(",");
	sendChar(buffer[4]+'0');
	sendString(",");
	sendChar(buffer[5]+'0');
	sendString(",");
	sendString(measurementData);
	sendString(",");
	sendString(rssiToSerial);
#endif
}

void receiveBroadcastPacket(char length, char *buffer) {
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("A BaseStationDataLinkLayer broadcast uzenetet kapott!");
	LINE_BREAK;
	sendString("BROADCASTPACKET|myAddress|messageType|distance|");
	LINE_BREAK;

	to = buffer[0];
	from = buffer[1];
	messageType = buffer[2];
	distance = buffer[3];
	parentnode = buffer[4];

	sendChar(buffer[0] + '0');
	sendChar(buffer[1] + '0');
	sendChar(buffer[2] + '0');
	sendChar(buffer[3] + '0');
	sendChar(buffer[4] + '0');
	sendChar(buffer[5] + '0');

	DOUBLE_LINE_BREAK;

	sendString("Length: ");
	sendChar(length + '0');
	LINE_BREAK;
	sendString("To: ");
	sendChar(buffer[0] + '0');
	sendString(" | ");
	sendString("From: ");
	sendChar(buffer[1] + '0');
	sendString(" | ");
	sendString("Message Type: ");
	sendChar(buffer[2] + '0');
	sendString(" | ");
	sendString("Distance: ");
	sendChar(buffer[3] + '0');
	sendString(" | ");
	sendString("Parent Node: ");
	sendChar(buffer[4] + '0');
	sendString(" | ");
	LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;

}

