#include "PerfusateControl.h"

Perfusate::Perfusate() {
  perfusatePID = PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  perfusatePID.SetOutputLimits(0, 800);
  Setpoint = 0;
  perfusatePID.SetMode(AUTOMATIC);
}

void Perfusate::Init(GasFlow* _gasFlow){

}

double Perfusate::Handler(double _oxygenPressure) {
  Input = _oxygenPressure;
  perfusatePID.Compute();
  return Output;
}

void Perfusate::SetSetpoint(int _Setpoint) {
  if (_Setpoint > 1023) return;
  if (_Setpoint < 0) return;
  //Setpoint = FromFlowToAnalog(_Setpoint);
  Setpoint = _Setpoint;
}


String Perfusate::GetOutput() {
  return String(Output);
}

int Perfusate::GetSetpoint(){
  return Setpoint;
}

