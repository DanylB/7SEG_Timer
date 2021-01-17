
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define NUM PORTB
#define RANK PORTC

unsigned int rank_1, rank_2, rank_3, rank_4;
unsigned char count_rank = 1;
int timer_num = 9999;

int led_num [] = {
	0b00111111,//0
	0b00000110,//1
	0b01011011,//2
	0b01001111,//3
	0b01100110,//4
	0b01101101,//5
	0b01111101,//6
	0b00000111,//7
	0b01111111,//8
	0b01101111 //9
};


void split_num(){
	rank_1 = timer_num/1000;
	rank_2 = timer_num%1000/100;
	rank_3 = timer_num%100/10;
	rank_4 = timer_num%10;
}


ISR(TIMER1_COMPA_vect){
	timer_num < 0 ? timer_num = 9999 : timer_num--;
}

int main(void){
	DDRB = 0xFF;
	DDRC = 0b00001111;

	TCCR1B = 0b000000011; // Предделитель на 64 = (1000000гц/64) = 15625 Гц
	TIMSK |= (1<<OCIE1A); // прерывания при совпадении
	OCR1A = 15625; // регистр сравнения 15625
	TCNT1 = 0;
	TCCR1B |= (1<<WGM12); // сброс при совпадении
	sei(); // разрешить прерывания

	
	while (1)
	{
		split_num();
		
		if(count_rank == 1){ RANK = ~0b00000001; NUM = led_num[rank_1];}
		_delay_ms(1);
		if(count_rank == 2){ RANK = ~0b00000010; NUM = led_num[rank_2];}
		_delay_ms(1);
		if(count_rank == 3){ RANK = ~0b00000100; NUM = led_num[rank_3];}
		_delay_ms(1);
		if(count_rank == 4){ RANK = ~0b00001000; NUM = led_num[rank_4];}
		_delay_ms(1);

		count_rank > 4 ? count_rank = 1 : count_rank++;
	}
}