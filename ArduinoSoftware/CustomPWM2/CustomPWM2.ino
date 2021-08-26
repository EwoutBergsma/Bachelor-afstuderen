#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#define Valve Timer1

//LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long dataOutTimestamp;
unsigned long valveTimestamp;
unsigned long flowTimestamp;

const int ValvePin = 9;
const int MotorPin = 3;
const int MotorEnablePin = 4;
int MotorPwm = 25;

double oxygenPressure1;
double oxygenPressure2;
int ValvePwm = 1023;

float sendPulseCount = 0;
float pulseCount = 0;
float frequency = 0;
float flow = 0;

bool automatic = false;

void setup(void)
{
  //  lcd.begin();
  //  lcd.backlight();

  Valve.initialize(680);  // 680 us, makes 1.47khz (ish), valve needs somewhere between 1.4kHz-1.5kHz as per datasheet.
  Serial.begin(250000);

  Valve.pwm(ValvePin, ValvePwm);
  Serial.setTimeout(1);

  dataOutTimestamp = millis();
  valveTimestamp = millis();
  flowTimestamp = millis();

  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  attachInterrupt(digitalPinToInterrupt(2), pwm, RISING);

  pinMode(MotorPin, OUTPUT);
  pinMode(MotorEnablePin, OUTPUT);
  digitalWrite(MotorEnablePin, LOW);
  delay(10);
  digitalWrite(MotorEnablePin, HIGH);
  analogWrite(MotorPin, MotorPwm);
}

void loop(void)
{
  handleDataIn();
  handleFlow();
  handleValve();
  handeDataOut();
}

void handleDataIn() {
  String input = "";
  if (Serial.available()) input = Serial.readStringUntil('\n');
  else return;

  //oxygen pressure
  if (input.charAt(0) == 'p') {
    oxygenPressure1 = input.substring(1).toDouble();
    //input = String(oxygenPressure1, 3) + " hPa";
    //while (input.length() < 14) input = " " + input;
    //    lcd.setCursor(0, 1);
    //    lcd.print(input);
  }

  if (input.charAt(0) == 'o') {
    oxygenPressure2 = input.substring(1).toDouble();
    //input = String(oxygenPressure2, 3) + " hPa";
    //while (input.length() < 14) input = " " + input;
    //    lcd.setCursor(0, 1);
    //    lcd.print(input);
  }

  //valve pwm set
  if (input.charAt(0) == 'v') {
    ValvePwm = input.substring(1).toInt();
    if (ValvePwm > 1023) {
      automatic = true;
      Serial.println("Automatic activated");
    }
    else {
      Valve.pwm(ValvePin, ValvePwm);
      automatic = false;
      Serial.println("Valve PWM: " + String(ValvePwm));
    }
  }

  //Motor set
  if (input.charAt(0) == 'm') {
    int _MotorPwm = input.substring(1).toInt();
    setMotorPwm(_MotorPwm);
    Serial.println("Set motor PWM: " + String(MotorPwm));
  }
}

void handeDataOut() {
  if (millis() - dataOutTimestamp > 1000) {
    int flowAnalog = analogRead(A4);
    Serial.println(String(ValvePwm) + ';' + String(MotorPwm) + ';' + String(oxygenPressure1) + ';' + String(oxygenPressure2) + ';' + String(sendPulseCount) + ';' + String(flow) + ';' + String(flowAnalog) + ';' + String(calibrateGasFlow(flowAnalog)) + ';');
    dataOutTimestamp = millis();
  }
}

void handleValve() {
  if (!automatic) return;

  if (millis() - valveTimestamp > 10000) { ////////////////
    ValvePwm = ValvePwm - 10;
    if (ValvePwm < 0) ValvePwm = 0;
    Valve.pwm(ValvePin, ValvePwm);
    valveTimestamp = millis();
  }
}

void handleFlow() {
  if (millis() - flowTimestamp > 3000)
  {
    frequency = pulseCount / 3;
    //flow = 19.875 * frequency + 23.213; //old
    flow = 22.636 * frequency + 43.643;
    if (flow < 60) flow = 0; //We cannot measure flows under 60 ml/min

    //    String flowText = "";
    //    for (int i = String(flow, 1).length(); i < 6; i++) flowText = flowText + " ";
    //    if (flow == 0) flowText = flowText + " <60 ml/min";
    //    else flowText = flowText + String(flow, 1) + "  ml/min";

    //    lcd.setCursor(0, 0);
    //    lcd.print(flowText);

    sendPulseCount = pulseCount;
    pulseCount = 0;
    flowTimestamp = millis();
  }
}

void pwm() {
  pulseCount++;
}

void setMotorPwm(int _MotorPwm) {
  if (_MotorPwm == 0) {
    digitalWrite(MotorEnablePin, LOW);
    MotorPwm = 25;
    analogWrite(MotorPin, MotorPwm);
    return;
  }

  if (_MotorPwm > 229) {
    _MotorPwm = 229;
  }
  if (_MotorPwm < 26) {
    _MotorPwm = 25;
  }

  MotorPwm = _MotorPwm;

  Serial.println(MotorPwm);
  analogWrite(MotorPin, MotorPwm);
  digitalWrite(MotorEnablePin, HIGH);
}

float calibrateGasFlow(int analog) {
  float x = analog;

  if (analog < 205) return 0;
  else if (analog < 395) return 0.521413 * x - 106.3682;
  else if (analog < 543) return 0.6693815 * x - 164.6679;
  else if (analog < 661) return 0.8395632 * x - 256.9064;
  else if (analog < 752) return 1.088664 * x - 421.3131;
  else if (analog < 813) return 1.624073 * x - 823.4051;
  else if (analog < 858) return 2.10784 * x - 1216.223;
  else if (analog < 925) return 1.52413 * x - 714.8171;
  else if (analog < 950) return 3.962739 * x - 2968.091;
  else if (analog < 992) return 2.358773 * x - 1445.928;
  else if (analog < 1024) return 3.669202 * x - 2744.563;
}





