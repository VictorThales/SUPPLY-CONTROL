//Academicos da Eng da computação: Victor Thales P. de Limas / Gustavo Scheffer

#include "delay.h"
#include "adc.h"
#include "tm_stm32f4_ssd1306.h"
#include "stdio.h"
#include "stdio.h"
#include "dig_in.h"
#include "dig_out.h"

int main(void)
{
uint16_t nivel = 0;
float sensor = 0;
float porcentagem = 0;
char displaydata[20];
int start,inc,dec;
int auxup = 0;
int auxdown = 0;
float NivelMaximo = 0;
int alarme = 0;
int condicaoStart = 0;

//Executa a inicialização do microcontrolador
SystemInit();

//Executa a inicialização das funções de atraso
DELAY_Init();

//Executa a inicialização do ADC 1
ADClib_Init();

//Init SSD1306 LCD 128 x 64 px
TM_SSD1306_Init();

dig_in_Init();

dig_out_Init();

    /* Update screen, send changes to LCD */

Delayms(100);

while (1)
{

start  = !dig_in_Read(SW1);
inc = !dig_in_Read(SW2);
dec = !dig_in_Read(SW3);


//Executa a conversão AD para a porta PA0 - ADC12_IN0
nivel = ADClib_Read(ADC_Channel_0);

//Converte o valor analógico de 12bits em tensão
sensor = (((float)nivel*1000)/4096.0);
porcentagem = ((float)sensor/1000)*100;

        sprintf(displaydata,"Tanquecheio= %2.f",porcentagem);

        TM_SSD1306_GotoXY(0,10);
        TM_SSD1306_Puts(displaydata, &TM_Font_7x10, SSD1306_COLOR_WHITE);

        sprintf(displaydata,"NivelMaximo= %2.f",NivelMaximo);
         TM_SSD1306_GotoXY(0,20);
         TM_SSD1306_Puts(displaydata, &TM_Font_7x10, SSD1306_COLOR_WHITE);


   TM_SSD1306_UpdateScreen();


Delayms(500);
 auxup = 0;
 auxdown = 0;


 if(inc == 1 && auxup == 0  && condicaoStart == 0){
	if(NivelMaximo < 100) {
	 NivelMaximo = NivelMaximo + 5;
	}
     auxup = 1;

 }else if(dec == 1 && auxdown == 0 && condicaoStart == 0){
	 if(NivelMaximo > 0){
	 NivelMaximo = NivelMaximo - 5;
	 }
	 auxdown = 1;
 }

 if(porcentagem > (NivelMaximo + 5) && condicaoStart == 1){
	 alarme =1;
   while(alarme == 1){
	sprintf(displaydata,"ALARME REINICIE");
    TM_SSD1306_GotoXY(0,30);
	TM_SSD1306_Puts(displaydata, &TM_Font_7x10, SSD1306_COLOR_WHITE);
	TM_SSD1306_UpdateScreen();
	dig_out_Write(LED2, OUT_HI);
 	Delayms(1000);
 	dig_out_Write(LED2, OUT_LO);
 	Delayms(1000);
   }
   }
 if(start == 1 ){

	dig_out_Write(LED1, OUT_HI);
	condicaoStart = 1;
 }
 if(porcentagem> NivelMaximo && condicaoStart ==1){
	 Delayms(2000);
	 dig_out_Write(LED1, OUT_LO);
  }
 }
}
