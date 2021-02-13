# STM32F103_LINBUS
Example code to use LINBUS 

void USART3_IRQHandler(void) will be called when LIN break  is detected on LINBUS. After that we read two fealds, one will be sync, other identifier. If identifier is correct 
we will send message and calculate CRC of data. So we send DATA[8]+CRC. 

LIN is running at 19200Bps with TPIC1021AQDRQ1 Tranciever IC.

