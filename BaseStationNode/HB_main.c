//----------------------------------------------------------------------------
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.0
//  For EZ-RF2500 -- Roscoe Giles
// Basic operation is to push button 1 and send a message to the other board.
// When a message is sent, the green LED is toggled (LED2)
// When a message is received, the red LED is toggled. (LED 1)
//-----------------------------------------------------------------------------
//  Originally by
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//----------------------------------------------------------------------------

//#define VERBOSE
#include <stdlib.h>
#include "include.h"
#include "UART.h"
#include "DataLinkLayer.h"
#include <msp430x22x4.h>

// bit masks for P1 on the RF2500 target board


extern char paTable[];		// power table for C2500
extern char paTableLen;

char txBuffer[50];  // Ez a küldött csomagok tárolására szolgáló buffer
char rxBuffer[50];  // Ez a fogadott csomagok tárolására szolgáló buffer
unsigned int i;

char myAddress = 1;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	BCSCTL1 = CALBC1_1MHZ;          // Set DCO (Digitally controlled oscillator)
	DCOCTL = CALDCO_1MHZ;                     // Set DCO

	setSerialPort();

	P2SEL &= ~(BIT7 + BIT6); //clear select bits for XIN,XOUT, which are set by default (utolsó két bit, az SPI interfész használatához kell.)

	TI_CC_SPISetup();                         // Initialize SPI port
	TI_CC_PowerupResetCCxxxx();               // Reset CCxxxx
	writeRFSettings();                        // Write RF settings to config reg
	TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen); //Write PATABLE

	// Configure ports -- switch inputs, LEDs, GDO0 to RX packet info from CCxxxx

	P1REN |= SW1_MASK; //  enable pullups on SW1
	P1OUT |= SW1_MASK;

	// Configure LED outputs on port 1
	P1DIR = LED1_MASK + LED2_MASK; //Outputs

	// setup for interrupts related to receipt of a message from the CC2500

	TI_CC_GDO0_PxIES |= TI_CC_GDO0_PIN; // Int on falling edge of GDO0 (end of pkt)
	TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;    // Clear Interrupt flag for GDO0 pin
	TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN;      // Enable interrupt on end of packet

	// turn on the CC2500 in receive mode
	TI_CC_SPIStrobe(TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mode.
											  // When a pkt is received, it will
											  // signal on GDO0 and wake CPU
	TURN_OFF_BOTH_LED;

	__bis_SR_register(GIE);                   // Enter LPM3, enable interrupts
#ifdef VERBOSE
	DOUBLE_LINE_BREAK;
	sendString("**************************************************************************************************");
	LINE_BREAK;
	sendString("                                      BASESTATION                                                 ");
	LINE_BREAK;
	sendString("**************************************************************************************************");
	DOUBLE_LINE_BREAK;
	sendString("Jelenleg a BaseStationNode van sorosportra csatakoztatva.");
	LINE_BREAK;
	sendString("A halozat epites elkezdesehez, kerlek nyomd meg a gombot a controlleren!");
	LINE_BREAK;
	TURN_ON_BOTH_LED;
#endif
	initLayer(myAddress);
	char txbuffertmp[20];
	char networkBuilderPacketCounter = 0;
	char networkBuilderPacketCounterString[5];
	while (1) {
		if (BUTTON_PRESSED) {
			TURN_OFF_BOTH_LED;
			networkBuilderPacketCounter++;
			txBuffer[0] = 0;
			sendNetworkBuildDLPacket(1, 0, myAddress, txBuffer);
			TURN_ON_GREEN_LED;
			itoa(networkBuilderPacketCounter, networkBuilderPacketCounterString, 10);
#ifdef VERBOSE
			LINE_BREAK;
			sendString("Kiment a(z) ");
			sendString(networkBuilderPacketCounterString);
			sendString(" halozatepito uzenet!");
			LINE_BREAK;
#endif
			__delay_cycles(2000);
		}
	}
}

