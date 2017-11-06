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
			receiveNetworkBuildDLLPacketToSerialPort(payload);
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
			blinkLEDsForRecevingNetworkBuildPacket(parentnode);
			sendNetworkBuildDLLPacket(NETWORKBUILDDATATYPE, myDistance, from,
					payload);
		} else if ((messageType == MEASUREMENTDATATYPE) && (to == myAddress)) {
			source = payload[5];
			from = myAddress;
			to = parentnode;
			//receiveMeasurementDLLPacketToSerialPort(length, payload);
			//így ebben a legutolsó RSSI még nem kerül bele!
			//arrayShiftRight(length-6, payload+6, rssi);
			//rssi length hosszának növelése kellene!
			length++;
			sendString("The measurement packet is transmitted!");
			sendMeasurementDLLPacket(MEASUREMENTDATATYPE, to, from, length - 6,
					payload + 6, rssi); //source-t külön át kéne adni különben a globál sourcot küldi el és ha több eszköz csomópontja egy azonos node ott konfliktus lehet!
		}
	}
}

void sendNetworkBuildDLLPacket(char messageType, char myDistance, char from,
		char *payload) { //címzett, üzenet hossza + az üzenet.
	/*--------------------------
	 A CSOMAG FELÉPÍTÉSE:
	 BROADCASTPACKET|myAddress|messageType|distance|
	 */
	int payloadLength = 0;
	arrayShiftRight(payloadLength++, payload, myDistance);
	arrayShiftRight(payloadLength++, payload, messageType);
	arrayShiftRight(payloadLength++, payload, myAddress);
	arrayShiftRight(payloadLength++, payload, 0); //BROADCASTPACKET
	//sendNetworkPacketToSerialPort(payload);
	sendPPacket(payloadLength, payload);
	blinkLEDsForSendingNetworkBuildPacket();
}

void sendMeasurementDLLPacket(char messageType, char to, char from,
		char payloadLength, char *payload, unsigned char rssi) {
	__delay_cycles(10000000);
	putADCInBuffer(measurement);
	char rssi_str[] = { 0, 0, 0, 0 };
	itoa(rssi, rssi_str, 10);
	for (int i = 0; i < rssilength; i++) {
		arrayShiftRight(payloadLength++, payload, rssi_str[2 - i]);
	} //rssi értékek forditva fognak bemásolódni
	for (int i = 0; i < measurementlength; i++) {
		arrayShiftRight(payloadLength++, payload, measurement[3 - i]);
	} // szintén
	arrayShiftRight(payloadLength++, payload, source);
	arrayShiftRight(payloadLength++, payload, parentnode);
	arrayShiftRight(payloadLength++, payload, myDistance);
	arrayShiftRight(payloadLength++, payload, messageType);
	arrayShiftRight(payloadLength++, payload, from);
	arrayShiftRight(payloadLength++, payload, to);
	blinkLEDsForMeasurementPacket(to);
	sendPPacket(payloadLength, payload);
}

void sendMyMeasurementDLPacket(char messageType, char parentnode, char source,
		char distance, char *measurementData, char payloadLength, char *payload) {
	/*--------------------------
	 A CSOMAG FELÉPÍTÉSE:
	 parentnode|myAddress|messageType|distance|parentnode|myAddress|measurementData|rssi

	 HB_DLLayerben kapott csomag:
	 to|from|messagetype|distance|parentnode|source|measurementData|rssi
	 ----------------------------*/

	putADCInBuffer(measurement);
	for (int i = 0; i < rssilength; i++) {
		arrayShiftRight(payloadLength++, payload, 'n');
	} //rssi értékek forditva fognak bemásolódni
	for (int i = 0; i < measurementlength; i++) {
		arrayShiftRight(payloadLength++, payload, measurement[3 - i]);
	} // szintén
	arrayShiftRight(payloadLength++, payload, myAddress);
	arrayShiftRight(payloadLength++, payload, parentnode);
	arrayShiftRight(payloadLength++, payload, myDistance);
	arrayShiftRight(payloadLength++, payload, messageType);
	arrayShiftRight(payloadLength++, payload, myAddress);
	arrayShiftRight(payloadLength++, payload, parentnode);
	//sendMyMeasurementToSerialPort(payload, measurementData, rssi);
	sendPPacket(payloadLength, payload);
	blinkLEDsForMyMeasurementPacket(parentnode);

}

