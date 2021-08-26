#include "GasFlow.h"

unsigned long gasFlowTime;
unsigned long handleDataOutTime;
unsigned long liquidFlowTime;

double oxygenPressure1;
double oxygenPressure2;

//liquid flow
float liquidPulseCount = 0;
float liquidFrequency = 0;
float liquidFlow = 0;

GasFlow gasFlow;

void setup() {
  Serial.begin(250000);
  Serial.setTimeout(1);
  gasFlow.Init();

  attachInterrupt(digitalPinToInterrupt(2), pwm, RISING);

  gasFlowTime = millis();
  handleDataOutTime = millis();
  liquidFlowTime = millis();
}

void loop() {
  //Gas PID
  if (millis() - gasFlowTime > 10) {
    gasFlowTime = millis();
    gasFlow.Handler();
  }

  //Send data to computer
  if (millis() - handleDataOutTime > 1000) {
    handleDataOutTime = millis();
    Serial.println(String(gasFlow.GetSetpoint()) + ";" + String(gasFlow.GetFlow()) + ";" + String(gasFlow.GetOutput()) + ";" + String(oxygenPressure1) + ";" + String(oxygenPressure2) + ";"  + String(liquidFlow) + ";"+ String(99) + ";");
  }

  //Check for commands from pc
  handleDataIn();

  //perfusate flow
  handleLiquidFlow();
}

void handleDataIn() {
  String input = "";
  if (Serial.available()) input = Serial.readStringUntil('\n');
  else return;

  //oxygen pressure
  if (input.charAt(0) == 'f') {
    int gasSetpoint;
    gasSetpoint = input.substring(1).toInt();
    gasFlow.SetSetpoint(gasSetpoint);
  }

  //oxygen pressure
  if (input.charAt(0) == 'p') {
    oxygenPressure1 = input.substring(1).toDouble();
  }

  if (input.charAt(0) == 'o') {
    oxygenPressure2 = input.substring(1).toDouble();
  }
}


void handleLiquidFlow() {
  if (millis() - liquidFlowTime > 3000)
  {
    float frequency = liquidPulseCount / 3;
    
    liquidFlow = 22.636 * frequency + 43.643;
    if (liquidFlow < 60) liquidFlow = 0; //We cannot measure flows under 60 ml/min

    liquidPulseCount = 0;
    liquidFlowTime = millis();
  }
}


void pwm() {
  liquidPulseCount++;
}
