//definicion de pins
const int motorPin1 = 2;    // 28BYJ48 In1
const int motorPin2 = 3;    // 28BYJ48 In2
const int motorPin3 = 4;   // 28BYJ48 In3
const int motorPin4 = 5;   // 28BYJ48 In4
                   
//definicion variables
int motorSpeed = 2000;   //variable para fijar la velocidad
int stepCounter = 0;     // contador para los pasos
int stepsPerRev = 4076;  // pasos para una vuelta completa

int cicloActual = 0; // variable para siempre saber por que ciclo va
const char Ciclos[4] = {'A', 'J', 'A', 'D'}; // nombres de los ciclos...
/* 
  A: Agua 
  J: Jabon
  D: Agua Desionizada
*/
bool cicloArrancado = false; // para saber si se puede arrancar el proximo ciclo o no

bool abajo = false; // la variable esta es para saber si la maquina esta abajo
bool arriba = true; // la variable esta es para saber si la maquina esta arriba


bool valvulaAguaDes = false; // estado valvula de agua desionizada
bool valvulaJabon = false; // estado vavluvla de jabon

//tablas con la secuencia de encendido
//secuencia 1-fase
const int numSteps = 4;
const int stepsLookup[4] = { B1000, B0100, B0010, B0001 };

const int relay_pin = 6;

// pines  para led RGB
const int r_pin = 14;
const int g_pin = 15;
const int b_pin = 16;

// pin para boton
const int button_pin = 9;
void setup()
{
  //declarar pines como salida
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(r_pin, OUTPUT);
  pinMode(g_pin, OUTPUT);
  pinMode(b_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  if(cicloArrancado) return; // fijarse si hay un ciclo de lavado ya iniciado ya que no podemos tener dos ciclos de lavados en simultaneo...
  if(Ciclos[cicloActual] == 'A'){ // fijarse si es ciclo A, J o D
    Serial.println(cicloActual);
    digitalWrite(r_pin, LOW); // prender led verde inicialmente luego si se debe cambiar a rojo o azul se vera 
    digitalWrite(g_pin, HIGH);
    digitalWrite(b_pin, LOW);
    if(valvulaJabon) { // fijarse si la valuvla de jabon esta en el etastado que debe estar
      CambiarValvulaJabon(); // pedir cambio de estado de valvula
    }
    if(valvulaAguaDes) { // fijarse si la valuvla de jabon esta en el etastado que debe estar
      CambiarValvulaDes(); // pedir cambio de estado de valvula
    }
    prenderBomba(); // prender bomba para que impieze a fluir el agua denuevo
    if(arriba) { // si la maquina esta arriba empezar a bajar
      CicloBajando(); // bajar maquina
    } 
    if(abajo) { // si la maquina esta abajo empezar a subirla
      CicloSubiendo(); // subir maquina
    }
    apagarBomba();
    cicloActual += 1; // adelantar para poder cambiar de ciclo
  } else if(Ciclos[cicloActual] == 'J') { // ciclo jabon
    Serial.println(cicloActual);
    if(!valvulaJabon) { // si la valvula de jabon no esta "cerrada" por asi decier 
      CambiarValvulaJabon();
    }
    if(valvulaAguaDes) {
      CambiarValvulaDes();
    }
    if(arriba) {
      prenderBomba();
      CicloBajando();
      apagarBomba();
    } 
    if(abajo) {
      CicloSubiendo();
    }
    cicloActual += 1;
  } 
  if(Ciclos[cicloActual] == 'D'){
    Serial.println(cicloActual);
    if(valvulaJabon) {
      CambiarValvulaJabon();
    }
    if(!valvulaAguaDes) {
      CambiarValvulaDes();
    }
    prenderBomba();
    if(arriba) {
      CicloBajando();
    } 
    if(abajo) {
      CicloSubiendo();
    }
    if(arriba) {
      CicloBajando();
      apagarBomba();
      cicloActual = 0;
    }
  }
}

void CambiarValvulaJabon() {
  Serial.println("CAMBIAR AGUA JABON");
  bool valvulaCambiadaBool = valvulaJabon;
  digitalWrite(r_pin, HIGH);
  digitalWrite(g_pin, LOW);
  digitalWrite(b_pin, LOW);
  while(true) {
    if(valvulaJabon != valvulaCambiadaBool){
      return;
    }
    ValvulaCambiada(false);
  }
}
void CambiarValvulaDes() {
  Serial.println("CAMBIAR AGUA DES");
  bool valvulaCambiadaBool = valvulaAguaDes;
  digitalWrite(r_pin, LOW);
  digitalWrite(g_pin, LOW);
  digitalWrite(b_pin, HIGH);
  while(true) {
    if(valvulaAguaDes != valvulaCambiadaBool){
      return;
    }
    ValvulaCambiada(true);
  }
}
void ValvulaCambiada(bool AguaDes) {
  bool boton = digitalRead(button_pin);
  Serial.println(boton);
  // boton no se apreto todavia
  if(boton == LOW) return;
    if(AguaDes) {
      valvulaAguaDes = !valvulaAguaDes;
    } else if(!AguaDes) {
      valvulaJabon = !valvulaJabon;
    }
    digitalWrite(r_pin, LOW);
    digitalWrite(g_pin, HIGH);
    digitalWrite(b_pin, LOW);
}
void prenderBomba() {
  digitalWrite(relay_pin, HIGH);
}

void apagarBomba() {
  digitalWrite(relay_pin, LOW);
}

void CicloBajando() {
  cicloArrancado = true;
  Serial.println("Arranco");
  for (int i = 0; i < stepsPerRev; i++)
  {
    clockwise();
    delayMicroseconds(motorSpeed);
  }
  Serial.println("Termino");
  arriba = false;
  abajo = true;
  cicloArrancado = false;
}
void CicloSubiendo() {
  cicloArrancado = true;
  Serial.println("Arranco");
  for(int i = 0; i < stepsPerRev; i++)
  {
    anticlockwise();
    delayMicroseconds(motorSpeed);
  }
  Serial.println("Termino");
  arriba = true;
  abajo = false;
  cicloArrancado = false;
}
void clockwise()
{
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  setOutput(stepCounter);
}

void anticlockwise()
{
  stepCounter--;
  if (stepCounter < 0) stepCounter = numSteps - 1;
  setOutput(stepCounter);
}

void setOutput(int step)
{
  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}