void blinkLEDsForTurningOnTheNode() {
	/*The Measure node's green LED will blink when one will turn it on as much as its own address.
	 *At the end the green led will stay on, which shows from this moment that the device is turned on.
	 * example: source address = 2 : _ | _ | _ | ->     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 */
	for (int i = 0; i < myAddress * 2 + 1; i++) {
		__delay_cycles(500000);
		BLINK_GREEN_LED;
	}
}

void blinkLEDsForRecevingNetworkBuildPacket(char parentnode) {
	/*The Measure node's red LED will blink when it receives a NetworkBuilding packet as much as the sender's address.
	 *At the red the green led will stay on to know that the device is connected to the network.
	 * example: source address = 2 : _ | _ | _ | ->     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 * @param		parentnode	 The address of the node who sent the NetworkBuild packet and who will be our parent node
	 */

	for (int i = 0; i < parentnode * 2 + 1; i++) {
		__delay_cycles(500000);
		BLINK_RED_LED;
	}
}

void blinkLEDsForSendingNetworkBuildPacket() {
	/*The Measure node's both LED will blink once when it forwards the NetworkBuild packet.
	 *At the end the both led will stay on.
	 * example: source address = 2 : _ | _ | >     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 */

	BLINK_BOTH_LED;
	__delay_cycles(500000);
	BLINK_BOTH_LED;
}

void blinkLEDsForMeasurementPacket(char to) {
	/*The Measure node's green LED will blink when one will turn it on as much as its own address.
	 *At the end the green led will stay on.
	 * example: to  address = 2 : _ | _ | _ | ->     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 * @param		to	 The address of the device to whom the measurement message is transmitted.
	 */

	TURN_OFF_BOTH_LED
	;
	for (int i = 0; i < to * 2 + 1; i++) {
		__delay_cycles(500000);
		BLINK_BOTH_LED;
	}
}

void blinkLEDsForMyMeasurementPacket(char parentnode) {
	/*The Measure node's both led will blink as much as our parent nodes address to whom the message is addressed.
	 *At the end the both led will stay on.
	 * example: parent nodes address = 2 : _ | _ | _ | ->     means: _: OFF, |: ON, -> stay on the last state
	 *
	 * @author		Marcell Zoltan Szalontay
	 * @date		2017.11.05
	 * @param		parentnode	 The address of the device to whom the measurement message is transmitted.
	 */

	TURN_OFF_BOTH_LED
	;
	for (int i = 0; i < parentnode * 2 + 1; i++) {
		__delay_cycles(500000);
		BLINK_BOTH_LED;
	}
}

char getParentNode() {
	return parentnode;
}

char getDistance() {
	return distance;
	DOUBLE_LINE_BREAK;
	sendString(
			"**************************************************************************************************");
	LINE_BREAK;
	sendString(
			"                                      MEASURENODE                                                 ");
	LINE_BREAK;
	sendString("                                      ADDRESS: ");
	sendChar(myAddress + '0');
	LINE_BREAK;
	sendString(
			"**************************************************************************************************");
	DOUBLE_LINE_BREAK;
	sendString("Jelenleg egy MeasureNode van sorosportra csatakoztatva.");
	LINE_BREAK;
	sendString(
			"A meresi adat kuldesehez, kerlek nyomd meg a gombot a controlleren!");
	LINE_BREAK;
}

