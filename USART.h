
#ifndef _HAL_USART_H_ //jest to dyrektywa preprocesora, która s³u¿y do zapobiegania podwójnemu za³adowaniu tego samego pliku nag³ówkowego wiele razy. Jeœli plik nag³ówkowy zostanie za³adowany, to dyrektywa preprocesora sprawdza, czy zmienna _HAL_USART_H_ jest definiowana. Jeœli nie jest, oznacza to, ¿e plik nag³ówkowy nie zosta³ do tej pory za³adowany. W przeciwnym razie, jeœli zmienna jest ju¿ zdefiniowana, oznacza to, ¿e plik n
#define _HAL_USART_H_

#include <avr/io.h>
#include "common.h"

void USART_init(void);
void USART_SendByte(unsigned char x);	//USART_Transmit
void Transmit_string(const char *s);
void USART_SendLine(void);


#endif
