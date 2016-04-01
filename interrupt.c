/*
 * interrupt.c
 *
 * Created: 4/19/2011 2:15:28 PM
 *  Author: robert kovacs
 */ 
#include <avr/io.h>
#include "Headers/avr_compiler.h"
#include "Headers/adc_driver.h"
#include "Headers/usart_driver.h"
#include "Headers/TC_driver.h"
#include "Headers/globals.h"
#include "Headers/port_driver.h"
#include "math.h"

//interrupt tajmera
ISR(TCE1_OVF_vect)	//1.5ms 
{	

}

ISR(TCF0_CCA_vect)
{
}

ISR(TCF0_OVF_vect)
{
	
}