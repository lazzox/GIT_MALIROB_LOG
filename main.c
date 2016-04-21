/*
 * main.c
 *
 * Poslednje_izmene: 30/03/2016 16:07:00
 * Autor: AXIS team 
 
 Izmene:

 
 
 Potrebne izmene:
Sve
 */ 

#include <avr/io.h>
#include "Headers/avr_compiler.h"
#include "Headers/usart_driver.h"
#include "Headers/port_driver.h"
#include "Headers/globals.h"
#include "Headers/hardware.h"
#include "Headers/taktike.h"
 
 #define CINC_AKTIVAN (PORTJ_IN & 0b00000001)
 #define taster_gore (PORTJ_IN & 0b00000001)
 #define taster_dole (PORTJ_IN & 0b00000001)
 #define taster_ok (PORTJ_IN & 0b00000001)
 #define taster_cancel (PORTJ_IN & 0b00000001)
 //#define unsigned char boja '-'
 


int main(void)
{
	 volatile unsigned char
	 taster_gore_f = 0,
	 taster_dole_f =0,
	 taster_ok_f = 0,
	 taster_cancel_f = 0,
	  taster_gore_fp = 0,
	  taster_dole_fp =0,
	  taster_ok_fp = 0,
	  taster_cancel_fp = 0,
	 meni_glavni = 0,
	 boja = '-',
	 taktika = '-',
	 taster_pritisnut = 0,
	 meni_taktika = 0;
	

	 volatile unsigned int 
	 i = 0,
	 k = 20;
	 
	tajmer=0;
	senzor_tajmer=0;
	vreme_primanja = 0;
	stigao_flag_pomocni=0;
	sys_time=0;	
	RX_i_E1 = 0;	
	TIMED_OUT_VREME = 200;
	korak = 0;
	korak2 = 0;
	okay_flag=0;
	stigao_flag=0;
	stigao_flag2 =0;
	sys_time=0;
	
	Podesi_Oscilator();					//podesavanje oscilatora
	Podesi_Pinove();					//podesavanje I/O pinova
	Podesi_Tajmere();					//podesavanje tajmera
	Podesi_Interapt();					//podesavanje interapt prioriteta
	Podesi_USART_Komunikaciju();		//podesavanje komunikacije
	
	_delay_ms(1500);
	sendMsg("LOGIKA", &USARTD1);

	while(1){
		if (vreme_primanja > 100){
			vreme_primanja = 0;
			RX_i_E1 = 0;
		}

 		if (senzor_tajmer>100)
 		{
	 		senzor_stop();
	 		senzor_tajmer=0;
 		}
 		
		//taktika_kocka();			//za idipravo2
		//taktika_1();				//brzina
		ljubicasta_1();
		
		
	
	} //while
	

	
	
	while(CINC_AKTIVAN){
		//PROVERI STANJE TASTERA
		if(taster_ok){ //debouncing?
			for(i=0;i<20;i++){
				if(taster_ok) k--;
			}
			if(!k) taster_ok_f = 1;
		}
		else{
			for(i=0;i<20;i++){
				if(!taster_ok) k--;
			}
			if(!k) taster_ok_f = 0;
		}
		
		if(taster_cancel){ //debouncing?
			for(i=0;i<20;i++){
				if(taster_cancel) k--;
			}
			if(!k) taster_cancel_f = 1;
		}
		else{
			for(i=0;i<20;i++){
				if(!taster_cancel) k--;
			}
			if(!k) taster_cancel_f = 0;
		}
		
		if(taster_gore){ //debouncing?
			for(i=0;i<20;i++){
				if(taster_gore) k--;
			}
			if(!k) taster_gore_f = 1;
		}
		else{
			for(i=0;i<20;i++){
				if(!taster_gore) k--;
			}
			if(!k) taster_gore_f = 0;
		}
		
		if(taster_dole){ //debouncing?
			for(i=0;i<20;i++){
				if(taster_dole) k--;
			}
			if(!k) taster_dole_f = 1;
		}
		else{
			for(i=0;i<20;i++){
				if(!taster_dole) k--;
			}
			if(!k) taster_dole_f = 0;
		}
		
		switch(meni_glavni){
			
			case 0: //GLAVNI MENI
				sendMsg("B:",&USART_LCD);
				SendChar(boja,&USART_LCD);
				sendMsg("T:",&USART_LCD);
				SendChar(taktika,&USART_LCD);
				if(taster_ok_f && !taster_ok_fp){ //taster OK u GLAVNOM MENIJU
					meni_glavni = 1;	
				}
			break;	
			
			case 1:
				sendMsg("TAKTIKE:",&USART_LCD);
				if(taster_ok_f && !taster_ok_fp){
					meni_glavni = 10;
					//idi u taktike
				}
				else if(taster_cancel_f && !taster_cancel_fp){
					//izadji
					meni_glavni = 0;
				}
				else if(taster_dole_f && !taster_dole_fp){
					meni_glavni = 2;
				}
				
			break;
			case 2:
				sendMsg("BOJA STOLA:",&USART_LCD);
				if(taster_ok_f && !taster_ok_fp){
					meni_glavni = 10;
					//idi u odabir boje stola
				}
				else if(taster_cancel_f && !taster_cancel_fp){
					//izadji u glavni meni
					meni_glavni = 0;
				}
			break;
			case 10:
				sendMsg("Taktika 1", &USART_LCD);
				if(taster_ok_f && !taster_ok_fp){ //taster OK 
					meni_glavni = 0;
					taktika = '1';
				}
			
			default:
			break;
		}
		
		taster_ok_fp = taster_ok_f;
		taster_cancel_fp = taster_cancel_f;
		taster_gore_fp = taster_gore_f;
		taster_dole_fp = taster_dole_f;
		
		
		//BEZ DOBOUNCING-a
		//if(taster_cancel){
			//taster_cancel_f = 1;
		//}else{
			//taster_cancel_f = 0;
		//}
		//if(taster_gore){
			//taster_gore_f = 1;
		//}else{
			//taster_gore_f = 0;
		//}
		//if(taster_dole){
			//taster_dole_f = 1;
		//}else{
			//taster_dole_f = 0;
		//}
		
		
	}
	
}//main



