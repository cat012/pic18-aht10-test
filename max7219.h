// max7219.h
// max7219 8-digit display driver for PIC18
// (^^)~ beta 16 09 '23


#ifndef MAX7219_H_
#define MAX7219_H_


#include <xc.h>
#include <stdint.h>


#define MAX7219_DIN_PIN   PORTBbits.RB0
#define MAX7219_DIN_TRIS  TRISBbits.RB0
#define MAX7219_DIN_LAT   LATBbits.LATB0

#define MAX7219_CS_PIN   PORTBbits.RB1
#define MAX7219_CS_TRIS  TRISBbits.RB1
#define MAX7219_CS_LAT   LATBbits.LATB1

#define MAX7219_CLK_PIN   PORTBbits.RB2
#define MAX7219_CLK_TRIS  TRISBbits.RB2
#define MAX7219_CLK_LAT   LATBbits.LATB2


#define MAX7219_DIN_CLR  MAX7219_DIN_LAT=0
#define MAX7219_DIN_SET  MAX7219_DIN_LAT=1
#define MAX7219_DIN_OUT  MAX7219_DIN_TRIS=0
#define MAX7219_DIN_INP  MAX7219_DIN_TRIS=1

#define MAX7219_CS_CLR  MAX7219_CS_LAT=0
#define MAX7219_CS_SET  MAX7219_CS_LAT=1
#define MAX7219_CS_OUT  MAX7219_CS_TRIS=0
#define MAX7219_CS_INP  MAX7219_CS_TRIS=1

#define MAX7219_CLK_CLR  MAX7219_CLK_LAT=0
#define MAX7219_CLK_SET  MAX7219_CLK_LAT=1
#define MAX7219_CLK_OUT  MAX7219_CLK_TRIS=0
#define MAX7219_CLK_INP  MAX7219_CLK_TRIS=1



void max7219_init(void);
void max7219_bright(uint8_t value);
void max7219_shutdown(uint8_t mod);

void max7219_char(uint8_t xpos, uint8_t code);
void max7219_print(uint8_t xpos, const char *str);
void max7219_update(void);



#endif /* MAX7219_H_ */
