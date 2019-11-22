/*
 * LED Desk.c
 *
 * Created: 17/11/2019 3:02:53 PM
 * Author : Tan Run En
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <5Vlib/ADC.h>
#include <5Vlib/UART.h>

struct freq_amp{
	int delta_t;
	float amp;
};

typedef struct freq_amp Struct;

void pwm_init_0(){
	TCCR0A |= 1 << COM0A1 | 1 << COM0B1 | 1 << WGM01 | 1 << WGM00;	
	TCCR0B |= 1 << CS01;
}
void pwm_init_1(){
	ICR1 = 0xFFFF;
	TCCR1A |= 1 << COM1A1 | 1 << COM1B1| 1 << WGM11;
	TCCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS10;
}
void pwm_init_2(){
	TCCR2A |= 1 << COM2A1 | 1 << WGM21 | 1 << WGM20 | 1 << COM2B1;
	TCCR2B |= 1 << CS21;	
}

Struct get_frequency_amplitude(){
	Struct return_info;	
	int t = 1;
	int amp_avg = 0;
	if(adc_read(0) > 610){
		while(adc_read(0) > 610){
			t++;
			amp_avg += (adc_read(0) - 600)*9;
		}
	}
	return_info.delta_t = t;
	return_info.amp = amp_avg/t;
	return return_info;	
}

int main(void)
{    
	// OCR2B Green OCR0A Blue OCR0B Red
	DDRD = 0b11111111;
	DDRB = 0b11111111;
	pwm_init_0();
	pwm_init_2();
	adc_init();
	InitializeUART0(9600,0,8,NONE,1,1);
	Struct freq_amp_info;
	int freq_range[] = {0,2,4,6,8,10,12,14};
	int freq;
	float amp;
	int counter=0;
	bool lit = false;
    while (1) 
    {
		freq_amp_info = get_frequency_amplitude();
		//TransmitUART0(freq_amp_info.delta_t + '0');
// 		OCR0B = 0;
// 		OCR0A = 0;
// 		OCR2B = 0;
		if(freq_range[0] < freq_amp_info.delta_t && freq_range[1] > freq_amp_info.delta_t){
			OCR0B = freq_amp_info.amp;					
		}
		else if(freq_range[1] < freq_amp_info.delta_t && freq_range[2] > freq_amp_info.delta_t){
			OCR0A = freq_amp_info.amp;		
		}
		else if(freq_range[2] < freq_amp_info.delta_t && freq_range[3] > freq_amp_info.delta_t){
			OCR2B = freq_amp_info.amp;			
		}
		else if(freq_range[3] < freq_amp_info.delta_t && freq_range[4] > freq_amp_info.delta_t){
			OCR0B = freq_amp_info.amp;
			OCR0A = freq_amp_info.amp;		
		}
		else if(freq_range[4] < freq_amp_info.delta_t && freq_range[5] > freq_amp_info.delta_t){
			OCR0B = freq_amp_info.amp;
			OCR2B = freq_amp_info.amp;			
		}
		else if(freq_range[5] < freq_amp_info.delta_t && freq_range[6] > freq_amp_info.delta_t){
			OCR2B = freq_amp_info.amp;
			OCR0A = freq_amp_info.amp;	
		}
		else if(freq_range[6] < freq_amp_info.delta_t && freq_range[7] > freq_amp_info.delta_t){
			OCR2B = freq_amp_info.amp;
			OCR0A = freq_amp_info.amp;
			OCR0B = freq_amp_info.amp;	
		}
		else {
			OCR2B = 0;
			OCR0A = 0;
			OCR0B = 0;
		}
    }
	
}

