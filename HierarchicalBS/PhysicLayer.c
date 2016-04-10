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
   if (RFReceivePacket(rxBuffer,&len, 0)){
      receivePPacket(len, rxBuffer);
   } 
   P2IFG &= ~TI_CC_GDO0_PIN;
}


void receivePPacket(char length, char *payload){
   // Lehetne ide CRC kód ellenőrzést tenni, de most ezt hagyjuk
   // Egyszerűen csak továbbítjuk a felső rétegnek
   receiveDLPacket(length, payload);
}

/**
   length  : A payload hossza byte-ban
   payload : A payload, amit el akarunk küldeni
*/
void sendPPacket(char length, char *payload){
   arrayShiftRight(length, payload, length);
   RFSendPacket(payload, length+1);
}
