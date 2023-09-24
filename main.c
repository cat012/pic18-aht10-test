//18-09-2023


#include <stdio.h>
#include <stdint.h>

#include "main.h"
#include "max7219.h"
#include "aht10.h"


volatile uint8_t syscnt=0;

char strbuff[16];



//=============================================================================
void __interrupt isr_high(void)
    {
    if(TMR0IF && TMR0IE)  //timer0 overflow - 100Hz ticks
        {
        TMR0IF=0;
        TMR0=TMR0_OVF_PRELOAD;

        if(syscnt) syscnt--;
        }

    if(TMR1IF && TMR1IE)  //timer1 overflow
        {
        TMR1IF=0;
        TMR1=TMR1_OVF_PRELOAD;
        }
    }


//-----------------------------------------------------------------------------
void delay_ms(volatile uint16_t val)
    {
    while(val-->0) __delay_us(1000);
    }


//-----------------------------------------------------------------------------
void main(void)
    {
    IRCF2=1; IRCF1=1; IRCF0=1; //111=8M 110=4M 101=2M 100=1M 011=500k 010=250k 001=125k 000=31k

    //INTSRC PLLEN - TUN4 TUN3 TUN2 TUN1 TUN0
    OSCTUNE=0b00000000;

    SBOREN=1; //1=enable brown out reset
    SWDTEN=1; //0=disable watchdog timer
    RBPU=1;   //0=enable pull-up

    PEIE=1;
    GIE=1;

    //TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0
    T0CON=0b00000000 | TMR0_PRESCALER;
    TMR0=TMR0_OVF_PRELOAD;
    TMR0ON=1;
    TMR0IF=0;
    TMR0IE=1;

    //RD16 T1RUN T1CKPS1 T1CKPS0 T1OSCEN T1SYNC TMR1CS TMR1ON
    T1CON=0b00000000 | TMR1_PRESCALER;
    TMR1=TMR1_OVF_PRELOAD;
    TMR1ON=1;
    TMR1IF=0;
    TMR1IE=1;

    //- TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
    T2CON=0b00000000 | TMR2_PRESCALER;
    TMR2=TMR2_OVF_PRELOAD;
    TMR2ON=0;
    TMR2IF=0;
    TMR2IE=0;

    ////RD16 T3CCP2 T3CKPS1 T3CKPS0 T3CCP1 T3SYNC TMR3CS TMR3ON
    T3CON=0b00000000 | TMR3_PRESCALER;
    TMR3=TMR3_OVF_PRELOAD;;
    TMR3ON=0;
    TMR3IF=0;
    TMR3IE=0;

    //- - DCxB1 DCxB0 CCPxM3 CCPxM2 CCPxM1 CCPxM0
    CCP2CON=0b00000000;

    //P1M1 P1M0 DC1B1 DC1B0 CCP1M3 CCP1M2 CCP1M1 CCP1M0
    CCP1CON=0b00000000;

    CMCON=0b000111; //Comparators Off

    ADCON1 = 0b001101; //5-Vref=Vss //4+Vref=Vdd //AN0,AN1
    ADCON0 = 0b000000; //Channel 0  //A/D converter module is disabled
    //ADFM - ACQT2 ACQT1 ACQT0 ADCS2 ADCS1 ADCS0
    ADCON2 = 0b10010101; //TACQ 010=4TAD //TAD 000=FOSC/2 100=FOSC/4 101=FOSC/16 Fosc/4=0.5M=2us  //111=FRC
    ADON=0; //Turn off A/D module

    /*** external interrupt 0 settigs ***/
    INTEDG0=0; //on falling edge
    INT0IE=0; //enable

    INIT_V_SENS;
    UPIN_ALL_INIT;

    max7219_init();
    max7219_bright(3);

    aht10_init();
    aht10_start_meas();
    delay_ms(100);

    uint8_t scrupd=0;

    for(;;)
        {
        if(syscnt==0)
            {
            syscnt=EVENT_PERIOD(500);

            scrupd=0;
            CLRWDT();
            }

        if(scrupd==0)
            {
            float t=0;
            float h=0;
            uint8_t r=255;
            aht10_read_data(&r ,&t, &h);

            sprintf(strbuff, "%2.1f  %2.1f",t,h);
            max7219_print(0,strbuff);
            max7219_update();
            scrupd=1;

            aht10_start_meas();
            }
        }
    }

