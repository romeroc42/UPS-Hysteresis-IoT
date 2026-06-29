//Se definen variables
//Define variables
//Histéresis para batería
//Battery Hysteresis
float Histb;
//Voltaje de Batería
//Battery Voltage
float Vb;
//Voltaje de Batería superior
//Higher Battery Voltage
float Vlbu;
//Voltaje de Batería inferior
//Lower Battery Voltage
float Vlbd;
//Voltaje medio de Batería
//Medium Battery Voltage
float Vnb;
//Entrada analógica
//Analog Input
float a1;
//Estado del pin digital 5
//Digital Pin 5 Status
bool d5;
//Se define una variable para el estado de la conmutación
//Define a variable for switching status
int state;

void setup() {
// put your setup code here, to run once:
//Valores de inicialización para cada una de las variables
//Initialization values ​​for each of the variables
Vb = 0.0;
Histb = 0.5;
Vnb = 11.2;
Vlbu = Vnb + Histb;
Vlbd = Vnb - Histb;
a1 = 0.0;
d5 = 0;
//Establecer rol de pin de lectura si está disponible la Red Eléctrica
//Set read pin role if the Power Grid is available
pinMode(5, INPUT);
//Establecer roles de pines de salida
//Set roles for output pines
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
//Usar la Red Eléctrica por defecto
//Use Electrical Grid by default
digitalWrite(6, LOW);
//Esperar un segundo para evitar interferencias
//Wait one second to avoid interferences
delay(1000);
//Estado de conmutación será 0 (Usa Red Eléctrica)
//The switching status will be 0 (Use Electric Grid)
state = 0;
//Encender inversor siempre
//Always power on inverter
digitalWrite(7, HIGH);
//Esperar a que encienda inversor
//Wait the inverter to turn on
delay(4000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Lectura de voltaje de batería
  //Battery voltage reading
  a1 = analogRead(1);
  Vb = (a1*20.0)/1024.0;
  //Lectura si está disponible la Red Eléctrica
  //Read if Electric Grid is available
  d5 = digitalRead(5);
  //Si Voltaje de batería es mayor a voltaje superior de batería
  //"Y" No está disponible la Red Eléctrica
  //If battery voltage is greater than the higher battery voltage
  //"AND" Electric Grid is unavailable
  if ((Vb>Vlbu) && (d5 == LOW)){
    //Si Estado de conmutación es 0 (Usando Red Eléctrica)
    //If Switching State is 0 (Use Electric Grid)
    if (state==0){
      //Conectar inversor a la carga o salida
      //Connect inverter to the load or output
      digitalWrite(6, HIGH);
      //Notificar estado de conmutación 1 (Usando Batería)
      //Notify switching status 1 (Using Battery)
      state = 1;
    }
  //Si también está disponible la Red Eléctrica
  //If the Electrical Grid is also available
  }else if ( (d5 == HIGH) ){
    //Si Estado de conmutación es 1 (Usando Batería)
    //If Switching State is 1 (Using Battery)
    if (state==1){
      //Cambiar estado de conmutación a 0 (Usar Red Eléctrica)
      //Change switching state to 0 (Use Electrical Grid)
      digitalWrite(6, LOW);
      //Notificar estado de conmutación 0 (Usar Red Eléctrica)
      //Notify switching status 0 (Use Electrical Grid)
      state = 0;
    }
  }else{
  //Si los voltajes están dentro de los valores que soporta la histéresis
  //No se toma ninguna acción
  //If the voltages are within the hysteresis tolerance
  //No action is taken
  }
}
