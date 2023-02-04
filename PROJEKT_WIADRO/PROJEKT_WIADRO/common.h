/*
 * common.h
 *
 * Created: 21.01.2023 18:42:35
 *  Author: jnyki
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>	//bool

/* deklaracje dla bluetooth (jak w dokumentacji)*/
#define FOSC 1843200 // prêdkoœæ zegara
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*8UL)-1) // Ustawienie wartoœæi Baud Rate dla UBRR 
#define MYUBRR FOSC/16/BAUD-1



#endif /* COMMON_H_ */