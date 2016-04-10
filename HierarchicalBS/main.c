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

#include <stdlib.h>
#include "include.h"
#include "UART.h"
#include "DataLinkLayer.h"
#include <msp430x22x4.h>

// bit masks for P1 on the RF2500 target board
#define LED1_MASK              0x01 // == BIT0	
#define LED2_MASK              0x02 // == BIT1
#define SW1_MASK               0x04 // == BIT2

extern char paTable[];		// power table for C2500
extern char paTableLen;

char txBuffer[50];  // Ez a küldött csomagok tárolására szolgáló buffer
char rxBuffer[50];  // Ez a fogadott csomagok tárolására szolgáló buffer
unsigned int i;

char myAddress = '1';

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

	// Input switch (on target board).
	// Pushing switch pulls down the input bit SW1 and triggers an input interrupt
	//   on port 1.  The handler sends a message to the second board.

	P1REN |= SW1_MASK; //  enable pullups on SW1
	P1OUT |= SW1_MASK;
	//P1IES = SW1_MASK; //Int on falling edge
	//P1IFG &= ~(SW1_MASK);//Clr flag for interrupt
	//P1IE = SW1_MASK;//enable input interrupt

	// Configure LED outputs on port 1
	P1DIR = LED1_MASK + LED2_MASK; //Outputs
	//P1OUT |= (LED1_MASK+LED2_MASK); // both lights on

	// setup for interrupts related to receipt of a message from the CC2500
	// A rádiómodul is válthat ki megszakítást
	// Ha megnézzük, hogy mik ezek a regiszterek, kiderül, hogy ez is csak egy digitális bemenet a PORT2-n
	// Ez azért van, mert a rádiómodul nem csak SPI interfészen keresztül tartja a kapcsolatot a chip-pel,
	// hanem néhány digitális vonalon is, amiken keresztül különböző jelzéseket tud küldeni. (Pl.: Csomag jött)
	// Itt a modul GD0 lábára váltódik ki az interrupt

	TI_CC_GDO0_PxIES |= TI_CC_GDO0_PIN; // Int on falling edge of GDO0 (end of pkt)
	TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;    // Clear Interrupt flag for GDO0 pin
	TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN;      // Enable interrupt on end of packet

	// turn on the CC2500 in receive mode
	TI_CC_SPIStrobe(TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mode.
											  // When a pkt is received, it will
											  // signal on GDO0 and wake CPU
	//sendString("Hello\n\r");
	//sendString(" BS woke up! \n\r");

	__bis_SR_register(GIE);                   // Enter LPM3, enable interrupts

	initLayer(myAddress);

	while (1) {
		if (!(P1IN & BIT2)) {
			sendNetworkBuildDLPacket('1', '0', '0', myAddress, 4, txBuffer);
			__delay_cycles(2000);
			P1OUT |= BIT0;
			while (!(P1IN & BIT2))
				;
		} else {
			P1OUT &= ~BIT0;
		}
	}
}

