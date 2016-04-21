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

#define senzor_maska_prednji 0b00000101
#define senzor_maska_zadnji  0b00010000
#define sensor_det (PORTJ.IN & sensor_dir)
//#define senzor_prednji (PORTJ.IN & senzor_maska_prednji)
//#define senzor_zadnji (PORTJ.IN &  senzor_maska_zadnji)

//#define s1s (sys_time>666)
//#define s2s (sys_time>1332)
//#define s3s (sys_time>1998)
//#define s4s (sys_time>2664)
//#define s5s (sys_time>3330)
//#define s10s (sys_time>6660)

void postavi_sistem(long x, long y, long ugao)
{
	switch(korak2){
		case 0:
			SendChar('G',&USART_XDRIVE);
			SendChar((x>>8),&USART_XDRIVE);
			SendChar(x,&USART_XDRIVE);
			SendChar((y>>8),&USART_XDRIVE);
			SendChar(y,&USART_XDRIVE);
			SendChar((ugao>>8),&USART_XDRIVE);
			SendChar(ugao,&USART_XDRIVE);
			SendChar('S',&USART_XDRIVE);
			korak2++;
			overflow_primanje = 0;
		break;
		case 1:
		if(okay_flag){
			
			//sendMsg("okey",&USART_XM);
			korak2 = 3;
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
		
		default:
		break;
	}
}

void idi_pravo(signed int x, signed int y, signed int ugao)
{
	switch(korak2){
		case 0:
		if (!stigao_flag_pomocni)
		{
			sensor_dir = senzor_maska_prednji;
			//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
			//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
			//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
			// i ako su negativni)
			x += 4000;
			y += 4000;
			ugao += 720;
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
		}
		else if (stigao_flag_pomocni)
		{
			stigao_flag_pomocni=0;
		}
		break;
		
		case 1:
			if(okay_flag){
				stigao_flag2 = 1;
				korak2 = 2;
				okay_flag = 0;
				overflow_primanje = 0;
			}
			else if(overflow_primanje > 200)
			{
				overflow_primanje = 0;
				korak2 = 0;
			}  
		
		break;
		
		case 2:
		if (stigao_flag_pomocni)
		{
			overflow_primanje=0;
			korak2=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
			
		}
		else if(stigao_flag){
				stigao_flag2 = 0;
				stigao_flag = 0;
				korak2=4;
			}
				
		break;
		
		case 4:
		
		if(stigao_flag_pomocni)
		{
			korak2=0;
			overflow_primanje=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
		}
		else if (!stigao_flag_pomocni)
		{
			korak2=3;
			stigao_flag2=0;
			stigao_flag-0;
		}
		break;
		
		default:
			break;
	}
	
}


void idi_pravo2(signed int x, signed int y)
{

	switch(korak2){
		case 0:
		if (!stigao_flag_pomocni)
		{
			sensor_dir = senzor_maska_prednji;
			//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
			//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
			//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
			// i ako su negativni)
			x += 4000;
			y += 4000;
			SendChar('X',&USART_XDRIVE);
			SendChar((x>>8),&USART_XDRIVE);
			SendChar(x,&USART_XDRIVE);
			SendChar((y>>8),&USART_XDRIVE);
			SendChar(y,&USART_XDRIVE);
			SendChar('y',&USART_XDRIVE);
			SendChar('y',&USART_XDRIVE);
			SendChar('X',&USART_XDRIVE);
			overflow_primanje = 0;
			korak2++;
		}
		else if (stigao_flag_pomocni)
		{
			stigao_flag_pomocni=0;
		}
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
		if (stigao_flag_pomocni)
		{
			overflow_primanje=0;
			korak2=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
			
		}
		else if(stigao_flag){
			stigao_flag2 = 0;
			sendMsg("stigao",&USART_XM);
			stigao_flag = 0;
			korak2=4;
		}
		
		break;
		
		
		case 4:
		if(stigao_flag_pomocni)
		{
			korak2=0;
			overflow_primanje=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
		}
		else if (!stigao_flag_pomocni)
		{
			korak2=3;
			stigao_flag2=0;
			stigao_flag-0;
		}
		
		
		break;
		
		
		
		default:
		break;
	}
	
}

void idi_nazad(signed int x, signed int y, signed int ugao)
{
	switch(korak2){
		case 0:
		if (!stigao_flag_pomocni)
		{
			sensor_dir = senzor_maska_prednji;
			//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
			//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
			//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
			// i ako su negativni)
			x += 4000;
			y += 4000;
			ugao += 720;
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
		}
		else if (stigao_flag_pomocni)
		{
			stigao_flag_pomocni=0;
		}
		break;
		
		case 1:
		if(okay_flag){
			stigao_flag2 = 1;
			korak2 = 2;
			okay_flag = 0;
			overflow_primanje = 0;
		}
		else if(overflow_primanje > 200)
		{
			overflow_primanje = 0;
			korak2 = 0;
		}
		break;
		
		case 2:
		if (stigao_flag_pomocni)
		{
			overflow_primanje=0;
			korak2=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
			
		}
		else if(stigao_flag){
			stigao_flag2 = 0;
			stigao_flag = 0;
			korak2=4;
		}
		break;
		
		case 4:
		if(stigao_flag_pomocni)
		{
			korak2=0;
			overflow_primanje=0;
			stigao_flag_pomocni=0;
			stigao_flag=0;
			stigao_flag2=0;
		}
		else if (!stigao_flag_pomocni)
		{
			korak2=3;
			stigao_flag2=0;
			stigao_flag-0;
		}
		break;
		
		default:
		break;
	}
	
}


void rotiraj (int ugao_rotacije)
{
	switch(korak2){
		case 0:
		//LOGIKA DODAJE +4000 za X, Y i +720 za ugao
		//TO JE DA BI SE SLALI POZITIVNI BROJEVI PREKO USART-a
		//A KASNIJE SE ODUZME DA BI SE DOBILI PRAVI BROJEVI (cak
		// i ako su negativni)
		ugao_rotacije += 720;
		SendChar('U',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar('v',&USART_XDRIVE);
		SendChar((ugao_rotacije>>8),&USART_XDRIVE);
		SendChar(ugao_rotacije,&USART_XDRIVE);
		SendChar('X',&USART_XDRIVE);
		overflow_primanje = 0;
		korak2++;
		break;
		
		case 1:
		if(okay_flag){
			stigao_flag2 = 1;
			sendMsg("okey",&USART_XM);
			korak2 = 3;
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
		
		default:
		break;
	}
		
	
}

void ukljuci_senzore(){
	sensor_enable = 0xFF;
}

void iskljuci_senzore(){
	sensor_enable = 0x00;
}

void senzor_stop(void)
{
	
	if(sensor_det & sensor_enable)
	{//Ako udje neki od senzora je detektovao
		sendMsg("DETEKTOVAO SAM",&USART_LCD);
		for (int i=0;i<11;i++)
		{
			if(sensor_det)
			flag_senzor++;
		}
		
	}
	else{
		sendMsg("CLEAN SAM",&USART_LCD);
		tajmer=0;
	}
	if(flag_senzor>9)
	{
		//stop
		
		stigao_flag_pomocni=1;
		SendChar('S',&USART_XDRIVE);
		SendChar('A',&USART_XDRIVE);
		SendChar('B',&USART_XDRIVE);
		SendChar('C',&USART_XDRIVE);
		SendChar('D',&USART_XDRIVE);
		SendChar('E',&USART_XDRIVE);
		SendChar('F',&USART_XDRIVE);
		SendChar('P',&USART_XDRIVE);
		flag_senzor=0;
		
		//korak
		
		
		//zabrana slanja
		
		
		//cekaj
		if (tajmer>2000)//  && korak_pom != 0)
		{
			korak2=0;
			sys_time=0;
			tajmer=0;
			korak=99;
			iskljuci_senzore();
		}
		
		//cekaj rutina
		
		
		//dozvola slanja
		
		
		//nastavi
		
	}
}


void brzina (unsigned int brzinaa)
{
	switch(korak2){
		case 0:
		SendChar('D',&USART_XDRIVE);
		SendChar((brzinaa>>8),&USART_XDRIVE);
		SendChar(brzinaa,&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('y',&USART_XDRIVE);
		SendChar('X',&USART_XDRIVE);
		overflow_primanje = 0;
		korak2++;
		break;
		
		case 1:
		if(okay_flag){
			korak2 = 3;
			okay_flag = 0;
			overflow_primanje = 0;
		}
		else if(overflow_primanje > 200)
		{
			overflow_primanje = 0;
			korak2 = 0;
		}
		break;
		
		default:
		break;
	}
	
}



void taktika_kocka(void){
		
		
		
		switch(korak)
		{
		
			
			case 0:
			//if (sys_time>1500)
			//{
				//sendMsg("case 0", &USART_XM);
				//brzina(500);
				idi_pravo2(500,500);
				if (korak2 == 3)
				{
					sendMsg("Tacka 0234324", &USARTD1);
					korak++;
					korak2 = 0;
					sys_time=0;
				}
			//}
			break;
			
			case 1:
			if (sys_time>2000)
			{
				idi_nazad(0,0,0);
				if (korak2 == 3)
				{
					sendMsg("Tacka 1", &USARTD1);
					korak++;
					korak2 = 0;
					sys_time=0;
				}
			}
			break;
			
// 			case 2:
// 			sendMsg("case 1", &USART_XM);
// 			//if (sys_time>3500)
// 			//{
// 				idi_pravo(500,0,0);
// 				if (korak2 == 3)
// 				{
// 					sendMsg("Tacka 2", &USARTD1);
// 					korak++;
// 					korak2 = 0;
// 					sys_time=0;
// 				}
// 			//}
// 			break;
// 			
// 			case 3:
// 			//sendMsg("case 2", &USART_XM);
// 				idi_pravo(800,0,0);
// 				if (korak2 == 3)
// 				{
// 					sendMsg("Tacka 3", &USARTD1);
// 					//sendMsg('korak++', &USART_XM);
// 					//if (!stigao_flag_pomocni)
// 					korak++;
// 					korak2 = 0;
// 					sys_time=0;
// 				}
// 			break;
// 			
// 			case 4:
// 			//sendMsg("case 3", &USART_XM);
// 			//if (sys_time>3500)
// 			//{
// 				idi_pravo(0,0,0);
// 				if (korak2 == 3)
// 				{
// 					sendMsg("Tacka 4", &USARTD1);
// 					//if (!stigao_flag_pomocni)
// 					korak=0;
// 					korak2 = 0;
// 					sys_time=0;
// 				}
// 			//}
// 			break;
			
			default:
			break;
		}
}

void taktika_1(void)
{
	
	switch (korak)
	{
		case 0:
			//brzina(250);
			postavi_sistem(0,0,0);			
			if (korak2 == 3)
			{
				korak ++;
				korak2 = 0;
				sys_time=0;
			}
		break;
		
		case 1:
		if (sys_time>700)
		{
		//rotiraj(180);
		iskljuci_senzore();
		idi_pravo(700,0,0);
		if (korak2==3)
			{
				korak++;
				korak2=0;
				sys_time=0;
				tajmer=0;
			}
		}
		break;
		
		case 2:
		ukljuci_senzore();
		idi_nazad(0,0,0);
		if (korak2==3)
		{
			korak++;
			korak2=0;
			sys_time=0;
			tajmer=0;
		}
		break;
		
		case 3:
		if (sys_time>700)
		{
			//rotiraj(180);
			idi_nazad(500,0,0);
			if (korak2==3)
			{
				korak++;
				korak2=0;
				sys_time=0;
				tajmer=0;
			}
		}
		break;
		
		//case 2:
		//if (sys_time>1000)
		//{
		//rotiraj(90);
		////idi_nazad(6000,400,180);
		//if (korak2==3)
		//{
			//korak++;
			//korak2=0;
			//sys_time=0;
		//}
		//}
		//break;
			//
		//case 3:
		//if (sys_time>1000)
		//{
			//rotiraj(180);
		////idi_pravo(0,400,270);
		////idi_pravo(600,0,0);
		//if (korak2==3)
		//{
			//korak++;
			//korak2=0;
			//sys_time=0;
		//}
		//}
		//break;
		//
		//case 4:
		//if (sys_time>1000)
		//{
			//rotiraj(270);
			////idi_pravo(0,0,0);
			//if (korak2==3)
			//{
				//korak++;
				//korak2=0;
				//sys_time=0;
			//}
		//}
		//break;
		//
		//case 5:
		//if (sys_time>1000)
		//{
			//rotiraj(0);
			////idi_pravo(0,0,0);
			//if (korak2==3)
			//{
				//korak=2;
				//korak2=0;
				//sys_time=0;
			//}
		//}
		//break;
			
		
		default:
		break;
	}
	
}


void ljubicasta(void)
{
	
	switch (korak)
	{
		case 99:
		idi_nazad(0,0,0);
		if (korak2==3)
		{
			korak++;
			korak2=0;
			sys_time=0;
		}
		break;
		
		//case 100:
		//if (sys_time>2000)
		//{
			//ukljuci_senzore();
			//
		//}		
		//break;
		
		case 0:
		//brzina(250);
		//ukljuci_senzore();
		idi_pravo(600,0,0);				//guranje kockica do pozicije gde ih ostavlja
		sendMsg("C1",&USARTD1);
		if (korak2 == 3)
		{
			korak ++;
			korak2 = 0;
			sys_time=0;
		}
		break;
		
  		case 1:
  			//brzina(250);
  			idi_nazad(0,0,0);				//izvlacenje roboba da ide ka vratima
  			sendMsg("C2",&USARTD1);
  			if (korak2 == 3)
  			{
  				korak ++;
  				korak2 = 0;
  				sys_time=0;
  			}
  		break;
		  
		  case 2:
		  //brzina(250);
		  idi_pravo(400,0,0);				//guranje kockica do pozicije gde ih ostavlja
		  sendMsg("C3",&USARTD1);
		  if (korak2 == 3)
		  {
			  korak =88;
			  korak2 = 0;
			  sys_time=0;
		  }
		  break;
		  
		  case 3:
		  //brzina(250);
		  idi_nazad(0,0,0);				//izvlacenje roboba da ide ka vratima
		  sendMsg("C4",&USARTD1);
		  if (korak2 == 3)
		  {
			  korak =1;
			  korak2 = 0;
			  sys_time=0;
		  }
		  break;
		  
		  case 4:
			  idi_nazad(550,200,90);			//prilazak vratima
			  if (korak2 == 3)
			  {
				  korak ++;
				  korak2 = 0;
				  sys_time=0;
			  }
		  break;
		  
		  case 5:
		  //brzina(150);
		  idi_nazad(550,0,90);					//udaranje vrata
			  if (korak2 == 3)
			  {
				  korak ++;
				  korak2 = 0;
				  sys_time=0;
			  }
		  break;
		  
		  case 6:
		  //brzina(250);
		  idi_pravo(400,150,90);				//izvlacenje ispred vrata
		  if (korak2 == 3)
		  {
			  korak ++;
			  korak2 = 0;
			  sys_time=0;
		  }
		  break;
		  case 7:
		 // brzina(150);
		  idi_pravo(300,0,90);				//udaranje drugih vrata ako moze pod tim uglom
		  if (korak2 == 3)
		  {
			  korak ++;
			  korak2 = 0;
			  sys_time=0;
		  }
		  break;
		   case 8:
		   // brzina(150);
		   idi_pravo(200,1250,90);				//skoljka prva treba da se smakne malo da upadne u rupu, kada se testira
		   if (korak2 == 3)
		   {
			   korak ++;
			   korak2 = 0;
			   sys_time=0;
		   }
		   break;
		   
		   
		   //treba da se stavi servo da zatvori
		   
		    case 9:								//skoljka druga treba da se smakne malo da upadne u rupu, kada se testira
		    // brzina(150);
		    idi_nazad(300,1500,90);
		    if (korak2 == 3)
		    {
			    korak ++;
			    korak2 = 0;
			    sys_time=0;
		    }
		    break;
			
			  //treba da se stavi servo da zatvori
			
			 case 10:							
			 // brzina(150);
			 idi_nazad(300,0,90);
			 if (korak2 == 3)
			 {
				 korak ++;
				 korak2 = 0;
				 sys_time=0;
			 }
			 break;
		
		default:
		break;
	}
	
}

void ljubicasta_1(void)
{
	
	switch (korak)
	{
		case 0:
		//brzina(250);
		korak_pom=0;
		iskljuci_senzore();
		postavi_sistem(210,1020,90);
		if (korak2 == 3)
		{
			korak++;
			korak2 = 0;
			sys_time=0;
		}
		break;
		
		case 1:
		if (sys_time>300)
		{
			//brzina(350);
			korak_pom=0;
			idi_nazad(210,900,90);
			if (korak2==3)
			{
				korak++;
				korak2=0;
				sys_time=0;
			}
		}
		break;
		
		case 2:
		
			//brzina(350);
			ukljuci_senzore();
			korak_pom=0;
			idi_nazad(1000,900,180);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
			break;
		
		case 3:
		if (sys_time>300)
		{
			//brzina(350);
			//korak_pom=101;
			korak_pom=101;
			idi_pravo(500,900,90);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 4:
		if (sys_time>300)
		{
			//brzina(350);
			//korak_pom=101;
			korak_pom=101;
			idi_nazad(550,250,90);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 5:
		if (sys_time>300)
		{
			//brzina(350);
			idi_nazad(550,100,90);	
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 6:
		if (sys_time>300)
		{
			//brzina(350);
			idi_pravo(300,350,90);	
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 7:
		if (sys_time>300)
		{
			//brzina(350);
			idi_nazad(300,100,90);	
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 8:
		if (sys_time>300)
		{
			//brzina(350);
			idi_pravo(700,1700,90);	
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 9:
		if (sys_time>300)
		{
			//brzina(350);
			idi_pravo(300,1650,180);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 10:
		if (sys_time>300)
		{
			//brzina(350);
			idi_pravo(180,1550,270);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 11:
		if (sys_time>300)
		{
			//brzina(350);
			idi_pravo(200,900,270);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		case 12:
		if (sys_time>300)
		{
			//brzina(350);
			idi_nazad(200,1500,90);
			if (korak2==3)
			{
				korak2=0;
				korak++;
				sys_time=0;
			}
		}
		break;
		
		
		
		
		
		
		
		case 101:
		//brzina(250);
		//idi_pravo(500,1000,90);
		iskljuci_senzore();
		rotiraj(180);
		if (korak2 == 3)
		{
			korak++;
			korak2 = 0;
			sys_time=0;
		}
		break;
		
		case 102:
		//brzina(250);
		ukljuci_senzore();
		if (sys_time>3330)
		{
			korak2=0;
			sys_time=0;
			korak = korak_pom;
		}
		break;
	
	default:
	break;
	}
	
}