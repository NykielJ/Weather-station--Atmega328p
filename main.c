/*
 * Czujnik i LCD.c
 *
 * Created: 21.01.2023 15:59:21
 * Author : jnyki
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

/* Biblioteki */
#include "lcd_displ.h"
#include "USART.h"
#include "common.h"

/* Czujnik i pomocnicze zmienne */
#define DHT11_PIN 0
#define S1_PIN 1

uint8_t c=0,I_W,D_W,I_Temp,D_Temp,CheckSum, wart;

 void Req()				/* Wyslanie ¿¹dania do Czujki/ jakiegoœ pulsu  */
 {
	 DDRB |= (1<<DHT11_PIN);	/*Ustawienie wejœcia */
	 PORTB &= ~(1<<DHT11_PIN);	/* Ustawienie stanu niskiego na pinie */
	 _delay_ms(20);			/* Se czekam 20ms */
	 PORTB |= (1<<DHT11_PIN);	/* Ustawianie stanu wysokiego na pinie */
 }

 void Odpowiedz()				/* OdpowiedŸ od czujki DHT11 */
 {
	 DDRB &= ~(1<<DHT11_PIN);	// ustawienie wejœcia
	 PORTB |= (1<<DHT11_PIN);	//Rzucanie stanu wysokiego pull up resistor
	 while(PINB & (1<<DHT11_PIN));	//czeka na sygna³ od DHT11
	 while((PINB & (1<<DHT11_PIN))==0);	//Sprawdza czy jest stan niski aby odebraæ dane
	 while(PINB & (1<<DHT11_PIN));		//Odbiera dane
 }

uint8_t Receive_data8bit()			/* Otrzymywanie danych 8 bitowych  */	
{
	for (int q=0; q<8; q++)
	{
		while((PINB & (1<<DHT11_PIN)) == 0);  /* sprawdza otrzymany bit 0 lub 1 */
		_delay_us(30);
		if(PINB & (1<<DHT11_PIN))/* Jeœli stan wysoki jest wiêkszy ni¿ 30 ms bo w dokumentacji tak jest */
		c = (c<<1)|(0x01);										/* wtedy ma 1, bo Operacja c<<1 przesuwa bity w liczbie c o jeden bit w lewo. Operacja 0x01 oznacza wartoœæ bina? 01. Operacja | (or) ³¹czy bity z dwoma liczbami w jednym wyniku.
																Dla przyk³adu, jeœli c=1101, wykonana operacja bêdzie wygl¹daæ nastêpuj¹co:
																(1101 << 1) | (0x01)
																= (1010) | (0x01)
																= 1011  */
		else													/* W przeciwnym razie daje niski czyli 0 */
		c = (c<<1);
		while(PINB & (1<<DHT11_PIN));
	}
	return c;
}

void Inicjalizacja() /*Pokazuje W= i T= oraz za³¹cza lcd */
{		
		lcd_clear();
		lcd_gotoxy(0,0);
		lcd_printf("W = ");
		lcd_gotoxy(0,1);
		lcd_printf("T = ");
}

void czystyEkran()
{
	lcd_clear();
	lcd_init();
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_clear();
	lcd_gotoxy(0,1);
	lcd_clear();
	
}

void Pomiar()
{
Req();		/* wyœlij impuls startowy */
Odpowiedz();		/* Otrzymuje odpowieŸ */
I_W=Receive_data8bit();	/* Przechowuje pierwsze 8 bitów */
D_W=Receive_data8bit();	/* Przechowuje kolejne 8 bitów */
I_Temp=Receive_data8bit();	/* Przechowuje kolejne 8 bitów */
D_Temp=Receive_data8bit();	/* Przechowuje kolejne 8 bitów */
CheckSum=Receive_data8bit();/* Przechowuje kolejne 8 bitów */	
}


void Start_BT()
{
	char Start[]= "Projekt czujnika temperatury i wilgotnosci, ktory moze byc zamontowany w Saunie";
	Transmit_string(Start);
	USART_SendLine();
	Transmit_string("Wykonal Jakub Nykiel");
	USART_SendLine();
	Transmit_string("Wcisnij przycisk S1");
	USART_SendLine();
	
}

