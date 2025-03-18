/**
 * Auteur 
 * Maxime Champagne
 * 3 mars 2022
 * 
 * Modifié par
 * Sonia Michaud
 * 4 mars 2025-18 mars 2025
 *
 * SPI/main.c
 * 
*/
#include <stdio.h>
#include "mcc_generated_files/mcc.h"
#include "ecran.h"

#define MAX 60
uint8_t const sin[MAX] ={
              254,254,252,249,244,238,231,222,213,202,
              191,179,167,154,141,127,114,101,88,76,
              64,53,42,33,24,17,11,6,3,1,
              0,1,3,6,11,17,24,33,42,53,
              64,76,88,101,114,128,141,154,167,179,
              191,202,213,222,231,238,244,249,252,254};

uint8_t const car[MAX] ={
             0,0,0,0,0,0,0,0,0,0,
			  0,0,0,0,0,0,0,0,0,0,
			  0,0,0,0,0,0,0,0,0,0,
			  255,255,255,255,255,255,255,255,255,255,
			  255,255,255,255,255,255,255,255,255,255,
			  255,255,255,255,255,255,255,255,255,255};

uint8_t const tri[MAX] ={
            9,17,26,34,43,51,60,68,77,85,
			 94,102,111,119,128,136,145,153,162,170,
			 179,187,196,204,213,221,230,238,247,255,
			 247,238,230,221,213,204,196,187,179,170,
			 162,153,145,136,128,119,111,102,94,86,
			 77,68,60,51,43,34,26,17,9,0};

void out_dig(uint8_t x);
//void sinus_60(void);
//void carre_60(void);
//void triangulaire_60(void);
void myTimer1_ISR(void);

uint8_t i=0;
uint16_t reset=0xEFBA;
/*
                         Main application
 */
void main(void)
{
//    uint8_t valeur, lecture;
//    float tension;
    uint8_t lecture='M';
    uint8_t lettre='M';
    SYSTEM_Initialize();
    
    INTERRUPT_GlobalInterruptEnable();
    
    INTERRUPT_PeripheralInterruptEnable();
    
    TMR1_SetInterruptHandler(myTimer1_ISR);

    SSPCON1bits.SSPEN = 1;
    IO_RA5_SetHigh();
    IO_RB5_SetHigh();
    uint8_t ancieni=0;
    uint8_t vitesse=0;
    videEcran();
    curseurPosition(0x00);//pour affichage lcd
    printf("ENTRER S,C OU T");
    curseurPosition(0x40);//pour affichage lcd
    printf("Onde:");
    curseurPosition(0x14);//pour affichage lcd
    printf("Freq:");
    while (1)
    {
        
        
//        //Code de test pour valider le fonctionnement du potentiomètre
//        {
//            printf("\n\rEntrer une valeur entre 0 et 255, suivie de [Enter]");
//            valeur = 0;
//            do
//            {
//                do
//                {
//                    
//                        lecture = EUSART1_Read();
//                                       
//                }
//                while (((lecture < '0') || (lecture > '9')) && (lecture != 0x0d));
//                if ((lecture >= '0') && (lecture <= '9')) 
//                {
//                    valeur = 10 * valeur + lecture - '0';
//                    putchar(lecture);
//                }
//            }
//        
//            while ((lecture != 0x0d) && (valeur < 26)); 
//            tension = (float)5* valeur /256;
//            printf("\n\rValeur = %u tension = %3.2f ", valeur, tension);
//            out_dig(valeur);    // envoi sur potentiometre 
//        } 
        if (ancieni==i){
            
        }else{
            ancieni=i;
            if (EUSART1_is_rx_ready()){
                lecture = EUSART1_Read();
                if(lecture=='+'){
                    vitesse++;
                    if(vitesse>4){
                        vitesse=4;
                    }
                }else if (lecture=='-'){
                    vitesse--;
                    if(vitesse<0){
                        vitesse=0;
                    }
                }else{
                    lettre=lecture;
                }
                switch (vitesse){
                 case 0:
                    reset=0xEFBA;
                    curseurPosition(0x19);//pour affichage lcd
                    printf("20Hz ");
                    break;
                 case 1:
                    reset=0xF7DF;
                    curseurPosition(0x19);//pour affichage lcd
                    printf("40Hz ");
                    break;
                 case 2:
                    reset=0xFA91;
                    curseurPosition(0x19);//pour affichage lcd
                    printf("60Hz ");
                    break;
                case 3:
                    reset=0xFBEF;
                    curseurPosition(0x19);//pour affichage lcd
                    printf("80Hz ");
                    break;
                case 4:
                    reset=0xFCBC;
                    curseurPosition(0x19);//pour affichage lcd
                    printf("100Hz");
                    break;
                default:
                    
                    break;
                }
            }else{
                switch (lettre){
                 case 'S':
//                    sinus_60();
                    out_dig(sin[i]);
                    curseurPosition(0x45);//pour affichage lcd
                    printf("Sinus   ");
                    break;
                 case 'C':
//                   carre_60();
                    out_dig(car[i]);
                    curseurPosition(0x45);//pour affichage lcd
                    printf("Carre   ");
                    break;
                 case 'T':
//                   triangulaire_60();
                    out_dig(tri[i]);
                    curseurPosition(0x45);//pour affichage lcd
                    printf("Triangle");
                    break;
                 default:

                    break;
                } 
            }
        }
        
        
        
        
        //Code de test pour générer une onde sinusoidale
//        sinus_60();
        
    }

           
}

//---------------------------------------------------------------
// Routine d'interruption du Timer1
//---------------------------------------------------------------
void myTimer1_ISR(void){
//    static uint8_t i; 
    
    TMR1_WriteTimer(reset);
    
//    out_dig(sin[i]);
    IO_RB5_Toggle();//pour observer l'onde
    i++;
    if (i==MAX){
        i=0;
    }
}
    
//----------------------------------------------------------------
// Transmission au pot. d'une onde comprenant 60 points par cycle.
//----------------------------------------------------------------
//void sinus_60(void) {
//    uint8_t i;
//    //while(1) {
//        for (i=0;i<MAX;i++) {
//            out_dig(sin[i]);
//			__delay_ms(1);
//            }
//        //} 
//}
//void carre_60(void) {
//    uint8_t i;
//    //while(1) {
//        for (i=0;i<MAX;i++) {
//            out_dig(car[i]);
//			__delay_ms(1);
//            }
//        //} 
//}
//void triangulaire_60(void) {
//    uint8_t i;
//    //while(1) {
//        for (i=0;i<MAX;i++) {
//            out_dig(tri[i]);
//			__delay_ms(1);
//            }
//        //} 
//}

//----------------------------------------------------------------
//  Transmission d'une donnee a la sortie du pot. numerique
//----------------------------------------------------------------
void out_dig(uint8_t x)
{
	IO_RA5_SetLow();   // selection du potentiometre
	SPI_ExchangeByte(0x11);  // ecriture, pot. 0
	SPI_ExchangeByte(x);
	IO_RA5_SetHigh();
	//__delay_ms(1);
}
