/*
 * taktike.h
 *
 * Created: 08-Apr-16 18:25:12
 *  Author: Sirius-PC
 */ 

#ifndef TAKTIKE_H_
#define TAKTIKE_H_



void idi_pravo(signed int x, signed int y, signed int ugao);
void idi_nazad(signed int x, signed int y, signed int ugao);
void senzor_stop(void);
void rotiraj(int ugao_rotacije);


void brzina(unsigned int brzinaa);
void postavi_sistem(long x, long y, long ugao);

void ukljuci_senzore(void);
void iskljuci_senzore(void);

void taktika_kocka(void);
void taktika_1(void);
void ljubicasta(void);
void ljubicasta_1(void);

volatile unsigned char
flag_senzor,
senzor_enable_prednji,
sensor_dir,
sensor_enable,
senzor_enable_zadnji;

volatile unsigned int 
senzor_tajmer,
vreme_cekanja,
korak,
korak2,
korak_pom,
korak_reg;


#endif /* TAKTIKE_H_ */