#include "M_DataLinkLayer.h"
#include "utility.h"
#include "M_PhysicLayer.h"
#include "UART.h"
#include "M_include.h"

char myAddress;
char myDistance = 9;
char const MEASUREMENTDATATYPE = 0;
char const NETWORKBUILDDATATYPE = 1;
char const BROADCASTPACKET = 0;
char to;
char from;
char messageType;
char distance;
char parentnode = UNDEDINED_PARENT_NODE;
char source;
char measurement[5];
char rssilength = 3;
char measurementlength = 4;

void initLayer(char _myAddress) {
	myAddress = _myAddress;
}

int getADC() {
	ADC10CTL0 |= ENC + ADC10SC;	// Sampling and conversion start
	while (!(ADC10CTL0 & ADC10IFG))
		__nop();
	return ADC10MEM;// Assigns the value held in ADC10MEM to the integer called ADC_value
}

void putADCInBuffer(char *buf) {
	itoa(getADC(), buf, 10);
}

void receiveDLPacket(char length, char *payload, char rssi) {
	to = payload[0];
	from = payload[1];
	messageType = payload[2];
	distance = payload[3];

	if (to == myAddress || to == BROADCASTPACKET) {
		// payload[length]=0;
		if ((messageType == NETWORKBUILDDATATYPE)
				&& (myDistance > (distance + 1))) {
			sendString(
					"I gona change my parameters cause i got a better offer from: ");
			sendChar(payload[1] + '0');
			LINE_BREAK;
			sendString("My previous distance was: ");
			sendChar(myDistance + '0');
			myDistance = distance + 1;
			LINE_BREAK;
			sendString("... and my new distance is: ");
			sendChar(myDistance + '0');
			parentnode = from;
			from = myAddress;
			distance = myDistance;
		}
	}
}



void sendMyMeasurementDLPacket(char payloadLength, char *payload) {
	sendPPacket(payloadLength, payload);

}


