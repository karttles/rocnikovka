#define F_CPU 16000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>		/* Include AVR std. library file */
#include <stdio.h>
#include <util/delay.h>		/* Include delay header file */
#include <time.h>
#include <string.h>
#include "USART_RS232_H_file.h"


typedef struct Souradnice{
	int x, y;
} Souradnice;

Souradnice curPozice = { .x = 0, .y = 0};
void nalozit(){
	
	for(int i=125;i<500;i++)
	{
		OCR1A = i;
		_delay_ms(3);
	}
	USART_SendString("nalozit");
}

void vylozit(){
	
	for (int i=500;i>125;i--)
	{
		OCR1A = i;
		_delay_ms(3);
		
	}
	USART_SendString("vylozit");
}
void doprava(){
	for (int i = 0; i<200; i++)
	{
		_delay_ms(1);
		PORTA = 0b00010011;
		_delay_ms(1);
		PORTA = 0b00010001;
	}
}
void doleva(){
	for (int i = 0; i<200; i++)
	{
		_delay_ms(1);
		PORTA = 0b00000010;
		_delay_ms(1);
		PORTA = 0b00000000;
	}
}
void nahoru(){
	for (int i = 0; i<200; i++)
	{
		_delay_ms(1);
		PORTA = 0b00001000;
		_delay_ms(1);
		PORTA = 0b00000000;
	}
}
void dolu(){
	for (int i = 0; i<200; i++)
	{
		_delay_ms(1);
		PORTA = 0b00001100;
		_delay_ms(1);
		PORTA = 0b00000100;
	}
}

int tryParseSouradnice(char* input, Souradnice* souradniceToLoad){
	char x = 0;
	int y = 0;
	
	if(sscanf(input, " %c%d", &x, &y) != 2){
		// Nepovedlo se to naparsovat ocekvali jsme že to parsne 2 v?ci ale parsnulo to m?n? v?cí
		return 1;
	}
	souradniceToLoad->x = x - 'a';
	souradniceToLoad->y = y - 1;
	return 0;
}

void move(Souradnice* start, Souradnice cil){
	while(start->x < cil.x){
		doprava();
		start->x += 1;
	}
	while(start->x > cil.x){
		doleva();
		start->x -= 1;
	}
	while(start->y < cil.y){
		nahoru();
		start->y += 1;
	}
	while(start->y > cil.y){
		dolu();
		start->y -= 1;
	}
}
// move a1
int vykonej(char* commandStr){
	char prikaz[20];

	// a1
	
	sscanf(commandStr, "%s", prikaz);
	char* argumenty = commandStr + strlen(prikaz);
	if(strcmp(prikaz, "move") == 0){

		
		Souradnice cil;
		if(tryParseSouradnice(argumenty, &cil) != 0){
			printf("Big oof nepovedlo se naparasovat sou?adnice\n");
		}
		
		move(&curPozice, cil);
		return 0;
		
	}
	else if(strcmp(prikaz, "nalozit") == 0){
		nalozit();
		
	}
	else if(strcmp(prikaz, "vylozit") == 0){
		vylozit();
		
	}
	else{
		printf("P?ikaz neznám");
		return 1;
	}
	return 0;
}

int main()
{
	TCNT1 = 0;
	ICR1 = 2499;
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	
	USART_Init(9600);
	char* Data_in;
	DDRA = 0b11111111;
	DDRD |= (1<<PD5);		
	PORTA = 0b00000000;
	while(1)
	{
		Data_in = USART_ReceiveString();
		USART_SendString(Data_in);
		vykonej(Data_in);
	}
}
