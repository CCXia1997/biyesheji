unsigned long lastTime; //上一次的采样时间
byte set_point=0; //设定值
byte pwm_point=0; //PWM波的占空比（0~255）
byte actual_point; //采集值
double Outout; //PID控制的输出
double errSum; //误差累积值，用于积分控制
double kp,ki; //比例控制和积分控制的系数
byte motor_mode=0;
int motor_speed=0;
int SPI_MOSI=10;
int SPI_MISO=11;
int SPI_SCK=12;
int SPI_SS=13;
int ENA=3;
int TRANS=6;
int IN1=7;
int IN2=8;
int isSet=9;
byte slave_spi(byte dat) //SPI通信
{
    byte spi_receive=0;
    while(!digitalRead(SPI_SS));
    while(digitalRead(SPI_SS));
    for(int i=0;i<8;i++)
    {
      while(digitalRead(SPI_SCK));
      spi_receive=spi_receive<<1;
      if(digitalRead(SPI_MOSI)==1)
        spi_receive=spi_receive|0x01;
      else
        spi_receive=spi_receive&0xFE;
      while(!digitalRead(SPI_SCK));
      if((dat&0x80)==0x80)
        digitalWrite(SPI_MISO,HIGH);
      else
        digitalWrite(SPI_MISO,LOW);
        dat=dat<<1;
    }
  while(!digitalRead(SPI_SS));
  return(spi_receive);
}
void Compute()
{
	unsigned long now=millis(); //获取当前时间
	double timeChange=(double)(now-lastTime);
	double error=(double)(set_point-actual_point); //计算当前偏差
	errSum+=(error*timeChange)/1000; //误差累积值计算
	Outout=kp*error+ki*errSum; //计算PID输出值
	
	lastTime=now; //调整上一次时间值以便下一次调用
}
void setTunings(double Kp,double Ki) //设置PID控制系数
{
	kp=Kp;
	ki=Ki;
}
void motor_pwm() //设置输出的PWM波的占空比
{
	TCCR2A=_BV(COM2B1)|_BV(WGM21)|_BV(WGM20);
	TCCR2B=_BV(CS22);
	OCR2B=pwm_point;
	if(motor_mode==0)
	{
	  digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);
	}
	if(motor_mode==1)
	{
	  digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
	}
}
void setup() {
  // put your setup code here, to run once:
  pinMode(A1,INPUT);
  digitalWrite(A1,LOW);
  pinMode(SPI_MOSI,INPUT);
  pinMode(SPI_MISO,OUTPUT);
  pinMode(SPI_SCK,INPUT);
  pinMode(SPI_SS,INPUT);
  pinMode(TRANS,INPUT);
  pinMode(isSet,INPUT);
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  setTunings(9,12); //设置PID控制系数
}

void loop() {
  // put your main code here, to run repeatedly:
  actual_point=(byte)map(analogRead(A1),0,1023,0,255); //采集电压值并换算成0~255范围
  set_point=slave_spi(actual_point); //SPI交换数据,接收设定值，发送采集值
  if(digitalRead(isSet)==1) //一开始不能进行PID运算，得等51发送设定值过来才行
  {
	Compute(); //PID运算
	if(Outout<86) //输出为PWM占空比，应该调整为86~255之间的值，因为小于86的占空比没啥效果
		Outout=86;
	else if(Outout>255)
		Outout=255;
	pwm_point=(byte)Outout;
	motor_pwm();
  }
  else
  {
	TCCR2A=0;
    TCCR2B=0;
    OCR2B=0;
    digitalWrite(ENA,LOW);
	lastTime=millis(); //不执行PID运算的话时间也需要进行设定
  }
}
