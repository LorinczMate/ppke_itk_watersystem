#include "DataLinkLayer.h"
#include "utility.h"
#include "PhysicLayer.h"
#include "UART.h"

char myAddress;
char myDistance;
char const NETWORKBUILDPACKET = '1';
char const MEASUREMENTDATATYPE = '0';
char const BROADCASTPACKET = '0';
char to;
char from;
char messageType;
char distance;
char parentnode;
char source;
char* measurementData;

void initLayer(char _myAddress){
   myAddress = _myAddress;
}

void receiveDLPacket(char length, char *payload){
   to   = payload[0];
   from = payload[1];
   messageType = payload[2]; //ha 1 akkor NETWORKBUILDPACKET ha 0 akkor mérési adat
   distance = payload[3];
   parentnode = payload[4];
   source = payload[5];
   measurementData = payload+6;


   if (to == myAddress){
      payload[length]=0;
      if(messageType == MEASUREMENTDATATYPE){
    	  /*sendString("Mérési üzenet érkezett!\n\r");
    	  sendString("A feladó: ");
    	  sendChar(source);
    	  sendString(" volt, aki pedig az üzenetet beküldte: ");
    	  sendChar(from);
    	  sendString(" volt!\n\r");
    	  sendString("A mérési adat: \n\r");
    	  sendString(measurementData);
    	  sendString("\n\r");

*/
    	  sendChar(to);
    	  sendChar(';');
    	  sendChar(from);
    	  sendChar(';');
    	  sendChar(messageType);
    	  sendChar(';');
    	  sendChar(distance);
    	  sendChar(';');
    	  sendChar(parentnode);
    	  sendChar(';');
    	  sendChar(source);
    	  sendChar(';');
    	  sendString(measurementData);
      }
      //sendString(payload);
      sendString("\n\r\n\r");
   }
}

void sendNetworkBuildDLPacket(char messageType, char parentnode, char distance, char myAddress,  char payloadLength, char *payload){ //címzett, üzenet hossza + az üzenet.
   arrayShiftRight(payloadLength, payload, source);
   arrayShiftRight(payloadLength+1, payload, parentnode);
   arrayShiftRight(payloadLength+2, payload, distance); //elsőként a vermünkbe a saját címünket tesszük, de ekkor a main-ben írt üzenet már benne van!
   arrayShiftRight(payloadLength+3, payload, messageType);
   arrayShiftRight(payloadLength+4, payload, myAddress); //második elem a címzett címe lesz
   arrayShiftRight(payloadLength+5, payload, BROADCASTPACKET);
   sendPPacket(payloadLength+6, payload); // így amikor megkapod az üzenetet 1 helyen a címzett címe lesz, majd a küldőé ezután következik az üzenet!
}




