#ifndef ADC_VALUE_H
#define ADC_VALUE_H

#define ADC_WIN	8
#define ADC_SUM	8
#define ADC_AVG	9
#define ADC_TOTAL 10

enum
{
	ADC_HEATER_1 = 0,
	ADC_HEATER_2,
	ADC_PT1000_1,
	ADC_PT1000_2,
	ADC_PT1000_3,
	ADC_PT1000_4,
	ADC_PT1000_5,
	ADC_PT1000_6,

};

void ADC_init(void);
void reflush_ADC_DMA(void);
uint32_t read_ADC_value(uint8_t ch);



#endif
