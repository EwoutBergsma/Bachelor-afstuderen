#include "PerfusateControl.h"
#include "GasFlowControl.h"

#include "TimerOne.h"

unsigned long PIDTime;
unsigned long handleDataOutTime;
unsigned long liquidFlowTime;

double oxygenPressure1;
double oxygenPressure2;

//liquid flow
float liquidPulseCount = 0;
float liquidFrequency = 0;
float liquidFlow = 0;

//Perfusate handling
GasFlow gasFlow; //PID controller for the gas flow
Perfusate perfusate; //PID controller for the perfusate oxygen

void setup() {
  //Serial communication with PC
  Serial.begin(9600);
  Serial.setTimeout(10); //Else the Arduino is going to wait too long for potential communication

  //Liquid flow sensor
  attachInterrupt(digitalPinToInterrupt(2), pwm, RISING);
  liquidFlowTime = millis();

  //Timestamp for the data out
  PIDTime = millis();
  handleDataOutTime = millis();

  Timer1.initialize(680);  // 680 us, makes 1.47khz (ish), valve needs somewhere between 1.4kHz-1.5kHz as per datasheet.

  gasFlow.Init();
  perfusate.Init(&gasFlow);
  perfusate.SetSetpoint(60);
}

void loop() {
  //PID handlers
  if (millis() - PIDTime > 100) {
    PIDTime = millis();
    
    double currentGasFlow = gasFlow.Handler();
    double newGasFlowSetpoint = perfusate.Handler(oxygenPressure1);
    
    //double newGasConcentration = perfusate.Handler(oxygenPressure2);
    //double newGasFlowSetpoint = -(10000*newGasConcentration)/(20*newGasConcentration-19);
    //Serial.println(String(newGasConcentration) + " " + String(newGasFlowSetpoint));
    gasFlow.SetSetpoint(newGasFlowSetpoint);
  }

  //Send data to computer
  if (millis() - handleDataOutTime > 1000) {
    handleDataOutTime = millis();
    Serial.println(String(gasFlow.GetSetpoint()) + ";" + String(gasFlow.GetFlow()) + ";" + String(gasFlow.GetOutput()) + ";" + String(oxygenPressure1) + ";" + String(oxygenPressure2) + ";" + String(perfusate.GetSetpoint()) +  ";"  + String(liquidFlow) + ";" );
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
    int perfusateSetpoint;
    perfusateSetpoint = input.substring(1).toInt();
    perfusate.SetSetpoint(perfusateSetpoint);
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
