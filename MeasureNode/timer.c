#include <msp430f2274.h>
#include "UART.h"
#include "timer.h"



void initTimerB(unsigned short period){
  // Ez a függvény nem indítja el az idõzítõt
  BCSCTL3 |= LFXT1S_2; // Basic clock system control register ->
  TBCCTL0 = CCIE;      // Engedélyezi a megszakítást a TimerB-re
  TBCCR0 = period;      // A megszakítás sebessége. Minél kisebb, annál sûrûbb.
                       // 10000 => 0.84sec 12 =1ms
  TBCTL = MC_1+TBSSEL_1; // Timer órajele és viselkedése
}


