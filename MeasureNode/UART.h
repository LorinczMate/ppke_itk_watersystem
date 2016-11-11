#ifndef UART_H
#define UART_H

#include  "msp430x22x4.h"

#define getUARTChar() UCA0RXBUF                // MAKRÓK!!! Csak akkor szabad őket használni, ha pontosan tudjuk, mit akarunk. Ez a sor például hibás. Miért?
#define isCharInBuffer() (IFG2 & UCA0RXIFG)    // Másik makró. Függvényeknek tűnnek. Általában nem jó gyakorlat. A makrók legyenek egyértelműen jelölve!

char getUARTCharBlocking();
void setSerialPort(void);
void sendChar(char c);
void sendString(const char *c);


#endif
