#include "PhysicLayer.h"

#include "include.h"
#include "DataLinkLayer.h"
#include "utility.h"

char rxBuffer[40];
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
   // Lehetne ide CRC kód ellenőrzést tenni, de most ezt hagyjuk
   // Egyszerűen csak továbbítjuk a felső rétegnek
	LINE_BREAK;
   receiveDLPacket(length, payload, rssi);
}

/**
   length  : A payload hossza byte-ban
   payload : A payload, amit el akarunk küldeni
*/
void sendPPacket(char length, char *payload){
   arrayShiftRight(length, payload, length);
   sendPPacketToSerialPort(payload);
   RFSendPacket(payload, length+1);
}

void sendPPacketToSerialPort(char *buffer){
   LINE_BREAK;
   sendString("BaseStationNode PhysicLayer elkulte a NetworkBuildPacket-et!");
   LINE_BREAK;
   TURN_ON_RED_LED;
   char length[5];

   itoa(buffer[0], length, 10);

   sendString("--------------------------------------------------------------------------------------------------");
   LINE_BREAK;
   sendString("Csomag felépítése: ");
   LINE_BREAK;
   sendString("Length |BROADCASTPACKET|myAddress|messageType|distance|");
   LINE_BREAK;
   sendString("PL payload: ");
   LINE_BREAK;
   sendString(length);
   sendChar(buffer[1]+'0');
   sendChar(buffer[2]+'0');
   sendChar(buffer[3]+'0');
   sendChar(buffer[4]+'0');
   LINE_BREAK;

   sendString("Payload Length: ");
   sendString(length);
   LINE_BREAK;
   sendString("BROADCASTPACKET: ");
   sendChar(buffer[1]+'0');
   LINE_BREAK;
   sendString("My Address: ");
   sendChar(buffer[2]+'0');
   LINE_BREAK;
   sendString("Message Type (1 - NETWORKBUILDPACKET; 2 - MEASUREMENTPACKET): ");
   sendChar(buffer[3]+'0');
   LINE_BREAK;
   sendString("Distance: ");
   sendChar(buffer[4]+'0');
   LINE_BREAK;
   sendString("--------------------------------------------------------------------------------------------------");
   LINE_BREAK;
}