void sendNetworkPacketToSerialPort(char *buffer) {
	LINE_BREAK;
	sendString("MeasurementNode DataLinkLayer tovabbkuldte a NetworkBuild-et!");
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
}

void sendMyMeasurementToSerialPort(char *buffer, char *measurementData,
		char *rssi) {

	// bug result: measurementData: 1530
	// rssi : 530 - és statikusan az is marad.

	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString(
			"MeasurementNode DataLinkLayer elkulte a MyMeasurementPacket-et!");
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
	LINE_BREAK;
	sendString(measurementData);
	LINE_BREAK;
	sendString(rssi);
	LINE_BREAK;

	sendString("To: ");
	sendChar(buffer[0] + '0');
	LINE_BREAK;
	sendString("From: ");
	sendChar(buffer[1] + '0');
	LINE_BREAK;
	sendString(
			"Message Type (1 - NETWORKBUILDPACKET; 0 - MEASUREMENTPACKET): ");
	sendChar(buffer[2] + '0');
	LINE_BREAK;
	sendString("Distance: ");
	sendChar(buffer[3] + '0');
	LINE_BREAK;
	sendString("Parent Node: ");
	sendChar(buffer[4] + '0');
	LINE_BREAK;
	sendString("Source: ");
	sendChar(buffer[5] + '0');
	LINE_BREAK;
	sendString("Measurement Data: ");
	sendString(measurementData);
	LINE_BREAK;
	sendString("RSSI: ");
	sendString(rssi);
	LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;

}

void receiveNetworkBuildDLLPacketToSerialPort(char *buffer) {
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("DataLinkLayer NetworkBuild uzenetet kapott!");
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

void receiveMeasurementDLLPacketToSerialPort(char length, char *buffer) {
	char rssi[5];
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
	sendString("DataLinkLayer Measurement uzenetet kapott!");
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

	memcpy(measurement, buffer + 6, 10);
	rssilength = length - 16;
	memcpy(rssi, buffer + 16, rssilength);

	sendString(measurement);
	sendString(rssi);
	char measurementToSerial;
	char rssiToSerial;
	itoa(rssi, rssiToSerial, 10);
	itoa(measurement, measurementToSerial, 10);

	sendString(measurementToSerial);
	sendString(rssiToSerial);
	sendString("To: ");
	sendChar(buffer[0] + '0');
	LINE_BREAK;
	sendString("From: ");
	sendChar(buffer[1] + '0');
	LINE_BREAK;
	sendString(
			"Message Type (1 - NETWORKBUILDPACKET; 2 - MEASUREMENTPACKET): ");
	sendChar(buffer[2] + '0');
	LINE_BREAK;
	sendString("Distance: ");
	sendChar(buffer[3] + '0');
	LINE_BREAK;
	sendString("Parent Node: ");
	sendChar(buffer[4] + '0');
	LINE_BREAK;
	sendString("Source: ");
	sendChar(buffer[5] + '0');
	LINE_BREAK;
	sendString("Measurement Data: ");
	sendString(measurementToSerial);
	LINE_BREAK;
	sendString("RSSI: ");
	sendString(rssiToSerial);
	LINE_BREAK;
	sendString(
			"--------------------------------------------------------------------------------------------------");
	LINE_BREAK;
}

void nodeIntroductionToSerialPort() {
	DOUBLE_LINE_BREAK;
	sendString(
			"**************************************************************************************************");
	LINE_BREAK;
	sendString(
			"                                      MEASURENODE                                                 ");
	LINE_BREAK;
	sendString("                                      ADDRESS: ");
	sendChar(myAddress + '0');
	LINE_BREAK;
	sendString(
			"**************************************************************************************************");
	DOUBLE_LINE_BREAK;
	sendString("Jelenleg egy MeasureNode van sorosportra csatakoztatva.");
	LINE_BREAK;
	sendString(
			"A meresi adat kuldesehez, kerlek nyomd meg a gombot a controlleren!");
	LINE_BREAK;
}
