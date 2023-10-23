#include "USART.h"

void USART_init(void)
{
	/*Ustaw szybkoœæ transmisji */
	UBRR0H = (BAUDRATE>>8);	// przesuñ rejestr w prawo o 8 bitów, aby uzyskaæ górne 8 bitów
	UBRR0L = BAUDRATE;		// ustaw szybkoœæ transmisji
	/*Podwójna prêdkoœæ transmisji */
	UCSR0A |= (1 << U2X0);
	/*W³¹cz nadajnik */
	UCSR0B = (1<<TXEN0);
	/* Ustaw format ramki: 8 bitów danych */
	UCSR0C = (3<<UCSZ00);
}

void USART_SendByte(unsigned char x)	//Wysy³anie bitu do UART
{
	while(!((UCSR0A)&(1<<UDRE0)));	//Sprawdzanie czy rejestr jest odpowiedzialny za odbiór danych (UDR0) jest gotowy do wys³ania danych (sprawdzaj¹c bit UDRE0 w rejestrze UCSR0A)
	UDR0=x;	//Jak tak to wysy³a przez UDR0
}

/*Funkcja USART_SendByte(unsigned char x) s³u¿y do wysy³ania jednego bajtu danych przez port szeregowy. 
Funkcja sprawdza czy bufor wyjœciowy UDR0 jest gotowy do wys³ania danych
 i nastêpnie wysy³a dane za pomoc¹ UDR0. */



void Transmit_string(const char *s)	// Funkcja ta wysy³a ci¹g znaków za poœrednictwem USART. Przyjmuje ona jako argument wskaŸnik na ci¹g znaków, a nastêpnie iteruje po nim, wysy³aj¹c pojedynczy znak na wyjœcie USART. Przed wys³aniem ka¿dego znaku opóŸnienie trwa 2 milisekundy.
{
	char c;
	while ( (c = *s++) )	//Operator *s++ w jêzyku C zwraca zawartoœæ wskaŸnika s (obecny element tablicy) i przesuwa wskaŸnik do kolejnego elementu.
	{
		USART_SendByte(c);
		_delay_ms(2);
	}
}
/*
Funkcja Transmit_string() w AVR C s³u¿y do wysy³ania ci¹gu znaków przy u¿yciu interfejsu USART.
 Ci¹g znaków jest przekazywany do funkcji jako wskaŸnik const char *s. 
 Wewn¹trz funkcji kolejne znaki s¹ pobierane za pomoc¹ operatora *s++ i wysy³ane do interfejsu USART za pomoc¹ funkcji USART_SendByte().
 Po ka¿dym wys³anym znaku nastêpuje opóŸnienie w milisekundach przy u¿yciu funkcji _delay_ms().
 */

void USART_SendLine(void)
{
	USART_SendByte(0X0D);	//Znak powrotu
	USART_SendByte(0X0A);	//Znak nowej lini w ASCII
}
/*
USART_SendLine() funkcja w AVR C s³u¿y do wysy³ania znaku nowej linii (CR/LF) do po³¹czonego urz¹dzenia. 
Funkcja ta wysy³a dwa bajty: 0X0D (znak powrotu) i 0X0A (znak nowej linii).
 Jest to u¿yteczne, gdy chcemy wyœwietliæ now¹ liniê na ekranie po³¹czonego urz¹dzenia.*/