//rc_base.h

#ifndef _RC_BASE_H_
#define _RC_BASE_H_
//start of file

//Ԥ����
//RST: GPD8
//IRQ: GPF0
//NSS: GPB5

//Ԥ����
#define rc_rst_set()    {gpd_set_bit(BIT08_MASK);}
#define rc_rst_clr()    {gpd_clr_bit(BIT08_MASK);}
#define rc_irq()        (gpf_rd16()&BIT00_MASK)
#define rc_cs_set()     {gpb_set_bit(BIT05_MASK);}
#define rc_cs_clr()     {gpb_clr_bit(BIT05_MASK);}


/*============================================================================
�Ĵ�����ַ����
==============================================================================*/
// PAGE 0      Command and Status
#define     REG_RC500_PAGE                  0     // Page Select Register
#define     REG_RC500_COMMAND               1     // Command Register
#define     REG_RC500_FIFO_DATA             2     // FiFo Register
#define     REG_RC500_PRIMARY_STATUS        3     // Modem State/IRQ/ERR/LoHiAlert Reg
#define     REG_RC500_FIFO_LENGTH           4     // Buffer lenght Register
#define     REG_RC500_SECONDARY_STATUS      5 
#define     REG_RC500_INTERRUPT_EN          6     // IRQ enable Register
#define     REG_RC500_INTERRUPT_RQ          7     // IRQ bits Register
// PAGE 1      Control and Status
#define     REG_RC500_CONTROL               9     // processor control
#define     REG_RC500_ERROR_FLAG            10
#define     REG_RC500_COLL_POS              11
#define     REG_RC500_TIMER_VALUE           12 
#define     REG_RC500_CRC_RESULT_LSB        13
#define     REG_RC500_CRC_RESULT_MSB        14
#define     REG_RC500_BIT_FRAMING           15
// PAGE 2      Transmitter and Coder Control
#define     REG_RC500_TX_CONTROL            0x11
#define     REG_RC500_CW_CONDUCTANCE        0x12
#define     REG_RC500_MOD_CONDUCTANCE       0x13
#define     REG_RC500_CODER_CONTROL         0x14             //RC531
#define     REG_RC500_MOD_WIDTH             0x15
#define     REG_RC500_MOD_WIDTH_SOF         0x16             //SLRC400
#define     REG_RC500_TYPEB_FRAMING         0x17             //RC531
// PAGE 3      Receiver and Decoder Control
#define     REG_RC500_RX_CONTROL1           0x19
#define     REG_RC500_DECODER_CONTROL       0x1A
#define     REG_RC500_BIT_PHASE             0x1B
#define     REG_RC500_RX_THRESHOLD          0x1C
#define     REG_RC500_BPSK_DEM_CONTROL      0x1D             //RC531
#define     REG_RC500_RX_CONTROL2           0x1E
#define     REG_RC500_CLOCK_Q_CONTROL       0x1F
// PAGE 4      RF-Timing and Channel Redundancy
#define     REG_RC500_RX_WAIT               0x21
#define     REG_RC500_CHANNEL_REDUNDANCY    0x22
#define     REG_RC500_CRC_PRESET_LSB        0x23
#define     REG_RC500_CRC_PRESET_MSB        0x24
#define     REG_RC500_TIME_SLOT_PERIOD      0x25            //RC632
#define     REG_RC500_MFOUT_SELECT          0x26
#define     REG_RC500_RFU27                 0x27
// PAGE 5      FIFO, Timer and IRQ-Pin Configuration
#define     REG_RC500_FIFO_LEVEL            0x29
#define     REG_RC500_TIMER_CLOCK           0x2A
#define     REG_RC500_TIMER_CONTROL         0x2B
#define     REG_RC500_TIMER_RELOAD          0x2C
#define     REG_RC500_IRQ_PIN_CONFIG        0x2D
#define     REG_RC500_RFU2E                 0x2E
#define     REG_RC500_RFU2F                 0x2F
// PAGE 6      RFU
#define     REG_RC500_CRYPTO_SELECT         0x31
#define     REG_RC500_RFU32                 0x32
#define     REG_RC500_RFU33                 0x33
#define     REG_RC500_RFU34                 0x34
#define     REG_RC500_RFU35                 0x35
#define     REG_RC500_RFU36                 0x36
#define     REG_RC500_RFU37                 0x37
// PAGE 7      Test Control
#define     REG_RC500_RFU39                 0x39   //RFU
#define     REG_RC500_TEST_ANA_SELECT       0x3A   //selects analog test mode
#define     REG_RC500_RFU3B                 0x3B   //RFU
#define     REG_RC500_RFU3C                 0x3C   //RFU
#define     REG_RC500_TEST_DIGI_SELECT      0x3D   //selects digital test mode
#define     REG_RC500_RFU3E                 0x3E   //RFU
#define     REG_RC500_TEST_DIGI_ACCESS      0x3F

