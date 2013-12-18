// PIC32MX795F512L Config Bits

#include <xc.h>
#include "FT800.h"
#include <p32xxxx.h>
#include <plib.h>

// DEVCFG3
// USERID = No Setting
#pragma config FSRSSEL   = PRIORITY_7   // SRS Select (SRS Priority 7)
#pragma config FMIIEN    = OFF          // Ethernet RMII/MII Enable (MII Enabled)
#pragma config FETHIO    = OFF          // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config FCANIO    = OFF          // CAN I/O Pin Select (Default CAN I/O)
#pragma config FUSBIDIO  = ON           // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = OFF          // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV  = DIV_4        // PLL Input Divider (2x Divider)
#pragma config FPLLMUL   = MUL_20       // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV  = DIV_4        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN    = ON           // USB PLL Enable (Enabled)
#pragma config FPLLODIV  = DIV_1        // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC     = PRIPLL       // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN   = ON           // Secondary Oscillator Enable (Enabled)
#pragma config IESO      = OFF          // Internal/External Switch Over (Enabled)
#pragma config POSCMOD   = XT           // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC  = OFF          // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV    = DIV_1        // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM     = CSDCMD       // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS     = PS1048576    // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN    = OFF          // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG     = OFF          // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL    = ICS_PGx2     // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP       = OFF          // Program Flash Write Protect (Disable)
#pragma config BWP       = OFF          // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP        = OFF          // Code Protect (Protection Disabled)


void wait3secs(void){
    __delay_ms(3000);
}

int_32t main(void) {
    //SpiChnOpen(1, SPICON_MSTEN | SPICON_CKE | SPICON_ON, 20);
    mPORTDSetPinsDigitalOut(BIT_0 | BIT_10); //Set port as output
    mPORTCSetPinsDigitalIn(BIT_4); //Set port as output
    /*
    TRISDbits.TRISD10 = 0;  //SCK1
    TRISDbits.TRISD0  = 0;  //SDO1
    TRISCbits.TRISC4  = 1;  //SDI1
     */

    PIN_CS = 1;
    PIN_PD = 1;

    PIN_CS_dir = 0;
    PIN_PD_dir = 0;

    SPI1BRG = 0x0003; // use FPB/4 clock frequency
    SPI1CON = 0x8120; // SPI ON, 8-bit communication 

    FT800_Init();

    cmd(CMD_LOGO);
    cmd_exec();
    while(rd16(REG_CMD_WRITE) != 0){}
    __delay_ms(3000);
    cmd_wait();
    while( (0 != rd16(REG_CMD_WRITE)) && (rd16(REG_CMD_WRITE) != rd16(REG_CMD_READ) ));//Wait till both read & write pointer register are equal to zero
    
    __delay_ms(500);
    cmd(CMD_DLSTART);
    cmd(CLEAR_COLOR_RGB(255, 0, 0));
    cmd(CLEAR(1, 1, 1));
    cmd_text(240, 136, 27, OPT_CENTER, "Please tap the dots to calibrate the display");
    //cmd(DISPLAY()); // display the image
    //cmd(CMD_SWAP);
    cmd_calibrate();
    cmd_exec();
    cmd_wait();
    //DelayMs(8000);
    ///*
    // example Display List
    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1)); // clear screen
    /*cmd(BEGIN(BITMAPS)); // start drawing bitmaps
    cmd(VERTEX2II(190, 110, 31, 'M')); // ascii I
    cmd(VERTEX2II(220, 110, 31, 'A')); // ascii F in font 31
    cmd(VERTEX2II(244, 110, 31, 'M')); // ascii T
    cmd(VERTEX2II(270, 110, 31, 'A')); // ascii D
    cmd(VERTEX2II(299, 110, 31, 'N')); // ascii I
    cmd(END());*/
    /*
    cmd(COLOR_RGB(160, 22, 22)); // change color to red
    cmd(POINT_SIZE(320)); // set point size to 20 pixels in radius
    cmd(BEGIN(POINTS)); // start drawing points
    cmd(VERTEX2II(192, 133, 0, 0)); // red point
    cmd(END());*/

    cmd_button(10, 10, 140, 100, 31, 0, "Press!");
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();
    //*/

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_text(10, 10, 31, 0, "Ceci n'est pas un test.");
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_button(10, 10, 50, 25, 26, 0, "Ein");
    cmd_button(10, 40, 50, 25, 26, 0, "Zwei");
    cmd_button(10, 70, 50, 25, 26, 0, "Drei");
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_keys(2, 2, 450, 63, 20, 0, "qwertyuiop");
    cmd_keys(2, 68, 450, 63, 20, 0, "asdfghijkl");
    cmd_keys(2, 136, 450, 63, 20, 0, "zxcvbnm");
    cmd_button(2, 204, 450, 63, 20, 0, "");
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_keys(22, 1, 116, 28, 29, 0, "789");
    cmd_keys(22, 31, 116, 28, 29, 0, "456");
    cmd_keys(22, 61, 116, 28, 29, 0, "123");
    cmd_keys(22, 91, 116, 28, 29, 0, "0.");
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_clock(80, 60, 50, 0, 8, 15, 0, 0);
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_gauge(80, 60, 50, 0, 5, 4, 30, 100);
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd_gradient(0, 0, 0x0000ff, 160, 0, 0xff0000);
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    wait3secs();

    cmd(CMD_DLSTART);
    cmd(CLEAR(1, 1, 1));
    cmd(SCISSOR_XY(20, 40));
    cmd(SCISSOR_SIZE(120, 32));
    cmd_gradient(20, 0, 0x606060, 140, 0, 0x404080);
    cmd_text(23, 40, 29, 0, "Heading 1");
    cmd(DISPLAY()); // display the image
    cmd(CMD_SWAP);
    cmd_exec();

    while(1){};
}