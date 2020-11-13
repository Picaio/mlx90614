
/* Termometro de personas V0.1
 * PICAIO SAS
 */

//Librerias
#include <Wire.h> //comms
#include <Adafruit_MLX90614.h> //Termico
#include <LiquidCrystal_I2C.h> //Pantalla
#include <SR04.h> //Ultrasonido

//Definicion de Pines
#define Echo 11 //Echo del Ultrasonido
#define Trig 10 //Trig del Ultrasonido
#define LP 9 //Salida LED

//Inicializar
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //Sensor Termico
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Pantalla LCD
SR04 sr04=SR04(Echo,Trig); //Ultrasonido

//Variables
int Espera1=300; //Espera en el loop

//Distancia
int Dist; //Distancia del ultrasonico
int DistMin=10; //Distancia minima para detectar al sujeto (mm)
int Presente=0; //Si hay alguien frente al Termometro
int Espera=500; //Tiempo de espera para verificar sujeto
unsigned long Tiempo=0; //Tiempo que lleva detectado para Millis
int Ahora=0; //Millis en el momento que se inicia

//Temperatura
float TempObj; //Temperatura del sujeto
float TempMax=37.50; //Temperatura maxima permitida
int TpoAlarma=200; //Tiempo de Alarma por alta temperatura

void setup() 
{
  pinMode(LP,OUTPUT); //LED
  mlx.begin(); //Termico

  Serial.begin(115200);
  //Sonido inicial
 // tone(12,NOTE_C5,3000);
  digitalWrite(LP,HIGH);
  delay(3000);
  digitalWrite(LP,LOW);

//Mensaje inicial
  lcd.begin(20,4);
  lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print(" TERMOMETRO / RELOJ");
  lcd.setCursor(0,0);
  lcd.print("AMB: ");
  lcd.setCursor(0,1);
  lcd.print("PER: ");
}

void loop() 
{
//Distancia
Dist=sr04.Distance();
if(Dist>DistMin) //No hay nadie
{
  Presente=0;
  Tiempo=millis();
}
if(Dist<=DistMin && Presente==0) //Llego alguien, tomemos el tiempo
{
  Presente=1;
  Tiempo=millis();
}

if(Presente==1)
{
  if(millis()-Tiempo>Espera) //Se completo el tiempo
  {
    Presente=2;
  }
}

//Actualizar pantalla
//lcd.setCursor(12,3);
//Temperaturas
lcd.setCursor(4,0);
lcd.print(mlx.readAmbientTempC());
lcd.setCursor(9,0);
lcd.print("c");
Serial.print("A");
Serial.println(mlx.readAmbientTempC());

TempObj=mlx.readObjectTempC();
switch(Presente)
{
  case 0: //No hay nadie
  lcd.setCursor(4,1);
  lcd.print("--.--c");
  lcd.setCursor(10,1);
  lcd.print(" LOOK.");
  Serial.println("BUSCANDO...  "); 
  Serial.println("B");// Se envia B para el dato
  break;

  case 1: //Llego alguien
  //lcd.setCursor(4,2);
  //lcd.print("--.--c");
  lcd.setCursor(4,1);
  lcd.print("MEASURING...");
  Serial.println("LEYENDO...");
  //tone(12,NOTE_C5,TpoAlarma);
  break;

  case 2: //Se Completo el tiempo
  lcd.setCursor(4,1);
  lcd.print(TempObj);
  lcd.setCursor(9,1);
  lcd.print("c");
  Serial.print("T"); // temperatura medida
  Serial.println(TempObj);
  if(TempObj>TempMax)
  {
   // tone(12,NOTE_G5,TpoAlarma);
   lcd.setCursor(10,1);
   lcd.print(" ALTA!");
   digitalWrite(LP,HIGH);
   delay(TpoAlarma);
   digitalWrite(LP,LOW);
   Serial.println("D"); // temperatura alta
  }
  else
  {
  lcd.setCursor(10,1);
  lcd.print(" GOOD");
  }
  break;
}


delay(Espera1);

}