//--------------------------------------------------------------------------------
//reg control
//7-6 00 Reserved for future use
#define BIT_STANDBY   BIT05_MASK   //StandBy Setting this bit to 1 enters the Soft PowerDown Mode. 
                                   //This means, internal current consuming blocks are switched off, 
                                   //the oscillator keeps running.
#define BIT_POWER_DOWN BIT04_MASK  //PowerDown Setting this bit to 1 enters the Soft PowerDown Mode. 
                                   //This means, internal current consuming blocks are switched off 
                                   //including the oscillator.
#define BIT_CRYPTO1ON  BIT03_MASK  //Crypto1On This bit indicates that the Crypto1 unit is switched on 
                                   //and therefore all data communication with the card is encrypted.
                                   //This bit can only be set to 1 by a successful execution of the Authent2 Command.
#define BIT_TSTOPNOW   BIT02_MASK  //TStopNow Setting this bit to 1 stops the timer immediately.
                                   //Reading this bit will always return 0.
#define BIT_TSTARTNOW  BIT01_MASK  //TStartNow Setting this bit to 1 starts the timer immediately.
                                   //Reading this bit will always return 0.
#define BIT_FLUSHFIFO  BIT00_MASK  //FlushFIFO Setting this bit to 1clears the internal FIFO-buffer��s 
                                   //read- and write-pointer,(FIFOLength becomes 0) and the flag FIFOOvfl 
                                   //immediately.Reading this bit will always return 0.

//--------------------------------------------------------------------------------
//reg errflag
//bit7: 0 Reserved for future use.
#define BIT_KEY_ERR       BIT06_MASK      //KeyErr This bit is set to 1, if the LoadKeyE2 or the LoadKey Command 
                                          //recognises, that the input data is not coded according to the Key format 
                                          //definition.This bit is set to 0 starting the LoadkeyE2 or the LoadKey command.
#define BIT_ACCESS_ERR    BIT05_MASK      //AccessErr This bit is set to 1, if the access rights to the E2PROM are violated.
                                          //This bit is set to 0 starting an E2PROM related command.
#define BIT_FIFO_OVERFLOW BIT04_MASK      //FIFOOvfl This bit is set to 1, if the ��-Processor or a MF RC531��s internal 
                                          //state machine,(e.g. receiver) tries to write data into the FIFO buffer although 
                                          //the FIFO buffer is already full.
#define BIT_CRC_ERR       BIT03_MASK      //CRCErr This bit is set to 1, if RxCRCEn is set and the CRC fails. It is 
                                          //cleared to 0 automatically at receiver start phase during the state PrepareRx.
#define BIT_FRAMING_ERR   BIT02_MASK      //FramingErr This bit is set to 1, if the SOF is incorrect. It is cleared 
                                          //automatically at receiver,start (that is during the state PrepareRx).
#define BIT_PARITY_ERR    BIT01_MASK      //ParityErr This bit is set to 1, if the parity check has failed. It is cleared 
                                          //automatically at receiver start (that is during the state PrepareRx).
#define BIT_COLL_ERR      BIT00_MASK      //CollErr This bit is set to 1, if a bit-collision is detected. It is cleared 
                                          //automatically at receiver start (that is during the state PrepareRx).
                                          //Note: only valid for communication according to ISO14443 A.

