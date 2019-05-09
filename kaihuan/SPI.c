#include<reg52.h>
#define uchar unsigned char //宏定义，后面使用unsigned char类型可以直接用uchar代替
//详细解释stc89c52和Arduino之间的软件SPI通信

uchar master_spi(uchar dat) //stc89c52作为主机的SPI通信函数，传入要发送的8位数据，返回接收到的8位数据
{
	uchar spi_receive; //定义一个8位无符号整型变量用于存放接收数据
	SCK=1; //时钟信号置1
	SS=0; //片选信号置0，通知从机自己被选中
	delay_nop(10); //等待从机执行后续指令，让其在下一个while循环等待
	for(num=0;num<8;num++) //8次循环，每次循环处理一位数据
	{
		SCK=0; //时钟端置0产生下降沿信号，准备发送数据，同时通知从机开始接受数据
		if((dat&0x80)==0x80) //0x80化为二进制为10000000，等效于判断要发送数据的最高位是否为1
			MOSI=1; //如果是1则发送1
		else
			MOSI=0; //如果是0则发送0
		dat=dat<<1; //主机发送的数据向左移一位，这样下一次循环最高位就是下一位的数据
		delay_nop(10); //等待从机接收这一位的数据，让其在下一个while循环等待
		SCK=1; //时钟端置1产生上升沿信号，准备接收数据，同时通知从机开始发送数据
		spi_receive=spi_receive<<1; //接收数据往左移一位，这样下一次循环最低位就是要接收的下一位数据
		delay_nop(10); //等待从机将当前位的数据发送过来，让其在下一个while循环等待
		if(MISO==1) //检测接收到的当前位数据是否为1
			spi_receive=spi_receive|0x01; //如果是1则接收变量的最低位写入1
		else
			spi_receive=spi_receive&0xfe; //如果是0则接收变量的最低位写入0
		delay_nop(10); //等待从机执行后续指令，让其在下一个while循环等待
	}
	SS=1; //8次循环结束后完成8位数据的交换，将片选信号置1，通知从机已经不被选中，从机可以自由执行自己的后续指令
	return(spi_receive); //返回接收到的数据
}



byte slave_spi(byte dat) //Arduino作为从机的SPI通信函数，byte是Arduino语言中的8位无符号整型
{
    byte spi_receive=0; //定义一个8位无符号整型变量用于接收数据
    while(!digitalRead(SPI_SS)); //这边两个while循环是为了让Arduino能检测到1->0的片选信号表示自己被选中
    while(digitalRead(SPI_SS));  //如果单单只有1个检测的话，会出现当主机在执行for循环时（此时SS=0），从机也进入for循环导致数据收发混乱
    for(int i=0;i<8;i++) //8次循环，每次循环处理一位数据
    {
      while(digitalRead(SPI_SCK)); //等待主机发出时钟负跳变信号
      spi_receive=spi_receive<<1; //检测到时钟负跳变信号后开始接受数据，原理同主机接收数据
      if(digitalRead(SPI_MOSI)==1)
        spi_receive=spi_receive|0x01;
      else
        spi_receive=spi_receive&0xFE;
      while(!digitalRead(SPI_SCK)); //检测到时钟正跳变后开始发送数据，原理同主机发送数据
      if((dat&0x80)==0x80)
        digitalWrite(SPI_MISO,HIGH);
      else
        digitalWrite(SPI_MISO,LOW);
        dat=dat<<1;
    }
  while(!digitalRead(SPI_SS)); //等待主机将片选信号置1解除对自身的选择，这样从机可以自由执行后续的指令
  return(spi_receive); //返回接收到的数据
}