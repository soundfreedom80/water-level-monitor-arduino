#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//-------------------------------------------------
// Configuración
//-------------------------------------------------
const byte sensorPin = A0;

// Filtro exponencial (alpha = 0.10 para más suavidad, pero puedes ajustar)
const float FILTER_ALPHA = 0.10;

// Intervalos en milisegundos
const unsigned long INTERVALO_SERIAL = 200;   // Actualizar Serial cada 200 ms
const unsigned long INTERVALO_PANTALLA = 100; // Actualizar OLED cada 100 ms

//-------------------------------------------------
// Tabla de calibración (optimizada: static const)
//-------------------------------------------------
static const int adcCal[] = {
  65,   // 0 mm
  310,  // 10 mm
  340,  //20 mm
  350,  //30 mm
  400,  //40 mm
};

static const int nivelCal[] = {
   0,
   25,
   50,
   75,
   100,
};

const int PUNTOS_CAL = sizeof(adcCal) / sizeof(adcCal[0]);

//-------------------------------------------------
// Variables globales
//-------------------------------------------------
float sensorFiltrado = 40.0;
int   sensorValue    = 0;
int   porcentaje     = 0;

// Variables para control de tiempo con millis()
unsigned long tiempoAnteriorSerial = 0;
unsigned long tiempoAnteriorPantalla = 0;

// Constantes de dibujo (fuera del loop)
const int tankX = 8;
const int tankY = 14;
const int tankW = 24;
const int tankH = 46;

//-------------------------------------------------
// Conversión ADC -> Porcentaje (optimizada)
//-------------------------------------------------
int calcularNivel(int adc)
{
    if (adc <= adcCal[0])
        return 0;

    if (adc >= adcCal[PUNTOS_CAL - 1])
        return 100;

    for (int i = 0; i < PUNTOS_CAL - 1; i++)
    {
        if (adc >= adcCal[i] && adc <= adcCal[i + 1])
        {
            if (adcCal[i] == adcCal[i + 1])
                return nivelCal[i + 1];

            return map(adc,
                       adcCal[i],
                       adcCal[i + 1],
                       nivelCal[i],
                       nivelCal[i + 1]);
        }
    }

    return 0;
}

//-------------------------------------------------
void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();
}

//-------------------------------------------------
void loop()
{
  //---------------------------------------------
  // Lectura del sensor (se ejecuta siempre)
  //---------------------------------------------
  int lectura = analogRead(sensorPin);

  // Filtro exponencial (más suave que 0.20)
  sensorFiltrado = (1.0 - FILTER_ALPHA) * sensorFiltrado + FILTER_ALPHA * lectura;
  sensorValue = (int)sensorFiltrado;

  //---------------------------------------------
  // Conversión usando calibración (siempre)
  //---------------------------------------------
  porcentaje = calcularNivel(sensorValue);
  porcentaje = constrain(porcentaje, 0, 100);

  //---------------------------------------------
  // Monitor Serie (cada INTERVALO_SERIAL ms)
  //---------------------------------------------
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoAnteriorSerial >= INTERVALO_SERIAL)
  {
    tiempoAnteriorSerial = tiempoActual;
    Serial.print(F("ADC: "));
    Serial.print(sensorValue);
    Serial.print(F("   Nivel: "));
    Serial.print(porcentaje);
    Serial.println(F("%"));
  }

  //---------------------------------------------
  // Pantalla OLED (cada INTERVALO_PANTALLA ms)
  //---------------------------------------------
  if (tiempoActual - tiempoAnteriorPantalla >= INTERVALO_PANTALLA)
  {
    tiempoAnteriorPantalla = tiempoActual;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Título
    display.setTextSize(1);
    display.setCursor(18, 0);
    display.print(F("NIVEL DE AGUA"));

    // Tanque
    display.drawRect(tankX, tankY, tankW, tankH, SSD1306_WHITE);

    // Cuello del tanque
    display.drawRect(tankX + 7, tankY - 3, 10, 3, SSD1306_WHITE);

    int nivel = map(porcentaje, 0, 100, 0, tankH - 2);

    display.fillRect(
      tankX + 1,
      tankY + tankH - 1 - nivel,
      tankW - 2,
      nivel,
      SSD1306_WHITE);

    // Porcentaje
    display.setTextSize(2);
    display.setCursor(45, 16);

    if (porcentaje < 10)
        display.print(" ");

    display.print(porcentaje);
    display.print("%");

    // Estado
    display.setTextSize(1);
    display.setCursor(45, 42);

    if (porcentaje < 10)
        display.print(F("VACIO"));
    else if (porcentaje < 35)
        display.print(F("BAJO"));
    else if (porcentaje < 70)
        display.print(F("NORMAL"));
    else if (porcentaje < 95)
        display.print(F("ALTO"));
    else
        display.print(F("LLENO"));

    // ADC
    display.setCursor(45, 55);
    display.print(F("ADC:"));
    display.print(sensorValue);

    display.display();
  }

  // NOTA: No hay delay, el loop se ejecuta continuamente.
  // El sensor se lee en cada iteración (muy rápido).
}