byte set_point=0;
byte pwm_point=0;
byte actual_point;
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
byte slave_spi(byte dat)
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
}
void motor_pwm()
{
  TCCR2A=_BV(COM2B1)|_BV(WGM21)|_BV(WGM20);
  TCCR2B=_BV(CS22);
  pwm_point=(byte)((word)(set_point-100)*255/100);
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

void loop() {
  // put your main code here, to run repeatedly:
  actual_point=(byte)map(analogRead(A1),0,1023,0,255);
  set_point=slave_spi(actual_point);
  if(set_point<100)
    set_point=100;
  else if(set_point>200)
    set_point=200;
  if(digitalRead(TRANS)==1)
  {
    motor_mode=1;
  }
  else
  {
    motor_mode=0;
  }
  if(digitalRead(isSet)==1)
  {
    motor_pwm();
  }
  else
  {
    TCCR2A=0;
    TCCR2B=0;
    OCR2B=0;
    digitalWrite(ENA,LOW);
  }

}
