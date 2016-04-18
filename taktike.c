/*
 * taktike.c
 *
 * Created: 08-Apr-16 18:24:23
 *  Author: Sirius-PC
 */ 

#include "Headers/avr_compiler.h"
#include "Headers/port_driver.h"
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/taktike.h"
#include "Headers/globals.h"
#include "Headers/usart_driver.h"


void idi_pravo(unsigned int x, unsigned int y, unsigned int ugao)
{
	switch(korak2){
		case 0:
			SendChar('A',&USART_XDRIVE);
			SendChar((x>>8),&USART_XDRIVE);
			SendChar(x,&USART_XDRIVE);
			SendChar((y>>8),&USART_XDRIVE);
			SendChar(y,&USART_XDRIVE);
			SendChar((ugao>>8),&USART_XDRIVE);
			SendChar(ugao,&USART_XDRIVE);
			SendChar('X',&USART_XDRIVE);
			overflow_primanje = 0;
			korak2++;
		break;
		
		case 1:
			if(okay_flag){
				stigao_flag2 = 1;
				sendMsg("okey",&USART_XM);
				korak2 = 2;
				okay_flag = 0;
				overflow_primanje = 0;
			}
			else if(overflow_primanje > 200)
			{
				overflow_primanje = 0;
				korak2 = 0;
				sendMsg("OVERFLOW",&USART_XM);
				
			}
		
		break;
		
		case 2:
			if(stigao_flag){
				stigao_flag2 = 0;
				sendMsg("stigao",&USART_XM);
				stigao_flag = 0;
				korak2++;
			}
			else if(overflow_primanje > 1600)
			{
				overflow_primanje = 0;
				korak2 = 0;
				sendMsg("OVERFLOW",&USART_XM);
				
			}
				
		break;
		
		
		
		default:
			break;
	}
}

void idi_nazad(unsigned int x, unsigned int y, unsigned int ugao)
{	
	switch(korak2){
		case 0:
			SendChar('B',&USART_XDRIVE);
			SendChar((x>>8),&USART_XDRIVE);
			SendChar(x,&USART_XDRIVE);
			SendChar((y>>8),&USART_XDRIVE);
			SendChar(y,&USART_XDRIVE);
			SendChar((ugao>>8),&USART_XDRIVE);
			SendChar(ugao,&USART_XDRIVE);
			SendChar('X',&USART_XDRIVE);
			overflow_primanje = 0;
			korak2++;
		break;
		
		case 1:
			if(okay_flag){
				stigao_flag2 = 1;
				sendMsg("okey",&USART_XM);
				korak2 = 2;
				okay_flag = 0;
				overflow_primanje = 0;		
			}
			else if(overflow_primanje > 200)
			{
				overflow_primanje = 0;
				korak = 0;
				sendMsg("OVERFLOW",&USART_XM);
				
			}
		break;
			
		case 2:
			if(stigao_flag){
				stigao_flag = 0;
				stigao_flag2=0;
				korak2++;
			}
			else if(overflow_primanje > 1600)
			{
				overflow_primanje = 0;
				korak2 = 0;
				sendMsg("OVERFLOW",&USART_XM);
				
			}
		break;
		
		default:
		break;
	}
}



void senzor_stop (void)
{
	if(PORTJ.IN & 0b00000001)
		{
			SendChar('S',&USART_XDRIVE);
			SendChar('A',&USART_XDRIVE);
			SendChar('B',&USART_XDRIVE);
			SendChar('C',&USART_XDRIVE);
			SendChar('D',&USART_XDRIVE);
			SendChar('E',&USART_XDRIVE);
			SendChar('F',&USART_XDRIVE);
			SendChar('P',&USART_XDRIVE);
			stigao_flag_pomocni=1;
			//stigao_flag=0;
			//korak2=0;
		}
}



void taktika_kocka(void){
		switch (korak)
		{
			case 0:
				idi_pravo(400,0,0);
				if (korak2 == 3)
				{
					sendMsg('korak++', &USART_XM);
					korak++;
					korak2 = 0;
				}
			break;
			
			case 1:
				
				idi_pravo(400,400,0);
				if (korak2 == 3)
				{
					sendMsg('korak++', &USART_XM);
					korak=2;
					korak2 = 0;
					
				}
			break;
			
			case 2:
				
				idi_nazad(0,400,0);
				if (korak2 == 3)
				{
					sendMsg('korak++', &USART_XM);
					korak=3;
					korak2 = 0;
				}
			
			break;
			
			case 3:
				idi_pravo(0,0,0);
				if (korak2 == 3)
				{
					sendMsg('korak++', &USART_XM);
					korak=0;
					korak2 = 0;
				}
			
			break;
			
			default:
			break;
		}
}

void taktika_1(void)
{
	switch (korak)
	{
		case 0:
			idi_pravo(500,0,0);			
			if (korak2 == 3)
			{
				sendMsg("Tacka 2", &USART_XM);
				korak ++;
				korak2 = 0;
			}
			break;
		
			case 1:
			idi_pravo(500,500,0);
			if(korak2==3)
			{
				sendMsg("Tacka 3", &USART_XM);
				korak++;
				korak2 = 0;
			}
			break;
			
			case 2:
			idi_pravo(0,500,0);
			if(korak2==3)
			{
				sendMsg("Tacka 4", &USART_XM);
				korak++;
				korak2 = 0;
			}
			break;
			
			case 3:
			idi_pravo(0,0,0);
			if(korak2==3)
			{
				sendMsg("Tacka 1 ", &USART_XM);
				korak++;
				korak2 = 0;
			}
			break;
		default:
		break;
	}
}