//--------------------------------------------------------------------------------
//reg channel redundancy
//7-6 00 This value shall not be changed
#define BIT_CRC3309  BIT05_MASK          //CRC3309 If set to 1, CRC-calculation is done according ISO/IEC3309 (ISO14443B)
                                         //Note: For usage according to ISO14443A this bit has to be 0.
#define BIT_CRC8     BIT04_MASK          //CRC8 If set to 1, an 8-bit CRC is calculated.
                                         //If set to 0, a 16-bit CRC is calculated.
#define BIT_RX_CRCEN BIT03_MASK          //RxCRCEn If set to 1, the last byte(s) of a received frame is/are 
                                         //interpreted as CRC byte/s.If the CRC itself is correct the CRC byte(s) is/are 
                                         //not passed to the FIFO.In case of an error, the CRCErr flag is set.If set to 0, 
                                         //no CRC is expected.
#define BIT_TX_CRCEN BIT02_MASK          //TxCRCEn If set to 1, a CRC is calculated over the transmitted data and the CRC 
                                         //byte(s) are appended to the data stream.If set to 0, no CRC is transmitted.
#define BIT_PARITY_ODD BIT01_MASK        //ParityOdd If set to 1, an odd parity is generated or expected, respectively.
                                         //If set to 0 an even parity is generated or expected, respectively.
                                         //Note: For usage according to ISO14443-A this bit has to be 1.
#define BIT_PARITY_EN BIT00_MASK         //ParityEn If set to 1, a parity bit is inserted in the transmitted data stream 
                                         //after each byte and expected in the received data stream after each byte 
                                          //(MIFARE?, ISO14443A). If set to 0, no parity bit is inserted or 
                                          //expected (ISO14443B)

//---------------------------------------------------------------------------------
//reg InterruptEn, InterruptRq
#define BIT_SETI        BIT07_MASK
#define BIT_TIMERI      BIT05_MASK
#define BIT_TXI         BIT04_MASK 
#define BIT_RXI         BIT03_MASK
#define BIT_IDLEI       BIT02_MASK
#define BIT_HIALERTI    BIT01_MASK
#define BIT_LOALERTI    BIT00_MASK

//COMMAND-------------------------------------------------------------------------
#define RC500_CMD_IDLE           0x00 // No action: cancel current command 
                                // or home state, respectively
#define RC500_CMD_WRITEE2        0x01 // Get data from FIFO and write it to the E2PROM
#define RC500_CMD_READE2         0x03 // Read data from E2PROM and put it into the 
                                // FIFO
#define RC500_CMD_LOADCONFIG     0x07 // Read data from E2PROM and initialise the 
                                // registers
#define RC500_CMD_LOADKEYE2      0x0B // Read a master key from the E2PROM and put 
                                // it into the master key buffer
#define RC500_CMD_AUTHENT1       0x0C // Perform the first part of the card 
                                // authentication using the Crypto1 algorithm.
                                // Remark: The master key is automatically taken                                // from 
                                // the master key buffer. this implies, that the
                                // command LoadKeyE2 has to be executed before 
                                // to use a certain key for card authentication
#define RC500_CMD_CALCCRC        0x12	// Activate the CRC-Coprocessor
                                // Remark: The result of the CRC calculation can
                                // be read from the register CRCResultXXX
#define RC500_CMD_AUTHENT2       0x14	// Perform the second part of the card 
                                // authentication using the Crypto1 algorithm.
#define RC500_CMD_RECEIVE        0x16 // Activate Receiver Circuitry. Before the 
                                // receiver actually starts, the state machine 
                                // waits until the time 
                                // configured in the register RcvWait has 
                                // passed.
                                // Remark: It is possible to read data from the 
                                // FIFO although Receive is active. Thus it is 
                                // possible to receive any number of bytes by 
                                // reading them from the FIFO in time.
#define RC500_CMD_LOADKEY        0x19 // Read a master key from the FIFO and put it 
                                // into the master key buffer
                                // Remark: The master key has to be prepared in
                                // a certain format. Thus, 12 byte have to be 
                                // passed to load a 6 byte master key
