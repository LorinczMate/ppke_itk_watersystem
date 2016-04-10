#include "DataLinkLayer.h"
#include "utility.h"
#include "PhysicLayer.h"
#include "UART.h"

char myAddress;
char myDistance='9';
char const MEASUREMENTDATATYPE = '0';
char const NETWORKBUILDDATATYPE = '1';
char const BROADCASTPACKET = '0';
char to;
char from;
char messageType;
char distance;
char parentnode;
char source;
char txBufferDL[20];

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

   if (to == myAddress || to == BROADCASTPACKET){
      payload[length]=0;
      if(messageType == NETWORKBUILDDATATYPE){
          if (myDistance > (distance + 1)){
        	  parentnode = from;
        	  myDistance = distance + 1;

        	  // csomag továbbítás

        	  from = myAddress;
        	  distance = myDistance;
        	  sendNetworkBuildDLPacket(NETWORKBUILDDATATYPE, parentnode, distance, from, 2, txBufferDL);
          }
      }
      else if (messageType == MEASUREMENTDATATYPE) { //mérési adatot kell a bázisállomás felé továbbítani
    	  if(to == myAddress){
    		  txBufferDL[20];
    		  from = myAddress;
    		  to = parentnode;
    		  sendMeasurementDLPacket(MEASUREMENTDATATYPE, to, from, 3, txBufferDL);
    	  }
      }

      sendString(payload);
      sendString("\n\r\n\r");
   }
}


void sendNetworkBuildDLPacket(char messageType, char parentnode, char distance, char from,  char payloadLength, char *payload){ //címzett, üzenet hossza + az üzenet.
   arrayShiftRight(payloadLength, payload, source);
   arrayShiftRight(payloadLength+1, payload, parentnode);
   arrayShiftRight(payloadLength+2, payload, distance); //elsőként a vermünkbe a saját címünket tesszük, de ekkor a main-ben írt üzenet már benne van!
   arrayShiftRight(payloadLength+3, payload, messageType);
   arrayShiftRight(payloadLength+4, payload, myAddress); //második elem a címzett címe lesz
   arrayShiftRight(payloadLength+5, payload, BROADCASTPACKET);
   sendPPacket(payloadLength+6, payload); // így amikor megkapod az üzenetet 1 helyen a címzett címe lesz, majd a küldőé ezután következik az üzenet!
}

void sendMeasurementDLPacket(char messageType, char to, char from, char payloadLength, char *payload){
	arrayShiftRight(payloadLength, payload, source);
	arrayShiftRight(payloadLength + 1, payload, parentnode);
	arrayShiftRight(payloadLength + 2, payload, distance); //elsőként a vermünkbe a saját címünket tesszük, de ekkor a main-ben írt üzenet már benne van!
	arrayShiftRight(payloadLength + 3, payload, messageType);
	arrayShiftRight(payloadLength + 4, payload, from); //második elem a címzett címe lesz
	arrayShiftRight(payloadLength + 5, payload, to);
	sendPPacket(payloadLength + 6, payload);
}

void sendMyMeasurementDLPacket(char messageType, char parentnode, char source, char distance, char payloadLength, char *payload){
	   arrayShiftRight(payloadLength, payload, myAddress);
	   arrayShiftRight(payloadLength+1, payload, parentnode);
	   arrayShiftRight(payloadLength+2, payload, distance); //elsőként a vermünkbe a saját címünket tesszük, de ekkor a main-ben írt üzenet már benne van!
	   arrayShiftRight(payloadLength+3, payload, messageType);
	   arrayShiftRight(payloadLength+4, payload, myAddress); //második elem a címzett címe lesz
	   arrayShiftRight(payloadLength+5, payload, parentnode);
	   sendPPacket(payloadLength+6, payload);
}



char getParentNode(){
	return parentnode;
}

char getDistance(){
	return distance;
}

