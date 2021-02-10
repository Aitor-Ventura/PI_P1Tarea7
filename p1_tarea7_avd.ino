/* Visualizacion de los digitos de la tarea 6 creando una
 *  subrutina de servicio de interrupcion de forma sistematica
 *  cada 10 ms.
 */
byte Uno     = 0x06;
byte Dos     = 0x5B;
byte Tres    = 0x4F;
byte Cuatro  = 0x66;
byte Cinco   = 0x6D;
byte Seis    = 0x7D;
byte Siete   = 0x07;
byte Ocho    = 0x7F;
byte Nueve   = 0x67;
byte Cero    = 0x3F;
byte Unidades = 0;
byte Decenas = 0;
unsigned long time;
int interr;

void setup() {
  int interr = 0;
  time = millis();
  DDRA = B11111111;
  PORTA = 0x3F;
  cli();
  // Inicializamos los leds
  pinMode(49, OUTPUT);
  pinMode(48, OUTPUT);
  // Inicializamos con pullup
  pinMode(21, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  // Estado inicial de apagado
  digitalWrite(49, LOW);
  digitalWrite(48, LOW);
  // Interrupcion cuando pulsamos algun pulsador
  attachInterrupt(digitalPinToInterrupt(21), pup, LOW);
  attachInterrupt(digitalPinToInterrupt(20), pdown, LOW);
  attachInterrupt(digitalPinToInterrupt(19), pcenter, LOW);
  interrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  OCR1A = 100;
  TIMSK1 |= (1 << OCIE1A);
  sei();
  // Monitor serie
  Serial.begin(9600);
  DDRA = B11111111;
  PORTC = 0xFF;
}

ISR(TIMER1_COMPA_vect) {
  if (interr == 0) {
    digit(Unidades);
    digitalWrite(49, LOW);
    delay(10);
    digitalWrite(49, HIGH);
    interr++;
  } else {
    interr = 0;
    digit(Decenas);
    digitalWrite(48, LOW);
    delay(10);
    digitalWrite(48, HIGH);
  }
}
void digit(byte dig) {
  switch (dig) {
    case 0: {
        PORTA = Cero;
        break;
      }
    case 1: {
        PORTA = Uno;
        break;
      }
    case 2: {
        PORTA = Dos;
        break;
      }
    case 3: {
        PORTA = Tres;
        break;
      }
    case 4: {
        PORTA = Cuatro;
        break;
      }
    case 5: {
        PORTA = Cinco;
        break;
      }
    case 6: {
        PORTA = Seis;
        break;
      }
    case 7: {
        PORTA = Siete;
        break;
      }
    case 8: {
        PORTA = Ocho;
        break;
      }
    case 9: {
        PORTA = Nueve;
        break;
      }
   }
}

// Pone contador a 0
int pcenter() {
  if (time + 200 < millis()) {
    Serial.write("puto");
    time = millis();
    Unidades = 0;
    Decenas = 0;
  }
}

// Incrementa los digitos en uno
int pup() {
  if (time + 200 < millis()) {
    // Escribir las unidades
    Serial.write("%d",Unidades);
    time = millis();
    if (Unidades == 9) {
      Unidades = 0;
      if (Decenas == 9) {
        Decenas = 0;
      } else {
        Decenas++;
      }
    } else {
      Unidades++;
    }
  }
}

// Decrementar los digitos en uno
int pdown() {
  if (time + 200 < millis()) {
    // Escribir las unidades
    Serial.write("%d", Unidades);
    time = millis();
    if (Unidades == 0) {
      Unidades = 9;
      if (Decenas == 0) {
        Decenas = 9;
      } else {
        Decenas--;
      }
    } else {
      Unidades--;
    }
  }
}

// Solo utiliza interrupciones, luego no tenemos necesidad de loop
void loop() {
}
