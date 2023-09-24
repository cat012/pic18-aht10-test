//16-09-2023


#ifndef AHT10_H
#define	AHT10_H


#include "swi2c.h"



void aht10_init(void);
void aht10_start_meas(void);
void aht10_read_data(uint8_t*, float*, float*);



#endif	/* AHT10_H */


