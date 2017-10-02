#include "M_PhysicLayer.h"
#include "UART.h"
#include "M_include.h"
#include "M_DataLinkLayer.h"
#include "utility.h"

char rxBuffer[60];

// ISR for received packet
// The ISR assumes the int came from the pin attached to GDO0 and therefore
// does not check the other seven inputs.  Interprets this as a signal from
// CCxxxx indicating packet received.
__attribute__((interrupt(PORT2_VECTOR)))
void port2_ISR (void){
   unsigned char len=40;
   char status[2];
   if (RFReceivePacket(rxBuffer,&len, status)){
	   rxBuffer[len] = status[0];
	   len+=1;
      receivePPacket(len, rxBuffer, status[0]); //// ??? status[0] vagy a status[1] az RSSI? Adatlap
   } 
   P2IFG &= ~TI_CC_GDO0_PIN;
}


void receivePPacket(char length, char *payload, char rssi){
	TURN_ON_GREEN_LED;
	reveivePPacketFromSerialPort(length, payload);
	receiveDLPacket(length, payload, rssi);
}

/**
   length  : A payload hossza byte-ban
   payload : A payload, amit el akarunk küldeni
*/
void sendPPacket(char length, char *payload){
   arrayShiftRight(length, payload, length);
   RFSendPacket(payload, length+1);
   P1OUT ^= 0x03;
   LINE_BREAK;
   sendString("MeasurementNode PhysicLayer elkuldott egy uzenetet!");
   LINE_BREAK;
   sendString("PL payload: ");
   sendString(payload);
   LINE_BREAK;
}

void reveivePPacketFromSerialPort(char length, char *buffer){
	LINE_BREAK;
	sendString("MeasurementNode PhysicLayer uzenetet kapott!");
	DOUBLE_LINE_BREAK;
	if(buffer[2] == 1)
		receiveNetworkBuildPPacketFromSerialPort(length, buffer);
	else if (buffer[2] == 0)
		receiveMeasurementPPacketFromSerialPort(length, buffer);
}

void receiveNetworkBuildPPacketFromSerialPort(char length, char *buffer){
	sendString("--------------------------------------------------------------------------------------------------");
	   LINE_BREAK;
	   sendString("A Measurement PhysicalLayer NetworkBuild uzenetet kapott!");
	   LINE_BREAK;
	   sendString("BROADCASTPACKET|myAddress|messageType|distance|");
	   LINE_BREAK;
	   char lengthToSerial[5];
	   itoa(length, lengthToSerial, 10);
	   sendChar(buffer[0]+'0');
	   sendChar(buffer[1]+'0');
	   sendChar(buffer[2]+'0');
	   sendChar(buffer[3]+'0');
	   LINE_BREAK;
	   sendString("Length: ");
	   sendString(lengthToSerial);
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
	   sendString("RSSI: ");
	   char rssitoSerial[5];
	   itoa(buffer[4], rssitoSerial,10);
	   sendString(rssitoSerial);
	   LINE_BREAK;
	   sendString("--------------------------------------------------------------------------------------------------");
	   LINE_BREAK;
}

void receiveMeasurementPPacketFromSerialPort(char length, char *buffer){
	sendString("--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("DataLinkLayer Measurement uzenetet kapott!");
	LINE_BREAK;
	sendString("A Csomag felépítése: ");
	sendString("length|to|from|messagetype|distance|parentnode|source|measurementData|rssi");
	LINE_BREAK;
	char lengthToSerial[5];
	itoa(length, lengthToSerial, 10);
	sendChar(buffer[0]+'0');
	sendChar(buffer[1]+'0');
	sendChar(buffer[2]+'0');
	sendChar(buffer[3]+'0');
	sendChar(buffer[4]+'0');
	sendChar(buffer[5]+'0');
	char measurement[10];
	char rssi[20];
	char rssilength;
	memcpy(measurement, buffer + 6, 10);
	rssilength = length-16;
	memcpy(rssi, buffer+16, rssilength);

	sendString(measurement);
	sendString(rssi);
	DOUBLE_LINE_BREAK;

	sendString("Length: ");
	sendString(lengthToSerial);
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
	sendString(measurement);
	LINE_BREAK;
	sendString("RSSI: ");
	sendString(rssi);
	LINE_BREAK;
	sendString("--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
}