#define RC500_CMD_TRANSMIT       0x1A // Transmit data from FIFO to the card
                                // Remark: If data is already in the FIFO when 
                                // the command is activated, this data is 
                                // transmitted immediately. It is possible to 
                                // write data to the FIFO although Transmit 
                                // is active. Thus it is possible to transmitt 
                                // any number of bytes by writting them to the 
                                // FIFO in time. 
#define RC500_CMD_TRANSCEIVE     0x1E	// Transmit data from FIFO to the card and after 
                                // that automatically activates the receiver. 
                                // Before the receiver actually starts, the 
                                // STATE MACHINE waits until the time configured
                                //  in the register RcvWait has passed.
                                // Remark: This command is the combination of 
                                // Transmit and Receive
#define RC500_CMD_RESETPHASE     0x3F // Runs the Reset- and Initialisation Phase
                                // Remark: This command can not be activated by 
                                // software, but only by a Power-On or 
                                // Hard Reset 
                                
//-----------------------------------------------------------------------------------------------------------                                
#define PICC_REQSTD        0x26			// request idle
#define PICC_REQALL        0x52			// request all
#define PICC_ANTICOLL1     0x93			// anticollision level 1
#define PICC_ANTICOLL2     0x95			// anticollision level 2
#define PICC_ANTICOLL3     0x97			// anticollision level 3
#define PICC_AUTHENT1A     0x60			// authentication step 1
#define PICC_AUTHENT1B     0x61			// authentication step 2
#define PICC_READ          0x30			// read block
#define PICC_WRITE         0xA0			// write block
#define PICC_DECREMENT     0xC0         	// decrement value
#define PICC_INCREMENT     0xC1         	// increment value
#define PICC_RESTORE       0xC2         	// restore command code
#define PICC_TRANSFER      0xB0         	// transfer command code
#define PICC_HALT          0x50			// halt

//Definitions for lower Anticollision / Select functions.
#define BITS_PER_BYTE               0x08
#define UPPER_NIBBLE_SHIFT          0x04
#define COMPLETE_UID_BITS           0x28
#define NVB_MIN_PARAMETER           0x20
#define NVB_MAX_PARAMETER           0x70
#define SELECT_CASCADE_LEVEL_1      0x93
#define SELECT_CASCADE_LEVEL_2      0x95
#define SELECT_CASCADE_LEVEL_3      0x97
#define MAX_CASCADE_LEVELS          0x03
#define SINGLE_UID_LENGTH           0x20
#define CASCADE_BIT                 0x04

//-----------------------------------------------------------------------------------------------------
#define STATUS_IO_TIMEOUT               (0x0001)  /*!< No reply received, e.g. PICC removal.    */
#define STATUS_CRC_ERROR                (0x0002)  /*!< Wrong CRC detected by RC or library.     */
#define STATUS_PARITY_ERROR             (0x0003)  /*!< Parity error detected by RC or library.  */
#define STATUS_BITCOUNT_ERROR           (0x0004)  /*!< Typically, the RC reports such an error. */
#define STATUS_FRAMING_ERROR            (0x0005)  /*!< Invalid frame format.                    */
#define STATUS_COLLISION_ERROR          (0x0006)  /*!< Typically, the RC repors such an error.  */
#define STATUS_BUFFER_TOO_SMALL         (0x0007)  /*!< Communication buffer size insufficient.  */
#define STATUS_ACCESS_DENIED            (0x0008)  /*!< Access has not been granted (readonly?). */
#define STATUS_BUFFER_OVERFLOW          (0x0009)  /*!< Attempt to write beyond the end of a
                                                          buffer.                                  */
#define STATUS_PROTOCOL_ERROR           (0x000B)  /*!< Mifare start bit wrong, buffer length
                                                          error.                                   */
#define STATUS_ERROR_NY_IMPLEMENTED     (0x000C)  /*!< Feature not yet implemented.             */
#define STATUS_FIFO_WRITE_ERROR         (0x000D)  /*!< Error caused because of interface conflict
                                                          during write access to FIFO.             */
