#include "main.h"
#include <stdint.h>

bool Timer_50ms_Flag=0;
uint16_t Test_Time_Count=0;
uint8_t TX_Time_Cnt=0;
uint8_t Test_Status=0;
uint8_t TxBuf_Onboard[Length_Frame_Onboard_TX_RX]= {DATA_Frame_Onboard_Check_0 DATA_Frame_Onboard_Check_1 0x00};
int16_t tempInt16;

 int main(void)
 {
        
        SystemInit();
        NVIC_Configuration();
        uart_init(115200);
        Timerx_Init(0,0);//linux系统函数
    
       
     TxBuf_Onboard[Index_Frame_Onboard_Check_0]=DATA_Frame_Onboard_Check_0;
    TxBuf_Onboard[Index_Frame_Onboard_Check_1]=DATA_Frame_Onboard_Check_1;
        TxBuf_Onboard[Index_Frame_Onboard_Pitch]=128;
    TxBuf_Onboard [Index_Frame_Onboard_Roll] =128;
    TxBuf_Onboard[Index_Frame_Onboard_Yaw]=128;
    TxBuf_Onboard[Index_Frame_Onboard_Throttle]=128;
        for(uint8_t i=6;i<21;i++)
        {
            TxBuf_Onboard[i]=0x00;
        }
        

        while(1)
        {
            if(Test_Status==0&&Test_Time_Count<=200)
            {
                 if(Test_Time_Count>=200)
                 {
                    Test_Time_Count=0;
                    Test_Status=1;
                 }
            }
            
            else if(Test_Status==1&&Test_Time_Count<=100)
            {
                 if(Test_Time_Count==20)
                 {
                    TxBuf_Onboard[Index_Frame_Onboard_CMD]=Index_Onboard_CMD_Gyro_Calibrate;
                    TX_Time_Cnt=5;
                 }
                 if(Test_Time_Count>=100)
                 {
                    Test_Time_Count=0;
                    Test_Status=2;
                 }
            }
            
            else if(Test_Status==2&&Test_Time_Count<=40)
            {
                    if(Test_Time_Count==10)
                 {
                    TxBuf_Onboard[Index_Frame_Onboard_CMD]=Index_Onboard_CMD_Drone_Lock;
                    TX_Time_Cnt=5;
                 }
                  if(Test_Time_Count>=40)
                 {
                    Test_Time_Count=0;
                    Test_Status=3;
                 }
            }
            
            else if(Test_Status==3&&Test_Time_Count<=200)
            {
                if(Test_Time_Count==20)
                 {
                    TxBuf_Onboard[Index_Frame_Onboard_CMD]=Index_Onboard_CMD_Drone_Auto_Takeoff_Landing;
                    tempInt16=20;
                    TxBuf_Onboard[Index_Frame_Onboard_CMD_Data1]=((int16_t)tempInt16)>>8;
                    TxBuf_Onboard[Index_Frame_Onboard_CMD_Data2]=((int16_t)tempInt16)&0x00FF;
           
                    TX_Time_Cnt=5;
                 }
                  if(Test_Time_Count>=200)
                 {
                    Test_Time_Count=0;
                    Test_Status=4;
                 }
            }
            
            else if(Test_Status==4&&Test_Time_Count<=300)
            {
                if(Test_Time_Count==200)
                 {
           TxBuf_Onboard[Index_Frame_Onboard_Pitch]=200;
                     TX_Time_Cnt=200;
                 }
                  if(Test_Time_Count>=300)
                 {
                    Test_Time_Count=0;
                    Test_Status=5;
                 }
            }
            else if(Test_Status==5&&Test_Time_Count<=300)
            {
                if(Test_Time_Count==200)
                 {
           TxBuf_Onboard[Index_Frame_Onboard_Yaw]=200;
                     TX_Time_Cnt=200;
                 }
                  if(Test_Time_Count>=300)
                 {
                    Test_Time_Count=0;
                    Test_Status=6;
                 }
            }
            else if(Test_Status==6&&Test_Time_Count<=300)
            {
                if(Test_Time_Count==200)
                 {
           TxBuf_Onboard[Index_Frame_Onboard_Roll]=200;
                     TX_Time_Cnt=200;
                 }
                  if(Test_Time_Count>=300)
                 {
                    Test_Time_Count=0;
                    Test_Status=7;
                 }
            }
            else if(Test_Status==7&&Test_Time_Count<=300)
            {
                if(Test_Time_Count==200)
                 {
           TxBuf_Onboard[Index_Frame_Onboard_Throttle]=200;
                     TX_Time_Cnt=200;
                 }
                  if(Test_Time_Count>=300)
                 {
                    Test_Time_Count=0;
                    Test_Status=8;
                 }
            }
            else if(Test_Status==8&&Test_Time_Count<=60)
            {
                if(Test_Time_Count==20)
                 {
           TxBuf_Onboard[Index_Frame_Onboard_CMD]=Index_Onboard_CMD_Drone_Auto_Takeoff_Landing;
                     TX_Time_Cnt=5;
                 }
                  if(Test_Time_Count>=60)
                 {
                    Test_Time_Count=0;
                    Test_Status=6;
                 }
            }
            else
            {
                    ;
            }
            
            if(Timer_50ms_Flag)
            {
                if(TX_Time_Cnt==0)
                    {
                        TxBuf_Onboard[Index_Frame_Onboard_Check_0]=DATA_Frame_Onboard_Check_0;
                        TxBuf_Onboard[Index_Frame_Onboard_Check_1]=DATA_Frame_Onboard_Check_1;
                        TxBuf_Onboard[Index_Frame_Onboard_Pitch]=128;
                        TxBuf_Onboard[Index_Frame_Onboard_Roll]=128;
                        TxBuf_Onboard[Index_Frame_Onboard_Yaw]=128;
                        TxBuf_Onboard[Index_Frame_Onboard_Throttle]=128;
                        for(uint8_t i=6;i<21;i++)
                        {
                            TxBuf_Onboard[i]=0x00;
                        }
                }
                else
                {
                        TX_Time_Cnt--;
                }
                 UART_Write_DMA(TxBuf_Onboard,Length_Frame_APP_TX_RX);
            }
        }
 }
 
//∂® ±∆˜÷–∂œ50ms
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
        {
                TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
                Timer_50ms_Flag=1;
                Test_Time_Count++;
        }
}

u8 Usart1_putchar(u8 ch)
{
    USART_SendData(USART1,(u8)ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
    return ch;
}

void Usart1_putstring(u8 *buf,u8 length)
{
    while(length--)
    {
        Usart1_putchar(*buf);
        buf++;
    }
}


void UART_Write_DMA(uint8_t *pTxBuf,uint16_t Length)
{
    uint16_t Count;

    for(Count=0; Count<Length; Count++)
    {
        TxBuffer2[Count]=*(pTxBuf+Count);
    }
    DMA_Send_Data(Length);
}


void DMA_Send_Data(uint8_t length)
{
    DMA_SetCurrDataCounter(DMA1_Channel4,length);
    DMA_Cmd(DMA1_Channel4,ENABLE);
}
