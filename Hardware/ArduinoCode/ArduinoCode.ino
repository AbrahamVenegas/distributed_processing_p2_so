//Pines de los botones.
const int buttonPins[] = {2, 3, 4, 5}; //Pines digitales conectados a los botones.
const int numButtons = 4; //Numero de botones.

//Pines para los LEDS y el Speaker.
const int ledPinInProgress = 8; //Led que indica que aun no hay una combinacion de cuatro botones.
const int ledPinSuccess = 7; //LED que indica que se esta reproducioendo algun numero.
const int speakerPin = 6; //Pin para el speaker.

//Variable para conteo de presiones de boton.
int pressCount = 0; //Contador de presiones de boton.
String buttonSequence = ""; //Cadena para almacenar la secuencia de botones.
bool sequenceSent = false; //Bandera para saber si al secuencia se envio.

//Set up del codigo del arduino.
void setup() {
  Serial.begin(9600); //Inicializacion del serial.

  //Configuracion de los botones como entrada con resistencias pull-down.
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT); //SIn pull-up porque ya posee un pull-down fisico.
  }

  //Configuracion de los pines de LEDS y del speaker como salida.
  pinMode(ledPinInProgress, OUTPUT); //Configuracion pin de progreso.
  pinMode(ledPinSuccess, OUTPUT); //Configuracion pin de resultados.
  pinMode(speakerPin, LOW); //Configuracion pin del speaker.

  //Estado inicial de LEDS y speaker.
  digitalWrite(ledPinInProgress, HIGH); //LED de proceso encendido.
  digitalWrite(ledPinSuccess, LOW); //LED de resultado apagado.
  digitalWrite(speakerPin, LOW); // Speaker apagado.
}

void loop() {
  checkButtonPresses(); //Revisa las presiones de los botones

  if (!sequenceSent && buttonSequence.length() >= 4){
    sendSequenceToC(); //Envia la secuencia a C
  }

  if (sequenceSent && Serial.available() > 0){
    String binaryResponse = Serial.readStringUntil('\n'); //Lee la respuesta en binario
    playBinaryResponse(binaryResponse); //Reproduce la secuencia en el speaker
  }
}

//Funcion para revisar el estado de los botones
void checkButtonPresses(){
  for (int i = 0; i < numButtons; i++){
    if (digitalRead(buttonPins[i]) == HIGH) {
      buttonSequence += String(i+1);
      Serial.print("Button ");
      Serial.print(i+1);
      Serial.println(" pressed");

      delay(200); //Debounce basico para evitar lecturas multiples

      //Esperar que se suelte el boton
      while (digitalRead(buttonPins[i]) == HIGH);
    }
  }
}

//Funcion para enviar la secuencia.
void sendSequenceToC(){
  Serial.println(buttonSequence); //Envia la secuencia a C.
  sequenceSent = true; //Marca que la secuencia fue enviada.
  digitalWrite(ledPinInProgress, LOW); //Apaga el LED de proreso.
  digitalWrite(ledPinSuccess, HIGH); //Enciende el LED de exito.
}

//Funcion para reproducir la secuencia binaria en el speaker.
void playBinaryResponse(String binaryResponse) {
  for (char bit : binaryResponse){
    if (bit == '1'){
      digitalWrite(speakerPin, HIGH); //Sonido pr 1 segundo.
      delay(1000);
      digitalWrite(speakerPin, LOW);
    } else if(bit == '0'){
      delay(2000); //Silencio or 2 segundos.
    }
  }

  //Apaga LED de exito al finalizar.
  digitalWrite(ledPinSuccess, LOW);
  sequenceSent = false; //Reinicia la bandera para una nueva secuencia.
  buttonSequence = ""; //Reinicia la secuencia de botones.
  pressCount = 0; //Reinicia el conteo de los botones.
}
