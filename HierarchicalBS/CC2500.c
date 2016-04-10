//----------------------------------------------------------------------------
//  Description:  This file contains functions that configure the CC1100/2500 
//  device.
// 
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v3.41
//----------------------------------------------------------------------------


#include "include.h"
#include "TI_CC_CC2500.h"
#include "UART.h"

#define TI_CC_RF_FREQ  2400  // 315, 433, 868, 915, 2400



//-------------------------------------------------------------------------------------------------------
//  void writeRFSettings(void)
//
//  DESCRIPTION:
//  Used to configure the CCxxxx registers.  There are five instances of this
//  function, one for each available carrier frequency.  The instance compiled
//  is chosen according to the system variable TI_CC_RF_FREQ, assigned within
//  the header file "TI_CC_hardware_board.h".
//
//  ARGUMENTS:
//      none
//-------------------------------------------------------------------------------------------------------


#if TI_CC_RF_FREQ == 2400                          // 2.4GHz
void writeRFSettings(void)
{
    // Write register settings
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   0x0B);  // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG0,   0x06);  // GDO0 output pin config.
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTLEN,   0xFF);  // Packet length.
    //TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL1, 0x05);  // Packet automation control.
    TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL0, 0x05);  // Packet automation control.
    TI_CC_SPIWriteReg(TI_CCxxx0_ADDR,     0x01);  // Device address.
    //TI_CC_SPIWriteReg(TI_CCxxx0_CHANNR,   0x00); // Channel number.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL1,  0x06); // Freq synthesizer control.
    //TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL0,  0x00); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ2,    0x21); // Freq control word, high byte
    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ1,    0x62); // Freq control word, mid byte.
    TI_CC_SPIWriteReg(TI_CCxxx0_FREQ0,    0x76); // Freq control word, low byte.
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0xF5); // Modem configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x83); // Modem configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG2,  0x13); // Modem configuration.
    //TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG1,  0x62); // Modem configuration.
    //TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG0,  0x76); // Modem configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_DEVIATN,  0x15); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM1 ,   0x3F); //MainRadio Cntrl State  Machine // Ez fontos
    TI_CC_SPIWriteReg(TI_CCxxx0_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_CCxxx0_FOCCFG,   0x16); // Freq Offset Compens. Config
    //TI_CC_SPIWriteReg(TI_CCxxx0_BSCFG,    0x1C); //  Bit synchronization config.
    //TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL2, 0xC7); // AGC control.
    //TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL1, 0x00); // AGC control.
    //TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL0, 0xB2); // AGC control.
    //TI_CC_SPIWriteReg(TI_CCxxx0_FREND1,   0xB6); // Front end RX configuration.
    //TI_CC_SPIWriteReg(TI_CCxxx0_FREND0,   0x10); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL3,   0xE9); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL2,   0x2A); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL1,   0x00); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL0,   0x1F); // Frequency synthesizer cal.
    //TI_CC_SPIWriteReg(TI_CCxxx0_FSTEST,   0x59); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST2,    0x81); // Various test settings.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST1,    0x35); // Various test settings.
    TI_CC_SPIWriteReg(TI_CCxxx0_TEST0,    0x09);  // Various test settings.
}

// PATABLE (0 dBm output power)
char paTable[] = {0xFB};
char paTableLen = 1;

#endif


//-----------------------------------------------------------------------------
//  void RFSendPacket(char *txBuffer, char size)
//
//  DESCRIPTION:
//  This function transmits a packet with length up to 63 bytes.  To use this
//  function, GD00 must be configured to be asserted when sync word is sent and
//  de-asserted at the end of the packet, which is accomplished by setting the
//  IOCFG0 register to 0x06, per the CCxxxx datasheet.  GDO0 goes high at
//  packet start and returns low when complete.  The function polls GDO0 to
//  ensure packet completion before returning.
//
//  ARGUMENTS:
//      char *txBuffer
//          Pointer to a buffer containing the data to be transmitted
//
//      char size
//          The size of the txBuffer
//-----------------------------------------------------------------------------
void RFSendPacket(char *txBuffer, char size)
{
    TI_CC_SPIWriteBurstReg(TI_CCxxx0_TXFIFO, txBuffer, size); // Write TX data
    TI_CC_SPIStrobe(TI_CCxxx0_STX);         // Change state to TX, initiating
                                            // data transfer
    while (!(TI_CC_GDO0_PxIN&TI_CC_GDO0_PIN));
                                            // Wait GDO0 to go hi -> sync TX'ed
    while (TI_CC_GDO0_PxIN&TI_CC_GDO0_PIN);
                                            // Wait GDO0 to clear -> end of pkt
}



//-----------------------------------------------------------------------------
//  char RFReceivePacket(char *rxBuffer, char *length)
//
//  DESCRIPTION:
//  Receives a packet of variable length (first byte in the packet must be the
//  length byte).  The packet length should not exceed the RXFIFO size.  To use
//  this function, APPEND_STATUS in the PKTCTRL1 register must be enabled.  It
//  is assumed that the function is called after it is known that a packet has
//  been received; for example, in response to GDO0 going low when it is
//  configured to output packet reception status.
//
//  The RXBYTES register is first read to ensure there are bytes in the FIFO.
//  This is done because the GDO signal will go high even if the FIFO is flushed
//  due to address filtering, CRC filtering, or packet length filtering.
//
//  ARGUMENTS:
//      char *rxBuffer
//          Pointer to the buffer where the incoming data should be stored
//      char *length
//          Pointer to a variable containing the size of the buffer where the
//          incoming data should be stored. After this function returns, that
//          variable holds the packet length.
//
//  RETURN VALUE:
//      char
//          0x80:  CRC OK
//          0x00:  CRC NOT OK (or no pkt was put in the RXFIFO due to filtering)
//-----------------------------------------------------------------------------
char RFReceivePacket(char *rxBuffer, char *length, char *retStatus)
{
  char status[2];
  char pktLen;

  if ((TI_CC_SPIReadStatus(TI_CCxxx0_RXBYTES) & TI_CCxxx0_NUM_RXBYTES))
  {
    pktLen = TI_CC_SPIReadReg(TI_CCxxx0_RXFIFO); // Read length byte

    if (pktLen <= *length)                  // If pktLen size <= rxBuffer
    {
      TI_CC_SPIReadBurstReg(TI_CCxxx0_RXFIFO, rxBuffer, pktLen); // Pull data
      *length = pktLen;                     // Return the actual size
      TI_CC_SPIReadBurstReg(TI_CCxxx0_RXFIFO, status, 2); // Read appended status bytes
      if (retStatus){
        retStatus[0]=status[0];
        retStatus[1]=status[1];
      }
      return (char)(status[TI_CCxxx0_LQI_RX]&TI_CCxxx0_CRC_OK);
    }                                       // Return CRC_OK bit
    else
    {
      *length = pktLen;                     // Return the large size
      TI_CC_SPIStrobe(TI_CCxxx0_SFRX);      // Flush RXFIFO
      return 0;                             // Error
    }
  }
  else
      return 0;                             // Error
}
