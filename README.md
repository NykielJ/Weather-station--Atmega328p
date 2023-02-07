# Weather-station-- Atmega328p 

### Short description
POL

Projekt ma na celu monitorowanie parametrów środowiska, takich jak temperatura i wilgotność 
w pomieszczeniu. Czujnik DHT-11 ma za zadanie zbierać dane o tych parametrach, a następnie 
przekazywać je do innego urządzenia, aby wyświetlić odpowiednie wyniki na ekranie LCD oraz 
wysyłać przez komunikację UART, która jest wykorzystywana przez radiowy nadajnik HC-05 przez 
bluetooth do programu takiego jak np. Bluetooth Serial Terminal i wyświetlanie ich na terminalu

ENG

The project aims to monitor environmental parameters such as temperature and humidity
in the room. The DHT-11 sensor is designed to collect data about these parameters, and then
transfer them to another device to display the corresponding results on the LCD screen and
send via UART communication, which is used by the HC-05 radio transmitter via
bluetooth files to a program such as Bluetooth Serial Terminal and displaying them on the terminal


### Components
```cpp
• Microcotroler Arduino UNO with Atmega 328p
• LCD 2x16 HD44780
• Potentiometer 10k Ω
• LED White
• LED Red
• Sensor DHT11
• button type switch
• Bluetooth HC-05
• Resistors: 1kΩ, 330Ω
• Wires GPIO
```

### Main Wires

```javascript

'Atmega328p'   PD1 -> RX   'HC-05'
'Atmega328p'   PD4 -> D4   'LCD'
'Atmega328p'   PD5 -> D5   'LCD'
'Atmega328p'   PD6 -> D6   'LCD'
'Atmega328p'   PD7 -> D7   'LCD'
'Atmega328p'   PD2 -> RS   'LCD'
'Atmega328p'   PD3 -> E    'LCD'
'Atmega328p'   PB0 -> DATA  'DHT11'
'Atmega328p'   PB1  -> 'switch' -> Resistor 1kΩ
'Atmega328p'   PB2 -> +'LED_WHITE'
'Atmega328p'   PB3 -> +'LED_RED' 


'LCD'   VSS -> GND
'LCD'   VDD -> 5V
'LCD'   V0  -> 2'nd pin potentiometer
'LCD'   RW  -> GND
'LCD'   E   -> PD3 'Atmega328p'
'LCD'   D4  -> PD4 'Atmega328p'
'LCD'   D5  -> PD5 'Atmega328p'
'LCD'   D6  -> PD6 'Atmega328p'
'LCD'   D7  -> PD7 'Atmega328p'
'LCD'   A   -> Rezystor 1K Ω -> 5V
'LCD'   K   ->  GND

'DHT11' VCC   ->  5V
'DHT11' DATA  ->  PB0   'Atmega328p'
'DHT11' N     ->  N/o
'DHT11' GND   ->  GND

+'LED_RED'   -> PB3     'Atmega328p'
-'LED_RED'   -> Resistor 330 Ω -> GND
+'LED_WHITE'   -> PB2   'Atmega328p'
-'LED_WHITE'   -> Resistor 330 Ω -> GND

'HC-05' VCC -> 5V 
'HC-05' GND -> GND
'HC-05' RX  -> PD1   'Atmega328p'

```

### Source Code

This snippet presents piece of source code from `main.c`:

