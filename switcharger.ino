#define OUT_CHARGE_ENABLE 13
#define SENSOR_VOLTAGE_PANEL A0
#define SENSOR_VOLTAGE_BATTERY A1

const int ADC_RESOLUTION = 1024;
const float VCC_ARDUINO = 5.00; // Volts
const float VBAT_MAX = 14.4; // Volts
const float VBAT_FLOAT = 13.8; // Volts
const float VBAT_FLOAT_LOW = 12.8; // Volts

int state;
float Vin, Vbat;

float readVoltage(int pin, int R1, int R2) {
  /*
  *
  *  V
  * o--------+
  *          |
  *         [ ] R1
  *         [ ]
  *          |
  *          +----o Vprobe
  *          |
  *         [ ] R2
  *         [ ]
  *          |
  *         ---
  *     GND  -
  */
  return (R1 + R2) / R2 * VCC_ARDUINO * analogRead(pin) / ADC_RESOLUTION;
}

void ActivateCharging(bool bEnable = true) {
  // 
  if (bEnable) {
    // 
    digitalWrite(OUT_CHARGE_ENABLE, HIGH);
  } else {
    // 
    digitalWrite(OUT_CHARGE_ENABLE, LOW);
  }
}

void setup() {
  // 
  for (int i = 0; i <= 13;) pinMode(i++, OUTPUT);
  pinMode(SENSOR_VOLTAGE_PANEL, INPUT);
  pinMode(SENSOR_VOLTAGE_BATTERY, INPUT);
  ActivateCharging(false);
  state = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  Vin = readVoltage(SENSOR_VOLTAGE_PANEL, 10, 1);
  Vbat = readVoltage(SENSOR_VOLTAGE_BATTERY, 100, 51);
  if (Vin <= Vbat) {
    // 
    ActivateCharging(false);
    state = 0;
    return;
  }
  if (state == 0) {
    // bulk charging
    if (Vbat >= VBAT_MAX) {
      // 
      ActivateCharging(false);
      state = 1;
    } else {
      // 
      ActivateCharging();
    }
  }
  if (state == 1) {
    // float charging
    if (Vbat >= VBAT_FLOAT) {
      // 
      ActivateCharging(false);
    } else if (Vbat < VBAT_FLOAT_LOW) {
      //
      ActivateCharging();
    }
  }
}
