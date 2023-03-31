/*
 * File:   spi_read_write.c
 * Author: nandi
 *
 * Created on 23 December 2021, 14:25
 */


#include <xc.h>
#include "config.h"

#define ldata PORTD      //Declare ldata variable for PORTD
#define rs PORTEbits.RE0 //Declare rs variable for pin RE0
#define rw PORTEbits.RE1 //Declare rw variable for pin RE1
#define en PORTEbits.RE2 //Declare en variable for pin RE2
#define msec 50

#define SCK RC3 //Serial Clock
#define SDI RC4 //Serial Data In
#define SD0 RC5 //Serial Data Out
#define SS RA5 //Slave Select: Not used in this application
#define SCK_dir TRISC3
#define SDI_dir TRISC4
#define SDO_dir TRISC5
#define SS_dir TRISA5
#define WREN 0x06 //Write enable
#define WRDI 0x04 //Write Disable
#define WRITE 0x02
#define READ 0x03
#define RDSR 0x05 //Read Status Register
//----------------------Common Anode 7-Segment Codes----------------------------

char data[14] = {'p','r','o','t','e','u','s','-','e','e','p','r','o','m'};
/* Addresses are 16 bit(hence int data type) wide <ie 4 nibbles>(According to Datasheet info). NB: Chosen at Random */
//------------------------------------------------------------------------------
int address[14] = {0x0103,0x0B31,0x1032,0x0C23,0x6004,0xC0BF,0x10C6,0x0317,0x0AF8,0x0A19,0x102A,0x0CCB,0x1388,0x612D};
unsigned char commands[] = {0x38,0x0E,0x01,0x06,0x81,'\0'};

void EEPROM_write(int address,char data);
char EEPROM_read(int address);
char EEPROM_status();
char EEPROM_busy();
char EEPROM_wrEnable();

void lcdcmd(unsigned char value);//Function Prototype declaration
void lcddata(unsigned char value);
void msdelay(unsigned int itime);

void delay(unsigned int nMilliseconds)
{
    #define CYCLES_PER_MS 100 /* Number of decrement-and-test cycles. */
    unsigned long nCycles = nMilliseconds * CYCLES_PER_MS;
    while (nCycles--);
}
void SPI_init()
{
    SS_dir = 1; //Diselect chip initially
    SCK_dir = 0;
    SDI_dir = 1;
    SDO_dir = 0;
    SS_dir = 0;
    //SS_dir = 0; //Disable: Because operating in master mode

    SSPSTAT = 0x80; //SMP = 1: Sample bit. Input data sampled at end of data output time. CKE = 0: SPI Clock Edge Select bit CLEAR
    SSPCON1 = 0x30; //SSPEN = 1: Enable Serial Port. CKP = 1: Clock Polarity Select bit: Idle state for clock is a high level
}
//------------------------------------------------------------------------------
void SPI_write(char data)
{
    SSPBUF = data;
    while(BF == 0);

}
//-----------------------------------------------------------------------------
char SPI_read(char dummy)
{
    SSPBUF = dummy; //Assign to clear
    while(BF == 0);

    return SSPBUF;
}


void main(void) 
{
    SPI_init();
    TRISD = 0x00;  //Set direction of PORTD as output
    ADCON1=0X0F;
    TRISE=0X00;     //set direction of PORTE as output
    msdelay(50);
    //TRISD = 0;
    //PORTD = data[0];
    
    for( int i = 0 ; i < 14 ; i++ )
        EEPROM_write(address[i],data[i]);

    
    for(int i=0;commands[i]!='\0';i++)
    {
        lcdcmd(commands[i]);
        delay(100);
    }  
    for(int j=0;j<14;j++)
    {
        lcddata(EEPROM_read(address[j]));
        delay(100);
    }  
    //PORTD = EEPROM_read(address[i]); //Pass Address and Return value at that very address tp PORTD
    delay(500);
    return;
}
void EEPROM_write(int address,char data)
{
    while( EEPROM_busy() == 1 );

    //Set write enable latch
    SS = 0;
    SPI_write(WREN);
    SS = 1; //Latch-Enabled. use isWriteEn() if you want to verify

    //Start Write sequence(only occurs when/if Write latch is Enanled)
    SS = 0;
    SPI_write(WRITE);
    SPI_write(address>>8);
    SPI_write(address);
    SPI_write(data);
    SS = 1;
}
//------------------------------------------------------------------------------
char EEPROM_read(int address)
{
    while( EEPROM_busy() == 1 ); //Wait for previous Write/Read operations to finish

    SS = 0;
    SPI_write(READ);
    SPI_write(address>>8); //Upper Byte
    SPI_write(address); //Lower Byte
    char data = SPI_read(0);
    SS = 1;

    return data;
}
//------------------------------Used as a flag----------------------------------
char EEPROM_status()
{
    SS = 0;
    SPI_write(RDSR); //Intsruction: Read Status register of the EEPROM
    char status = SPI_read(0) ; //Send a Dummy parameter to read. Thus ANY int number is valid
    SS = 1;

    return status;
}
//------------------------------------------------------------------------------
char EEPROM_busy()
{
    char busy = EEPROM_status();

    return (busy & 0x01); //LSB is Write-In-Process (WIP) bit. When Set, a write is in progress, when Clear, no write is in progress
}
//------------------------------------------------------------------------------
char EEPROM_wrEnable()
{
    char isWrite = EEPROM_status();

    return (isWrite & 0x02); //Bit 2 is the Write Enable Latch (WEL) bit and indicates the status of the write enable latch
}

void lcdcmd (unsigned char value)
{
    ldata=value;     //Send the command value to PORTD
    rs=0;            //selection of command register of LCD
    rw=0;
    en=1;              //Generate High to Low pulse on Enable pin
    msdelay(1);
    en=0;
}
void lcddata (unsigned char value)
{
    ldata=value;  //Send the command value to PORTD
    rs=1;          //selection of DATA register of LCD
    rw=0;
    en=1;        //Generate High to Low pulse on Enable pin
    msdelay(1);
    en=0;
}
void msdelay (unsigned int itime)
{
    int i,j;
    for(i=0;i<itime;i++)
    for(j=0;j<135;j++);
}