void Start_Ekran() /*Pokazuje W= i T= oraz za³¹cza lcd */
{
	lcd_clear();
	lcd_init();
	lcd_gotoxy(0,0);
	lcd_printf("Wcisnij ");
	lcd_gotoxy(0,1);
	lcd_printf("Przycisk ");
}

void Dioda(int g)
{	
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<WGM20);
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22);	//Prescaler 64
	OCR2A = g;
	DDRB |= (1<<PB3);
	PORTB |= (1<<PB3);

}

void DiodaOpadajaca(int g)
{
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2A0);
	TCCR2A |= (1<<WGM20);
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22);	//Prescaler 64
	OCR2A = g;
	DDRB |= (1<<PB3);
	PORTB |= (1<<PB3);

}



int main(void)
{		USART_init();	// inicjalizacja USART
		Start_BT();
		Start_Ekran();
		
	while(1)
	{	
		
		if((!(PINB & (1<<S1_PIN))==0))
		{	
			Pomiar();
			char data1[5];	//Przed przecinkiem
			char data2[5];	//Po przecinku
			if ((I_W + D_W + I_Temp + D_Temp) != CheckSum)		/* Sprawdzanie czy dane siê zapisa³y bo jak siê Ÿle zapisz¹ (czyli wgl) to error siê wyœwietli i elo */
			{
				lcd_gotoxy(0,0);
				lcd_printf(" Error ");
				lcd_gotoxy(0,1);
				lcd_printf(" brak pomiaru");
				Transmit_string("Error - brak pomiaru");
				USART_SendLine();
				for(int i=0; i<270; i=i+50)
				{
					DiodaOpadajaca(60);
					_delay_ms(500);
					DiodaOpadajaca(1000);
					_delay_ms(500);
				}
			}
			else
			{	
				PORTB =~(1<<PB3);
				//Dioda
				if (I_W>=0 && I_W<30)
				{
					Dioda(10);
				}
				if (I_W>=30 && I_W<50)
				{
					Dioda(60);
				}
				if (I_W>=50 && I_W<70)
				{
					Dioda(120);
				}
				if (I_W>=70 && I_W<=80)
				{
					Dioda(160);
				}
				if (I_W>80 && I_W<90)
				{
					Dioda(180);
				}
				if (I_W>=90 && I_W<=101)
				{
					Dioda(225);
				}

				//Wilgoæ
				itoa(I_W,data1,10);
				itoa(D_W,data2,10);
				lcd_gotoxy(0,0);
				lcd_printf("W = ");
				lcd_gotoxy(4,0);
				lcd_printf(data1); // Procent
				lcd_printf(".");
				lcd_printf(data2);
				lcd_printf("[Proc]");

				//Wysy³anie wilgoci usartem do  kompa
				Transmit_string(" WILGOC W % ");
				USART_SendLine();
				Transmit_string(data1);
				Transmit_string(".");
				Transmit_string(data2);
				Transmit_string("%");
				USART_SendLine();
				
				//Temperatura
				itoa(I_Temp,data1,10);
				itoa(D_Temp,data2,10);
				lcd_gotoxy(0,1);
				lcd_printf("T = ");
				lcd_gotoxy(4,1);
				lcd_printf(data1); // Stopnie Celsjusza
				lcd_printf(".");
				lcd_printf(data2);
				lcd_printf("[st.C]");
				
				//Wysy³anie usartem przez BT
				Transmit_string(" Temperatura W stopniach Celsjusza ");
				USART_SendLine();
				Transmit_string(data1);
				Transmit_string(".");
				Transmit_string(data2);
				Transmit_string(" [st. C]");
				USART_SendLine();
			}
			wart = 200;
			if (wart > 100)
			{
				DDRB |= (1<<PB2);
				PORTB |= (1<<PB2);
				_delay_ms(500);
				PORTB &=~(1<<PB2);
				_delay_ms(500);
			}
		}
		_delay_ms(500);		
	}
}
