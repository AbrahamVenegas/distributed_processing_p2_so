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
      //Serial.print("Button ");
      //Serial.print(i + 1);
      //Serial.println(" pressed");

      delay(200); // Pequeño debounce.

      // Esperar a que se suelte el botón.
      while (digitalRead(buttonPins[i]) == HIGH);
    }
  }

  // Enviar la secuencia si tiene al menos 4 caracteres.
  if (!sequenceSent && buttonSequence.length() >= 4) {
    Serial.println(buttonSequence);   // Enviar secuencia completa al programa en C.
    sequenceSent = true;              // Marcar que se envió la secuencia.
    digitalWrite(ledPinInProgress, LOW); // Apagar LED de progreso.
    digitalWrite(ledPinSuccess, HIGH);   // Encender LED de éxito.
  }

  // Recibir respuesta binaria desde el programa en C.
  if (sequenceSent && Serial.available() > 0) {
    String binaryResponse = Serial.readStringUntil('\n'); // Leer la respuesta en binario.
    playBinaryResponse(binaryResponse);                   // Reproducir la respuesta en el speaker.

    // Restablecer el estado.
    digitalWrite(ledPinSuccess, LOW); // Apagar LED de éxito.
    sequenceSent = false;
    buttonSequence = "";              // Limpiar la secuencia de botones.
  }
}

// Función para reproducir la secuencia binaria en el speaker.
void playBinaryResponse(String binaryResponse) {
  for (char bit : binaryResponse) {
    if (bit == '1') {
      digitalWrite(speakerPin, HIGH); // Sonido por 1 segundo.
      delay(1000);
      digitalWrite(speakerPin, LOW);
    } else if (bit == '0') {
      delay(2000); // Silencio por 2 segundos.
    }
  }
}
