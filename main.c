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
 
 #define CINC_AKTIVAN  (PORTH_IN & 0b00100000)	//H5
 #define taster_gore   (PORTH_IN & 0b00001000)	//H3
 #define taster_dole   (PORTH_IN & 0b00000100)	//H2
 #define taster_ok     (PORTH_IN & 0b00000001)	//H0
 #define taster_cancel (PORTH_IN & 0b00000010)	//H1



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
	boja = '-',
	taktika = 1,
	taster_pritisnut = 0,
	meni_taktika = 0;
	

	volatile unsigned int
	i = 0,
	meni_glavni = 0,
	while_flag = 1,
	k = 10;
	 
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
	
	cli(); //STOPIRAJ INTERRUPT
	
	
	
	
	while(!CINC_AKTIVAN){
		if(!taster_ok){ //debouncing?
			for(i=0;i<10;i++){
				if(!taster_ok) k--;
			}
			if(!k) taster_ok_f = 1;
		}
		else{
			taster_ok_f = 0;
		}
			
		if(!taster_cancel){ //debouncing?
			for(i=0;i<10;i++){
				if(!taster_cancel) k--;
			}
			if(!k) taster_cancel_f = 1;
		}
		else{
			taster_cancel_f = 0;
		}
			
		if(!taster_gore){ //debouncing?
			for(i=0;i<10;i++){
				if(!taster_gore) k--;
			}
			if(!k) taster_gore_f = 1;
		}
		else{
			taster_gore_f = 0;
		}
			
		if(!taster_dole){ //debouncing?
			for(i=0;i<10;i++){
				if(!taster_dole) k--;
			}
			if(!k) taster_dole_f = 1;
		}
		else{
			taster_dole_f = 0;
		}
		k = 10;
		switch(meni_glavni){
				
			case 0: //POSTAVKE MENI
				sendMsg("B:",&USART_LCD);
				SendChar(boja,&USART_LCD);
				sendMsg(" ",&USART_LCD);
				sendMsg("T:",&USART_LCD);
				SendChar(taktika + 0x30,&USART_LCD);
				if(taster_ok_f && !taster_ok_fp){ //taster OK u GLAVNOM MENIJU
					meni_glavni = 1;
				}
				else if(taster_cancel_f && !taster_cancel_fp){
					//izadji
					meni_glavni = 1;
				}
				else if(taster_dole_f && !taster_dole_fp){ //idi dole kroz meni
					meni_glavni = 1;
				}
				else if(taster_gore_f && !taster_gore_fp){ //idi gore kroz meni
					meni_glavni = 1;
				}
			break;
				
			//MENI SA OPCIJAMA:
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
				else if(taster_gore_f && !taster_gore_fp){ //idi gore kroz meni
					meni_glavni = 2;
				}
				
				break;
					
			case 2:
				sendMsg("BOJA STOLA:",&USART_LCD);
				if(taster_ok_f && !taster_ok_fp){
					meni_glavni = 20;
					
				}
				else if(taster_cancel_f && !taster_cancel_fp){ //izadji iz menija
					//izadji u glavni meni
					meni_glavni = 0;
				}
				else if(taster_dole_f && !taster_dole_fp){ //idi dole kroz meni
					meni_glavni = 1;
				}
				else if(taster_gore_f && !taster_gore_fp){ //idi gore kroz meni
					meni_glavni = 1;
				}
			break;
				
				
				
			case 10:
				sendMsg("Ljubicasta()", &USART_LCD);
				if(taster_ok_f && !taster_ok_fp){ //taster OK
					meni_glavni = 0;
					taktika = 1;
					}else if(taster_cancel_f && !taster_cancel_fp){ //izadji iz menija
					//izadji u glavni meni
					meni_glavni = 0;
				}
				break;
				//boja stola
			case 20:
				sendMsg("LJUBICASTA", &USART_LCD);
				if(taster_ok_f && !taster_ok_fp){ //taster OK
					meni_glavni = 0;
					boja = 'L';
					}else if(taster_cancel_f && !taster_cancel_fp){ //izadji iz menija
					//izadji u glavni meni
					meni_glavni = 0;
				}
				else if(taster_dole_f && !taster_dole_fp){ //idi dole kroz meni
					meni_glavni = 21;
				}
				else if(taster_gore_f && !taster_gore_fp){ //idi gore kroz meni
					meni_glavni = 21;
				}
				
			break;
				
			case 21:
				sendMsg("ZELENA", &USART_LCD);
				if(taster_ok_f && !taster_ok_fp){ //taster OK
					meni_glavni = 0;
					boja = 'Z';
					}else if(taster_cancel_f && !taster_cancel_fp){ //izadji iz menija
					//izadji u glavni meni
					meni_glavni = 0;
				}
				else if(taster_dole_f && !taster_dole_fp){ //idi dole kroz meni
					meni_glavni =20;
				}
				else if(taster_gore_f && !taster_gore_fp){ //idi gore kroz meni
					meni_glavni = 20;
				}
				
			break;
				
			default:
			break;
			}
			
			taster_ok_fp = taster_ok_f;
			taster_cancel_fp = taster_cancel_f;
			taster_gore_fp = taster_gore_f;
			taster_dole_fp = taster_dole_f;	
		
		_delay_ms(200);
		
}//while
	
	sei();
	//restartovati vazne promenljive tipa vremenske!!!
	sys_time = 0;
	//GLAVNI WHILE!!!
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
			switch(taktika){ //TAKTIKE
				case 1: //taktika_1 - default
					ljubicasta();
				break;
				
				default:
					ljubicasta();
				break;	
			
			}//switch
			ljubicasta();
	} //while
		

	
}//main



