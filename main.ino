#include <LiquidCrystal.h>

//Crear el objeto LCD con los números correspondientes (rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Sensor de corriente
float Sensibilidad=0.066; //sensibilidad en V/A para nuestro sensor
float offset=-0.50; // Equivale a la amplitud del ruido

//Declaramos variables
float Idis; //corriente de disparo controlado por el potenciometro.
float Irms; //corriente media, medido por el sensor de corriente.
float Ip; //corriente pico, medido por el sensor de corriente.

int count2=0; //contador
int count3=0; //contador
int RelayPin1 = 10; //salida temporizada
int RelayPin2 = 11; //salida normal
int RelayPin3 = 12; //salida normal
int RelayPin4 = 13; //para parlante

void setup() {
  // Inicializar el LCD con el número de  columnas y filas del LCD
  lcd.begin(16, 2);

  //Seteamos todos las salidas
	pinMode(RelayPin1, OUTPUT);
	pinMode(RelayPin2, OUTPUT);
	pinMode(RelayPin3, OUTPUT);
	pinMode(RelayPin4, OUTPUT);
  
  //Puerto serial 
  Serial.begin(9600);

  //Estado inicial de los Relay
  digitalWrite(RelayPin1,HIGH);
  digitalWrite(RelayPin2,HIGH);
  digitalWrite(RelayPin3,HIGH);
  digitalWrite(RelayPin4,HIGH);
}

void loop() {
  Ip=get_corriente();
  Irms=Ip*0.707; 
  Idis = analogRead(A3) * (10.0 / 1023.0); // lee el potenciometro

  if (Irms>15){
    lcd.setCursor(0, 0);
    lcd.print("ARRANQ");

    lcd.setCursor(0, 1);
    lcd.print(Irms);

    lcd.setCursor(8, 1);
    lcd.print(Idis);

    delay(3000);
    count3= count3+1;
  }

  if(2<Irms<15){
    lcd.setCursor(0, 0);
    lcd.print("MOVIMI");
  }
  
  if (Irms<2){
    lcd.setCursor(0, 0);
    lcd.print("PARADO");

    lcd.setCursor(8, 0);
    lcd.print("         ");

    lcd.setCursor(0, 1);
    lcd.print(Irms);
    lcd.setCursor(8, 1);
    lcd.print(Idis);
    count3=0;
    count2=0;   
    
    digitalWrite(RelayPin1,HIGH);
    digitalWrite(RelayPin2,HIGH);
    digitalWrite(RelayPin3,HIGH);
    digitalWrite(RelayPin4,HIGH);
  }

  if (count3>0){
      pulso2();
  }

}
float get_corriente()
{
  float voltajeSensor;
  float corriente=0;
  long tiempo=millis();
  float Imax=0;
  float Imin=0;
  while(millis()-tiempo<1000)//realizamos mediciones durante 0.5 segundos
  { 
    voltajeSensor = analogRead(A0) * (5.0 / 1023.0);//lectura del sensor
    corriente=0.9*corriente+0.1*((voltajeSensor-2.498)/Sensibilidad); //Ecuación  para obtener la corriente
    if(corriente>Imax)Imax=corriente;
    if(corriente<Imin)Imin=corriente;
  }
  return(((Imax-Imin)/2)-offset);
}

float pulso(){
    if (count2==0){
      delay(10000);
      digitalWrite(RelayPin1,LOW);
      count2=count2 + 1;
    }
}

float pulso2(){
    Ip=get_corriente();
    Irms=Ip*0.707; 
  if (Irms>Idis){
    lcd.setCursor(8, 0);
    lcd.print("SOBRECARGA");

    lcd.setCursor(0, 1);
    lcd.print(Irms);
    lcd.setCursor(8, 1);
    lcd.print(Idis);

    digitalWrite(RelayPin2,LOW);
    digitalWrite(RelayPin3,LOW);
    digitalWrite(RelayPin4,LOW);
    pulso();
    count2=count2 + 1;
  }else{
    lcd.setCursor(8, 0);
    lcd.print("PERMITIDO ");

    lcd.setCursor(0, 1);
    lcd.print(Irms);
    lcd.setCursor(8, 1);
    lcd.print(Idis);
    
    digitalWrite(RelayPin2,HIGH);
    digitalWrite(RelayPin3,HIGH);
    digitalWrite(RelayPin4,HIGH);
  }
}