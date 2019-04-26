#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
uchar num;
uchar display_num=0;
uchar set_point;
uchar actual_point;
uchar pwm_point;
uint kpa_point=0;
uchar isSetOrStop;
uchar motor_mode;
//uchar code table[]="SetPoint:";
uchar code table1[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
//uchar code table2[]="ActPoint:";
sbit MOSI=P1^0;
sbit MISO=P1^1;
sbit SCK=P1^2;
sbit SS=P1^3;
sbit isMotorOn=P1^4;
sbit MotorMode=P1^5;
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
	//MISO=1;
	SCK=1;
	lcdrw=0;
	lcden=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
	/*
	for(num=0;num<9;num++)
	{
		write_data(table[num]);
		delay(5);
	}
	write_com(0x80+0x40);
	for(num=0;num<9;num++)
	{
		write_data(table2[num]);
		delay(5);
	}
	*/
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
void matrixkeyscan()
{
	uchar temp;
	P3=0xfe;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xee:
				if(set_point<156)
					set_point=set_point+100;
				break;
				case 0xde:
				if(set_point>=100)
					set_point=set_point-100;
				break;
				case 0xbe:
				if(set_point<246)
					set_point=set_point+10;
				break;
				case 0x7e:
				if(set_point>=10)
					set_point=set_point-10;
				break;
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
		}
	}
	P3=0xfd;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xed:
				if(set_point<255)
					set_point=set_point+1;
				break;
				case 0xdd:
				if(set_point>=1)
					set_point=set_point-1;
				break;
				case 0xbd:
				if(isSetOrStop==0)
					isSetOrStop=1;
				else
					isSetOrStop=0;
				break;
				case 0x7d:
				set_point=0;
				break;
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
		}
	}
	P3=0xfb;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xeb:
				if(motor_mode==1)
					motor_mode=0;
				else
					motor_mode=1;
				break;
				case 0xdb:break;
				case 0xbb:break;
				case 0x7b:break;
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
		}
	}
}
void display_pwmpoint(uchar dat)
{
	uchar bai,shi,ge;
	if(dat<100)
		dat=100;
	else if(dat>200)
		dat=200;
	pwm_point=(unsigned char)((unsigned long)(dat-100)*255/100);
	bai=pwm_point/100;
	shi=(pwm_point%100)/10;
	ge=pwm_point%10;
	write_com(0x80+0x06);
	write_data(table1[bai]);
	write_data(table1[shi]);
	write_data(table1[ge]);
}
void display_kpa()
{
	uchar qian,bai,shi,ge;
	kpa_point=(unsigned int)(((double)actual_point-127.5)/51*5000);
	qian=(kpa_point%10000)/1000;
	bai=(kpa_point%1000)/100;
	shi=(kpa_point%100)/10;
	ge=kpa_point%10;
	write_com(0x80+0x40+0x06);
	write_data(table1[qian]);
	write_data(table1[bai]);
	write_data(0x2E);
	write_data(table1[shi]);
	write_data(table1[ge]);
}
void main()
{
	set_point=0;
	init();
	while(1)
	{
		matrixkeyscan();
		if(motor_mode==1)
			MotorMode=1;
		else
			MotorMode=0;
		
		if(isSetOrStop==1)
			isMotorOn=1;
		else
			isMotorOn=0;
		actual_point=master_spi(set_point);
		display_setpoint();
		display_pwmpoint(set_point);
		if(display_num==20)
		{
			display_actualpoint();
			display_kpa();
			display_num=0;
		}
		display_num++;
	}
}