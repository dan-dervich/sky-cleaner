# Funcionamiento de la SkyCleaner V2m

- [1. Tuberia y Filtracion De Agua](#tuberia-y-filtracion-de-agua)
- [2. Electrónica De La SCV2M](#electrónica-de-la-scv2m)
- [3. Sistema Elevador De La SCV2M](#sistema-elevador-de-la-scv2m)
- [4. Componentes Necesitados](#componentes)

## Tuberia y Filtracion De Agua
&nbsp;&nbsp; La SCV2M va a arrancar con dos diferentes tuberias. Una conectada a un tanque de agua desionizada que luego explicaremos lo que es, y otra conectada al tanque del edificio, osea agua de la canilla. Si uno continua por la tuberia se encontrara con una valvula derivadora. Que hace esta valvula? Basicamente decide si el agua va a correr por un lado o el otro. Esta estar justo antes de la bomba/hidrolavadora.

&nbsp;&nbsp; Luego la tuberia sale del otro lado de la bomba/hidrolavadora con una presion de 1500PSI a 7L/min. Que significa esto? El agua sale con mucha fuerza y mucha cantidad de agua, es eso maso menos lo que significa. Luego esta tuberia se econtrara con otra valvula derivadora. Esto decidira si sale surfactante + agua o solo agua. Hay dos tuberias que podemos seguir ahora. 
* Una que va al espumador donde como dice el nombre se espuma el jabon mezclandolo con agua a presion. Este espumador esta configuradoa para lanzar espuma a exactamente 60º de apertura.
* La otra nos va a llevar directo a otro difusor de agua a presion. Este difusor esta configurado para lanzar agua a 60º de apertura.

Cada difusor esta a minimo 1,16 metros de la superficie a limpiar. para poder tener un rango amplio de limpieza en este caso la maquina cubre exactamente 2 metros de ancho o 1 metro de ancho por difusor si esta al minimo.

## Electrónica De La SCV2M
&nbsp;&nbsp; Los principales roles de la parte "electronica" por asi decir en la maquina serian:
* Controlar la valvula derivadora que decide si el agua va a correr por el tanque de agua desionizada o por el tanque del edificio.
* Controlar la valvula derivadora que decide si el agua va a correr por el espumador o por el difusor de agua.
* Controlar la bomba/hidrolavadora. En nuestro caso no vamos a usar valvulas selenoides ya que podemos apagar y prender la bomba de manera facil y rapida.
* Controlar el sistema elevador de la maquina. Este sistema elevador es un sistema de poleas que permite subir y bajar la maquina. Este sistema esta controlado por un motor paso a paso y un riel de aluminio.

## Sistema Elevador De La SCV2M
&nbsp;&nbsp; Este sistema sera compuesto por dos cables que van por los costados de la maquina. Los rieles estaran pasando por agujeros hechos en la carcaza de la maquina para que pueda ser estabilizada. Estos agujeros estaran a 80cm del medio. En el medio de la carcaza habra un circulo para poder enganchar el "sistema elevador". Este sistema lo unico que es es un motor paso a paso conectado a un riel de cable/soga que estara enganchado a este circulo en el medio de la carcaza.

## Sistema De Lavado De La SCV2M
&nbsp;&nbsp; La SCV2M va a tener un sistema de lavado automatico. Este sistema va a hacer que la maquina haga 4 "ciclos de lavado". El primer "ciclo" va a tirar agua a presion por un area de 2mx6m osea va a bajar 6 metros la maquina tirando agua a presion. Luego subira 6 metros con la bomba apagada para no desperdiciar agua. Una ves que subio los 6 metros la maquina volvera a bajar 6m, esta ves con la segunda valvula derivadora abierta para que tire el jabon. Al terminar esto volvera a subir 6m y repetir el primer paso. Por ultimo uno de los pasos mas importantes que es tirar el agua desionizada. Este paso va a permitir que la ventana se pueda secar al sol y que no queden manchas ya que es agua filtrada hasta 0.01 micrones. Una ves que termino con el ultimo paso repetira los pasos anteriores hasta terminar la columna entera de 2m de ancho. Esta maquina es altamente efectiva, barata y rapida ya que al poder limpiar 2m de ancho en un solo paso y poder repetirlo 4 veces en un solo ciclo de lavado, la maquina puede limpiar 12m^2 de en un solo ciclo de lavado. Esto es muy importante ya que la maquina puede limpiar 12m^2 de ancho en 1 minuto y 30 segundos. Esto es muy rapido comparado con una persona que tardaria 10 minutos en limpiar 16m^2.

### Componentes:
* Arduino Nano
* 40 cables macho a macho
* 40 cables hembra a hembra
* 1 Valvula derivadora 2 IN 1 OUT
* 1 Valvula derivadora 1 IN 2 OUT
* 20m tuberia 1/4"
* 4 AQF T 1414TT
* hidrolavadora
* difusor 60° agua
* Espumador
* Motor paso a paso
* Riel de hand fishing
* linea de pescar

## Pseudo Code
```cpp
bool arriba = true;
bool bajando = false;
bool subiendo = false;
bool valvulaDerivadoraAgua = false;
bool valvulaDerivadoraJabon = false;
vector<string> Ciclos(4, {"Agua", "Jabon", "Agua", "AguaDes"});
int currentCiclo = 0;
void setup() {
    ...
}
void loop() {
    if(bajando == false && subiendo == false) {
        if(arriba){
            if(valvulaDerivadoraAgua == true && currentCiclo <= 2) {
                pedirCambioDeValvulaAgua()
            } else {
                Bajar();
            }
        } else if(!arriba){
            if(currentCiclo == 3) {
                bajando = false;
                subiendo = false;
                arriba = true;
            }
            if(valvulaDerivadoraAgua == true && currentCiclo <= 2 ) {
                pedirCambioDeValvulaAgua()
            } else {
                Subir();
            }
        }
    }
}
void pedirCambioDeValvulaAgua() {
    // pedir que cambien la valvula desviadora y que apreten el boton para signalarlo
    ...
}
void Bajar() {
    // Prende la hidro, espera 2s y empieza a bajar una ves que llega abajo cambia bajando a false y abajo a true.
    ...
}
void Subir() {
    // Si es el ciclo 1 apagar la hidro al principio en cualquier otro caso apagar la hidro al final.
    ...
}
```