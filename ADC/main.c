#include "printf.h"  
#include "adc.h"  
#include "stm32f10x.h"  
  
extern __IO uint16_t ADC_ConvertedValue;  
float ADC_ConvertedValueLocal;  
void Delay(__IO uint32_t nCount)  
{  
   for(;nCount !=0;nCount--);  
}  
int main(void)  
{    
  printf_init();      
  adc_init();  
  printf("******This is a ADC test******\n");  
          
  while(1) {  
    ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3;  
    printf("The current AD value =0x%04X\n",ADC_ConvertedValue);  
    printf("The current AD value =%f V\n",ADC_ConvertedValueLocal);    
    Delay(0xffffee);  
  }  
  
  return 0;   
}  