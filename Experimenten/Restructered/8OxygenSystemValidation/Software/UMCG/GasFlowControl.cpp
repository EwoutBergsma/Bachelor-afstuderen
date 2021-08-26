#include "GasFlowControl.h"

GasFlow::GasFlow() {
  gasPID = PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  gasPID.SetOutputLimits(450, 1023);
  Setpoint = 0;
  gasPID.SetMode(AUTOMATIC);
}

void GasFlow::Init(){
  Valve.initialize(680);  // 680 us, makes 1.47khz (ish), valve needs somewhere between 1.4kHz-1.5kHz as per datasheet.
}

double GasFlow::Handler() {
  Input = FromAnalogToFlow(analogRead(PIN_INPUT));
  gasPID.Compute();
  Valve.pwm(PIN_OUTPUT, (int)Output);

  return Input;
}

void GasFlow::SetSetpoint(double _Setpoint) {
  if (_Setpoint > 1023) return;
  if (_Setpoint < 0) return;
  //Setpoint = FromFlowToAnalog(_Setpoint);
  Setpoint = _Setpoint;
}

String GasFlow::GetFlow() {
  return String(FromAnalogToFlow(analogRead(PIN_INPUT)));
}

String GasFlow::GetOutput() {
  return String(Output);
}

int GasFlow::GetSetpoint(){
  return Setpoint;
}


double GasFlow::FromAnalogToFlow(double analog) {
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

int GasFlow::FromFlowToAnalog(double flow) {
  double x = flow;

  if (flow < 99.1) return 1.917866 * x + 204;
  else if (flow < 198.2) return 1.493916 * x + 246;
  else if (flow < 297.2) return 1.191095 * x + 306;
  else if (flow < 396.3) return 0.9185567*x + 387;
  else if (flow < 495.4) return 0.6157358*x + 507;
  else if (flow < 594.4) return 0.4744194*x + 577;
  else if (flow < 693.5) return 0.6561119*x + 469;
  else if (flow < 792.6) return 0.2523507*x + 749;
  else if (flow < 891.6) return 0.4239492*x + 613;
  else if (flow < 990.7) return 0.2725388*x + 748;
}