```cpp

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

/* Libs */
#include "lcd_displ.h"
#include "USART.h"
#include "common.h"
#define DHT11_PIN 0
#define S1_PIN 1
uint8_t c=0,I_W,D_W,I_Temp,D_Temp,CheckSum, wart;

 void Reqest()				
 {
	 DDRB |= (1<<DHT11_PIN);	
	 PORTB &= ~(1<<DHT11_PIN);	
	 _delay_ms(20);			
	 PORTB |= (1<<DHT11_PIN);	
	 _delay_us(30);
 }

 void Response()				
 {
	 DDRB &= ~(1<<DHT11_PIN);	
	 PORTB |= (1<<DHT11_PIN);	
	 while(PINB & (1<<DHT11_PIN));	
	 while((PINB & (1<<DHT11_PIN))==0);	
	 while(PINB & (1<<DHT11_PIN));	
 }

uint8_t Receive_data8bit()		
{
	for (int q=0; q<8; q++)
	{
		while((PINB & (1<<DHT11_PIN)) == 0); 
		_delay_us(30);
		if(PINB & (1<<DHT11_PIN))
		c = (c<<1)|(0x01);										
		else			
		c = (c<<1);
		while(PINB & (1<<DHT11_PIN));
	}
	return c;
}

void Init_lcd() 
{		
		lcd_clear();
		lcd_gotoxy(0,0);
		lcd_printf("H = ");
		lcd_gotoxy(0,1);
		lcd_printf("T = ");
}
void Measurement()
{
Req();		
Response();		
I_W=Receive_data8bit();
D_W=Receive_data8bit();	
I_Temp=Receive_data8bit();	
D_Temp=Receive_data8bit();	
CheckSum=Receive_data8bit();
}
void Start_BT()
{
	char Start[]= "Projekt czujnika temperatury i wilgotnosci, ktory moze byc zamontowany w Saunie";
	Transmit_string(Start);
	USART_SendLine();
	Transmit_string("Author: NykielJ");
	USART_SendLine();
	Transmit_string("Press the button S1");
	USART_SendLine();
	
}

void Start_LCD()
{
	lcd_clear();
	lcd_init();
	lcd_gotoxy(0,0);
	lcd_printf("Press ");
	lcd_gotoxy(0,1);
	lcd_printf("Button S1 ");
}

void LED_RED1(int g)
{	
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<WGM20);
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22);	
	OCR2A = g;
	DDRB |= (1<<PB3);
	PORTB |= (1<<PB3);

}

void LED_RED2(int g)
{
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2A0);
	TCCR2A |= (1<<WGM20);
	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22);	
	OCR2A = g;
	DDRB |= (1<<PB3);
	PORTB |= (1<<PB3);

}
void LED_White()
{
	DDRB |= (1<<PB2);
				PORTB |= (1<<PB2);
				_delay_ms(500);
				PORTB &=~(1<<PB2);
				_delay_ms(500);
}
int main(void)
{		USART_init();	
		Start_BT();
		Start_LCD();
		
	while(1)
	{	
		if((!(PINB & (1<<S1_PIN))==0))
		{	
			Measurement();
			char data1[5];	
			char data2[5];	
			if ((I_W + D_W + I_Temp + D_Temp) != CheckSum)		
			{
				lcd_gotoxy(0,0);
				lcd_printf(" Error ");
				lcd_gotoxy(0,1);
				lcd_printf(" Check conection ");
				Transmit_string("Error - check conection");
				USART_SendLine();
				for(int i=0; i<270; i=i+50)
				{
					LED_RED2(i);
					_delay_ms(500);
					LED_RED2(1000);
					_delay_ms(500);
				}
			}
			else
			{  	PORTB =~(1<<PB3);
				if (I_W>=0 && I_W<30)
				{
					LED_RED1(10);
				}
				if (I_W>=30 && I_W<50)
				{
					LED_RED1(60);
				}
				if (I_W>=50 && I_W<70)
				{
					LED_RED1(120);
				}
				if (I_W>=70 && I_W<=80)
				{
					LED_RED1(160);
				}
				if (I_W>80 && I_W<90)
				{
				    LED_RED1(180);
				}
				if (I_W>=90 && I_W<=101)
				{
					LED_RED1(225);
				}
				itoa(I_W,data1,10);
				itoa(D_W,data2,10);
				lcd_gotoxy(0,0);
				lcd_printf("W = ");
				lcd_gotoxy(4,0);
				lcd_printf(data1);
				lcd_printf(".");
				lcd_printf(data2);
				lcd_printf("[Proc]");

				
				Transmit_string(" HUDIMITY in % ");
				USART_SendLine();
				Transmit_string(data1);
				Transmit_string(".");
				Transmit_string(data2);
				Transmit_string("%");
				USART_SendLine();
				
			
				itoa(I_Temp,data1,10);
				itoa(D_Temp,data2,10);
				lcd_gotoxy(0,1);
				lcd_printf("T = ");
				lcd_gotoxy(4,1);
				lcd_printf(data1); 
				lcd_printf(".");
				lcd_printf(data2);
				lcd_printf("[deg.C]");
				
				
				Transmit_string(" TEMPERATURE ");
				USART_SendLine();
				Transmit_string(data1);
				Transmit_string(".");
				Transmit_string(data2);
				Transmit_string(" [deg. C]");
				USART_SendLine();
			}
			LED_White();
		}
		_delay_ms(500);		
	}
}
```
