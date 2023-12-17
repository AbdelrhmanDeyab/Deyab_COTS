#ifndef ADC_REG_H
#define ADC_REG_H

#define ADMUX                        *((volatile uint8*)0x27)  /* ADC Multiplexer selection register*/
#define ADMUX_REFS1                  7u                        /*Refrence selection bit1*/
#define ADMUX_REFS0                  6u                        /*Refrence selection bit0*/
#define ADMUX_ADLAR                  5u                        /*left adjust result*/

#define ADCSRA                       *((volatile uint8*)0x26)  /*ADC control and status register*/
#define ADCSRA_ADEN                  7u                        /*Enable*/
#define ADCSRA_ADSC                  6u                        /*start convertion*/
#define ADCSRA_ADATE                 5u                        /*auto trigger enable*/
#define ADCSRA_ADIF                  4u                        /*interrupt flag*/
#define ADCSRA_ADIE                  3u                        /*interrupt enable*/


#define ADCH                         *((volatile uint8*)0x25)   /*ADC Data high register */
#define ADCL                         *((volatile uint8*)0x24)   /*ADC Data low register*/

#define ADC                          *((volatile uint16*)0x24)





#endif