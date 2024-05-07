
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);
//Direccion de default PCA9685 0x40
// 110 posicion 0° y 546 180°
const byte AIA = 5;  // (pwm) pin 9 connected to pin A-IA 
const byte BIA = 6; // (pwm) pin 10 connected to pin B-IA  
const byte CIA = 9 ; // (pwm) pin 10 connected to pin B-IA  
const byte DIA = 10;  // (pwm) pin 6 connected to pin B-IB 
const byte velmotores = 255;
byte gruposervo[16]={55,55,55,3,4,1,2,55,55,0,5,3,2,55,55,55}; //0=>'A' 1=>'B' 2=>'C' 3=>'D' 4='E' 5=>'F' 6=>'G' 7=>'H' 55=>'no asignado
int posiciones[16];
int velocidades[16][5]={0}; //[][0] >subida |[][1] > conteo maximo pausa | [][2] > bajada | [][3] > estado de servo | [][4] > conteo de pausa

float Tiempos[16][3]={{2.5,2,2},{4,1.5,2},{1.5,2.5,3},{3,2.5,1.5},{2,0.5,1},{1,0.5,2}};//campo 1 es tiempo de subida, 2 pausa y 3 bajada
byte Bsubidalista[16];
byte Bbajadalista[16];
int ContadorPausa[16];
float Texe=5.08;
float Textra=46.7;

void setup() {
  // put your setup code here, to run once:
  analogWrite(AIA, 0);
  analogWrite(BIA, 0);
  analogWrite(CIA, 0);
  analogWrite(DIA, 0);

Serial.begin(9600);
//Iinicializacion modulos
  board1.begin();
  board1.setPWMFreq(50);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
 for(int i=0;i<=15;i++)
 {
  posiciones[i]=110;
  Serial.println(posiciones[i]);
 }

//Tiempo de subida a bits de desplazamiento
float Periodo=Texe+Textra;
//Serial.println(Periodo);
for(int i=0;i<=5;i++)         //Cálculo de bits de subida para cada grupo
  {Bsubidalista[i]=round(Periodo*0.218/Tiempos[i][0]);}
  
//Tiempo de pausa

  for(int i=0;i<=5;i++)       //Cálculo de número de lazos de pausa para cada grupo
  {ContadorPausa[i]=round(Tiempos[i][1]*1000/(Periodo))-1;}
  
//Tiempo de bajada a bits de desplazamiento
  for(int i=0;i<=5;i++)     
  {Bbajadalista[i]=round((Periodo)*0.218/Tiempos[i][2]);}

//Cálculo de velocidades para cada servo
for (int i=0;i<16;i++)
{
  if (gruposervo[i]==55) velocidades[i][0]=0; 
  else  velocidades[i][0]=Bsubidalista[gruposervo[i]]; 
}


for (int i=0;i<16;i++)              //ASIGNACION de TIEMPO DE PAUSA a cada servo segun su grupo
{
  if (gruposervo[i]==55) velocidades[i][1]=0; 
  else  velocidades[i][1]=ContadorPausa[gruposervo[i]]; 
}

for (int i=0;i<16;i++)            //ASIGNACION de TIEMPO DE BAJADA para cada servo segun su grupo
{
  if (gruposervo[i]==55) velocidades[i][2]=0; 
  else  velocidades[i][2]=Bbajadalista[gruposervo[i]]*-1; 
}
for (int i=3;i<14;i++) //Inicio de posicion de servos
  {
  EjecutaServo(i);
  }
delay(5000);
Motores();

}

void loop() 
{ 
  int i,estado;
  // put your main code here, to run repeatedly:
//long int t1=micros();
digitalWrite(13,HIGH);
    for (i=3;i<13;i++) //cambiando los estados de los 32 servos
    {
      switch (velocidades[i][3])
        {
          case 0:
                 if (posiciones[i]<328) 
                  {
                    EjecutaServo(i);
                    posiciones[i]+=velocidades[i][0];
                  }
                  else {velocidades[i][3]++;posiciones[i]=328;EjecutaServo(i);}
          break; 
          case 1:
    
                  if ( velocidades[i][4]<velocidades[i][1])
                    {
                      EjecutaServo(i);
                      velocidades[i][4]++;
                    }
                  else {velocidades[i][3]++;velocidades[i][4]=0;EjecutaServo(i);}
                  break;
          case 2:
                  if (posiciones[i]>110) 
                    {
                      EjecutaServo(i);
                      posiciones[i]+=velocidades[i][2];
                    }
                  else {velocidades[i][3]=0;posiciones[i]=110;EjecutaServo(i);}
          break;
      }
    }
    digitalWrite(13,LOW);
  
//long int t2 = micros();
//Serial.print("Time taken by the task: "); Serial.print(t2-t1); Serial.println(" microseconds");
delay(Textra);
//long int t3 = micros();
//Serial.print("Time taken by the task: "); Serial.print(t3-t1); Serial.println(" microseconds"); 


}

void EjecutaServo(int numservo)
{
  if (gruposervo[numservo]==55) return;

else
    board1.setPWM(numservo,0,posiciones[numservo]);

}

void Motores()
{
  analogWrite(CIA, 255);

  analogWrite(AIA, velmotores);
  analogWrite(BIA, velmotores);
  analogWrite(CIA, 150);
  analogWrite(DIA, velmotores);
}