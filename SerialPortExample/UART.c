#include "UART.h"


// A setSerialPort több regiszterel dolgozik.
// Például a P3SEL -> Port 3 function selection. Az adatlapban 
// részletesen le van írva, hogy melyik érték hatására a port melyik lábának
// mi lesz a feladata. NEM kell tudni őket fejből. :)

// Hogyan találhatjuk meg ezt adatlapból?
//  1) Nyissuk meg az msp430f2274 adatlapját! (Be van linkelve a források között)
//  2) Keressünk rá az UART szóra! (Universal asynchronous receiver/transmitter)
//     - Az első találatból kiderül, hogy a mikrovezérlőn van UART. Ez eddig jó.
//  3) A 4. és 5. találatból már az is kiderül, hogy az USCI_A0 (bármi is ez) UART a P3.4,5 lábakon van
//  4) A 10. találatnál azt is megtudjuk, hogy az USCI nevű modul támogatja az UART-ot, SPI-t és I2C-t.
//     - Universal Serial Communication Interface
//  5) Ebből többet nem tudunk meg, nyissuk meg a család általános adatlapját. 
//     - Ezt megtalálni nem olyan triviális. Tudjuk, hogy az MSP430F2274-es uC-vel dolgozunk. A TI weblapján a 3.
//       piros sorból kiderül, hogy mi az MSP430x2xx családot használjuk
//     - Rákattintva bejön a 644 oldalas doksi, amit szintén belinkeltem 
//     - Kizártnak tartom, hogy ezt bárki egymaga az elejétől a végéig elolvasta. Nektek sem kell.
//  6) A tartalomjegyzéket végigolvasva találunk egy Universal Serial Communication Interface, UART Mode fejezetet. Ugorjunk oda!
//  7) Ez a fejezet csak 17 oldal, ezt végig kell olvasni!
//  8) A 15.4.2-es fejezet leírja, hogy mi az UCAxCTL1 (Az x a modul számát jelenti. Mi a 0-t használjuk)
//     - A felső két bit 1-re állítása (UCSSEL_2 = 0b11000000 konstans) az SMCLK-t állítja be a szinkronizációs óra forrásának
//     - Az SMCLK az a forrás, amit a belső órajelre is használunk, tehát ez az 1MHz-es forrásunk jelenleg
//  9) A 15.4.3-as fejezetből megtudjuk, hogy az UCAxBR0 és BR1 a Baud Rate control register
//     - A probléma az, hogy 1MHz-ből kell egy kb. 10kHz-es órajelet előállítani. A regiszter értékeinek meghatározása le van írva
//  10) A többi regiszter hasonlóak kereshető ki
//
//  +1) Az adatlap nem arra való, hogy egy kezdő programozó ez alapján implementáljon egy teljes rendszert. Ez a professzionális programozók játéka,
//      akik ismerik a szakkifejezéseket, módszereket teljes kontrollt akarnak az eszköz felett.
//      Az itt látható kódokat nem az adatlap alapján írtam meg, hanem a Google segítségével.
//      Az adatlapot arra használtam, hogy megértsem, mi mit csinál és leellenőrizzem a kódot.
//      !!! A Google segítségével nagyon sok, látszólag működő kódot lehet találni. Mindig ellenőrizzétek le a kódot, ha a forrása nem megbízható.


void setSerialPort(void){ // USCI_A0
  P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void sendChar(char c){
  while (!(IFG2 & UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = c;                            // Send
}

volatile int j=0;

void sendString(const char *c){
  int i=0;
  while(c[i]){                             // Elértük a string-et lezáró null-t?
    sendChar(c[i++]);                      // Ha nem, küldjük el a karaktert
    __delay_cycles(6000);                  // A hardver nem képes túl gyorsan küldeni a soros portra a karaktereket, ezért kell minden karakter után várni
  }
}

char getUARTCharBlocking(){
  while (!isCharInBuffer());
  return getUARTChar();
}
