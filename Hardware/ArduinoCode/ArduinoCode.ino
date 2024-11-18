// Pines de los botones.
const int buttonPins[] = {2, 3, 4, 5}; // Pines digitales conectados a los botones.
const int numButtons = 4;             // Número de botones.

// Pines para los LEDs y el Speaker.
const int ledPinInProgress = 8;       // LED que indica que aún no hay una combinación de cuatro botones.
const int ledPinSuccess = 7;          // LED que indica que se está reproduciendo algún número.
const int speakerPin = 6;             // Pin para el speaker.

// Variables de estado.
String buttonSequence = "";           // Cadena para almacenar la secuencia de botones.
bool sequenceSent = false;            // Bandera para saber si la secuencia se envió.
int pressCount = 0;

void setup() {
  Serial.begin(9600); // Inicialización del puerto serial.

  // Configuración de los botones como entrada.
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT);
  }

  // Configuración de los LEDs y del speaker como salida.
  pinMode(ledPinInProgress, OUTPUT);
  pinMode(ledPinSuccess, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // Estado inicial de LEDs y speaker.
  digitalWrite(ledPinInProgress, HIGH); // LED de progreso encendido.
  digitalWrite(ledPinSuccess, LOW);     // LED de éxito apagado.
  digitalWrite(speakerPin, LOW);        // Speaker apagado.
}

void loop() {
  // Revisar presiones de botones.
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      buttonSequence += String(i + 1); // Añadir el número del botón presionado.
      pressCount++;
      //Serial.print("Button ");
      //Serial.print(i + 1);
      //Serial.print(" pressed");

      delay(200); // Pequeño debounce.

      // Esperar a que se suelte el botón.
      while (digitalRead(buttonPins[i]) == HIGH);
    }
  }

  // Enviar la secuencia si tiene al menos 4 caracteres.
  if (pressCount >= 4){
    Serial.println(buttonSequence);
    sequenceSent = true;
    digitalWrite(ledPinInProgress, LOW);
    digitalWrite(ledPinSuccess, HIGH);
  }
  
  if (sequenceSent && Serial.available() > 0){
    String binaryResponse = Serial.readStringUntil('\n');
    playBinary(binaryResponse);

    digitalWrite(ledPinSuccess, LOW);
    sequenceSent = false;
    buttonSequence = "";
    pressCount = 0;
    digitalWrite(ledPinInProgress, HIGH);    
  }
}

void playBinary(String binaryResponse){
  for (char bit : binaryResponse) {
    if (bit == '1') {
      digitalWrite(speakerPin, HIGH);
      delay(1000);
      digitalWrite(speakerPin, LOW);
    }else if (bit == '0'){
      delay(2000);
    }
  }
}
