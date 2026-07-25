// Entrada del sensor infrarrojo
const int sensorIR = 7;

// Salidas

const int pinCapacitor  = 9;
const int pinIC         = 8;
const int pinResistor   = 11;
const int pinConector = 10;
//=====================================

bool estadoAnterior = LOW;
bool ifInf = LOW;

//=====================================

void setup()
{
    Serial.begin(115200);

    pinMode(sensorIR, INPUT);

    pinMode(pinCapacitor, OUTPUT);
    pinMode(pinIC, OUTPUT);
    pinMode(pinResistor, OUTPUT);
    pinMode(pinConector, OUTPUT);

    apagarTodo();
}

//=====================================

void loop()
{
    bool estadoActual = digitalRead(sensorIR);

    // Detectar flanco de subida del sensor
    if (estadoActual == HIGH && estadoAnterior == LOW)
    {
        // Avisar a Python que tome una fotografía
        ifInf = HIGH;
        Serial.println("IR");
    }

    estadoAnterior = estadoActual;

    // Esperar respuesta desde Python
    if (Serial.available() > 0 && ifInf == HIGH)
    {
        int clase = Serial.parseInt();

        apagarTodo();

        switch (clase)
        {
            case 1:
                digitalWrite(pinCapacitor, HIGH);
                Serial.println("Capacitor");
                break;

            case 2:
                digitalWrite(pinIC, HIGH);
                Serial.println("IC");
                break;

            case 3:
                digitalWrite(pinResistor, HIGH);
                Serial.println("Resistor");
                break;

            case 4:
                digitalWrite(pinConector, HIGH);
                Serial.println("Transistor");
                break;

            case 5:
                Serial.println("No se detectó nada");
                break;

            default:
                Serial.println("Dato invalido");  
                break;
        }

        Serial.print("Recibido: ");
        Serial.println(clase);

        ifInf = 0;
    }
}

//=====================================

void apagarTodo()
{
    digitalWrite(pinCapacitor, LOW);
    digitalWrite(pinIC, LOW);
    digitalWrite(pinResistor, LOW);
    digitalWrite(pinConector, LOW);
}