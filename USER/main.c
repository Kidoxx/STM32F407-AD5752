#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "AD5752.h"

int main(void)
{ 
  long int cstr = 0;

  long int buf;
	unsigned	char	tx[3] = {0,0,0};
	
	long int a=0;
  double Voltage_A=9.99969,Voltage_B=-9.99969;	//�����ѹ-10<Voltage<10
  unsigned int c=0;
    
				 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	AD5752_Init();	//AD5752��ʼ��
	ConfigAD5752();
	while(1)
	{
		cstr = DAC_Register | DAC_Channel_A | 0x00FFFF;		//VoutA=9.9996v
		WriteToAD5752Spi(&cstr);	//д������

		cstr = DAC_Register | DAC_Channel_B | 0x00AAAA;		//VoutB=3.33V
		WriteToAD5752Spi(&cstr);	//д������
	
		cstr = Read | DAC_Register | DAC_Channel_B;   //������Ӧ���� "00 AA AA"
		WriteToAD5752Spi(&cstr);	

		delay_us(20);

		ReadFromAD5752Spi(&buf);	  //Read select register, using two instruction cycles 
			
	  tx[2] = ((buf)&0xFF0000)>>16;	
		tx[1] = ((buf)&0x00FF00)>>8;	
		tx[0] = (buf)&0x000000FF;
		if(Voltage_A>=0)
		{
			c=Voltage_A/10*32768;	//ת���ɵ�ѹ
		}
		else
		{
			c=(32768-(-1*Voltage_A/10*32768));
			c=c&8000;
		}
		cstr = DAC_Register | DAC_Channel_B | c;		
		WriteToAD5752Spi(&cstr);		
	}
}

