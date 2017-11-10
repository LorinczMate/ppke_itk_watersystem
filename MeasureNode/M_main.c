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
#include "M_include.h"
#include "UART.h"
#include "M_DataLinkLayer.h"
#include "msp430x22x4.h"
#include "timer.h"

unsigned int ADC_value = 0;

void ConfigureAdc(void);

extern char paTable[];		// power table for C2500
extern char paTableLen;
char txBuffer[70];
char rxBuffer[70];
unsigned int i;

unsigned char myAddress = 4;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	BCSCTL1 = CALBC1_1MHZ;          // Set DCO (Digitally controlled oscillator)
	DCOCTL = CALDCO_1MHZ;                     // Set DCO

	P2SEL |= BIT1;					// A__bis_SR_registerDC input pin P2.1
	ConfigureAdc();					// ADC set-up function call
	__enable_interrupt();

	setSerialPort();

	P2SEL &= ~(BIT7 + BIT6); //clear select bits for XIN,XOUT, which are set by default (utolsó két bit, az SPI interfész használatához kell.)

	TI_CC_SPISetup();                         // Initialize SPI port
	TI_CC_PowerupResetCCxxxx();               // Reset CCxxxx
	writeRFSettings();                       // Write RF settings to config reg
	TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen); //Write PATABLE

	// Configure ports -- switch inputs, LEDs, GDO0 to RX packet info from CCxxxx

	// Input switch (on target board).
	// Pushing switch pulls down the input bit SW1 and triggers an input interrupt
	//   on port 1.  The handler sends a message to the second board.

	P1REN |= SW1_MASK; //  enable pullups on SW1
	P1OUT |= SW1_MASK;
	P1IES = SW1_MASK; //Int on falling edge
	P1IFG &= ~(SW1_MASK); //Clr flag for interrupt
	P1IE = SW1_MASK; //enable input interrupt

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
	TI_CC_SPIStrobe(TI_CCxxx0_SRX);          // Initialize CCxxxx in RX mode.
											 // When a pkt is received, it will
											 // signal on GDO0 and wake CPU
	TURN_OFF_BOTH_LED
	;
	__bis_SR_register(GIE);                   // Enter LPM3, enable interrupts
	blinkLEDsForTurningOnTheNode();
	nodeIntroductionToSerialPort();
	initLayer(myAddress);
	char parentNode;
	char distance;
	char source = myAddress;
	char ADC_Temp[5];
	char txbuffertmp[20];
	initTimerB(0xFFFF);
	TIMER_OFF;
	char A[10];
	char counter = 1;


	TURN_ON_GREEN_LED;

	while (1) {
		__bis_SR_register(CPUOFF + GIE);
/*

		sendString("B");
		A[counter] = counter;
		arrayShiftRight(counter, A, counter);
		for(int i = 0; i < counter; i ++){
			char temp[]={0, 0};
			itoa(A[i], temp, 10);
			sendString(temp);
			LINE_BREAK;
		}
		sendString("C");
		sendBullshit(counter, A);
		counter++;
		sendString("D");
		TURN_ON_RED_LED;
		__delay_cycles(1000000);
		TURN_OFF_RED_LED;
*/
		parentNode = getParentNode();
		distance = getDistance();
		if (parentNode != UNDEDINED_PARENT_NODE) {
			//__delay_cycles(1000);		// Wait for ADC Ref to settle
			putADCInBuffer(ADC_Temp);
			sendMyMeasurementDLPacket(0, parentNode, source, distance, ADC_Temp,
					0, txBuffer);

		}
	}
}

// Function containing ADC set-up
void ConfigureAdc(void) {
	ADC10CTL1 = INCH_1 + ADC10DIV_3;         // Channel 3, ADC10CLK/3
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON; // + ADC10IE; // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
	ADC10AE0 |= BIT1;                         // ADC input enable P2.1
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void wakeUpFromPort1BUTTON(void) {
	TIMER_ON;
	__bic_SR_register_on_exit(CPUOFF);
	P1IFG &= ~(SW1_MASK);
}

/*
 * __attribute__((interrupt(TIMERB0_VECTOR)))
void wakeUpFromTimer(void) {
	__bic_SR_register_on_exit(CPUOFF);
}
 */
