#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
uchar num;
uchar num1=0;
uchar display_num=0;
uchar set_point;
uchar actual_point;
uchar pwm_point;
uint kpa_point=0;
uchar isSetOrStop;
uchar motor_mode;
uchar R_data=0;
uchar R_flag=0;
//uchar code table[]="SetPoint:";
uchar code table1[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
//uchar code table2[]="ActPoint:";
sbit MOSI=P2^3;
sbit MISO=P2^4;
sbit SCK=P2^5;
sbit SS=P2^6;
sbit isMotorOn=P3^2;
sbit MotorMode=P3^3;
sbit lcden=P2^2;
sbit lcdrs=P2^0;
sbit lcdrw=P2^1;
void delay_nop(uchar nop)
{
	uchar x;
	for(x=nop;x>0;x--)
	{
		_nop_();
	}
}
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void write_com(uchar com)
{
	lcdrs=0;
	P0=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_data(uchar date)
{
	lcdrs=1;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
uchar master_spi(uchar dat)
{
	uchar spi_receive;
	SCK=1;
	SS=0;
	delay_nop(10);
	for(num=0;num<8;num++)
	{
		SCK=0;
		if((dat&0x80)==0x80)
			MOSI=1;
		else
			MOSI=0;
		dat=dat<<1;
		delay_nop(10);
		SCK=1;
		spi_receive=spi_receive<<1;
		delay_nop(10);
		if(MISO==1)
			spi_receive=spi_receive|0x01;
		else
			spi_receive=spi_receive&0xfe;
		delay_nop(10);
	}
	SS=1;
	return(spi_receive);
}
void init()
{
	SS=1;
	MOSI=1;
	SCK=1;
	lcdrw=0;
	lcden=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
}
void display_setpoint()
{
	uchar bai,shi,ge;
	bai=set_point/100;
	shi=(set_point%100)/10;
	ge=set_point%10;
	write_com(0x80);
	write_data(table1[bai]);
	write_data(table1[shi]);
	write_data(table1[ge]);
}
void display_actualpoint()
{
	uchar bai,shi,ge;
	bai=actual_point/100;
	shi=(actual_point%100)/10;
	ge=actual_point%10;
	write_com(0x80+0x40);
	write_data(table1[bai]);
	write_data(table1[shi]);
	write_data(table1[ge]);
}
void uart_init()
{
	TMOD=0x20;
	PCON=0x00;
	SCON=0x50;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	ES=1;
	EA=1;
}
void uart_sendbyte(uchar byte)
{
	SBUF=byte;
	while(!TI)
	{
		_nop_();
	}
	TI=0;
}
void main()
{
	set_point=0;
	uart_init();
	init();
	while(1)
	{
		if(R_data!=0&&R_flag==1)
		{
			set_point=R_data;
			R_data=0;
			isSetOrStop=1;
			R_flag=0;
			num1=1;
		}
		if(isSetOrStop==1)
			isMotorOn=1;
		else
			isMotorOn=0;
		actual_point=master_spi(set_point);
		display_setpoint();
		if(display_num==5)
		{
			display_actualpoint();
			display_num=0;
		}
		display_num++;
		if(num1==1)
		{
			uart_sendbyte(actual_point);
			num1=0;
		}
	}
}
void ser() interrupt 4
{
	ES=0;
	if(RI==1)
	{
		R_data=SBUF;
		RI=0;
		R_flag=1;
	}
	ES=1;
}