#define STATUS_USERBUFFER_FULL          (0x000E)  /*!< The user buffer is full, the calling
                                                          application/routine gets the chance to
                                                          save user buffer data and start over.    */
#define STATUS_ACK_SUPPOSED             (0x000f)  /*!< Single byte or nibble received, CRC error
                                                          detected, possibly MF (N)ACK response.            */
//��������
extern UBYTE gRc500Key[6];
extern UBYTE gThisCardSnr[16];
extern UBYTE gNhhFlag;

//��������:��ǿ�ɶ���
#define rc_crc_enable()        rc_set_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT_RX_CRCEN|BIT_TX_CRCEN)
#define rc_crc_disable()       rc_clr_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT_RX_CRCEN|BIT_TX_CRCEN)
#define rc_crc_rx_enable()     rc_set_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT_RX_CRCEN)
#define rc_crc_rx_disable()     rc_clr_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT_RX_CRCEN)
#define rc_oddpari_enable()    rc_set_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT_PARITY_EN|BIT_PARITY_ODD)
#define rc_pari_disable()      rc_clr_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT_PARITY_EN)
#define rc_set_bitframe(bits_cnt)  rc_write_byte(REG_RC500_BIT_FRAMING,bits_cnt)
#define rc_flush_fifo()            rc_set_bits(REG_RC500_CONTROL,BIT_FLUSHFIFO)
#define rc_write_data(indata)      rc_write_byte(REG_RC500_FIFO_DATA,indata)
#define rc_write_data_n(inbuf,inbytes)    rc_write_bytes(REG_RC500_FIFO_DATA,inbuf,inbytes)
#define rc_read_data()                    rc_read_byte(REG_RC500_FIFO_DATA)
#define rc_read_data_n(outbuf,outbytes)   rc_read_bytes(REG_RC500_FIFO_DATA,outbuf,outbytes)
#define rc_clr_crypt()                rc_clr_bits(REG_RC500_CONTROL,BIT_CRYPTO1ON)
#define rc_clr_irq()      {rc_write_byte(REG_RC500_INTERRUPT_EN,0x7f);rc_write_byte(REG_RC500_INTERRUPT_RQ,0x7f);} 
#define rc_power_on()      rc_set_bits(REG_RC500_TX_CONTROL,0x03)
#define rc_power_off()     rc_clr_bits(REG_RC500_TX_CONTROL,0x03)
#define rc_idle()          rc_write_byte(REG_RC500_COMMAND,RC500_CMD_IDLE)
#define rc_in_anticoll()       rc_set_bits(REG_RC500_DECODER_CONTROL,BIT05_MASK)
#define rc_not_in_anticoll()   rc_clr_bits(REG_RC500_DECODER_CONTROL,BIT05_MASK)
#define rc_select_mifare_auth()  {rc_clr_bits(REG_RC500_CRYPTO_SELECT,BIT00_MASK);gNhhFlag=0;}
#define rc_select_sh_auth()      {rc_set_bits(REG_RC500_CRYPTO_SELECT,BIT00_MASK);gNhhFlag=0x55;}
#define rc_crc_sel_15693()         rc_set_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT05_MASK)
#define rc_crc_sel_14443a()        rc_clr_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT05_MASK)
#define rc_crc_sel_14443b()        rc_set_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT05_MASK)
#define rc_crc_sel_icode1()        rc_clr_bits(REG_RC500_CHANNEL_REDUNDANCY,BIT05_MASK)
#define rc_clr_time_out()         {rc_set_bits(REG_RC500_CONTROL,BIT_TSTOPNOW);} 

//��������--------------------------------------------------------------------------
void rc_write_byte(UBYTE addr,UBYTE inbyte);
void rc_write_bytes(UBYTE addr,UBYTE *inbuf,UBYTE inbytes);
UBYTE rc_read_byte(UBYTE addr);
void rc_read_bytes(UBYTE addr,UBYTE *outbuf,UBYTE outbytes);
void rc_set_time_out(UWORD cnt_150us);
void  rc_clr_bits(UBYTE addr,UBYTE mask);
void  rc_set_bits(UBYTE addr,UBYTE mask);


//end of file
#endif

