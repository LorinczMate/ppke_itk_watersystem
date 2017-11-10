#include "M_PhysicLayer.h"
#include "UART.h"
#include "M_include.h"
#include "M_DataLinkLayer.h"
#include "utility.h"

char rxBuffer[70];

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
	//reveivePPacketFromSerialPort(length, payload);
	receiveDLPacket(length, payload, rssi);
}

/**
   length  : A payload hossza byte-ban
   payload : A payload, amit el akarunk küldeni
*/
void sendPPacket(char length, char *payload){
   arrayShiftRight(length, payload, length);
   RFSendPacket(payload, length+1);
}

