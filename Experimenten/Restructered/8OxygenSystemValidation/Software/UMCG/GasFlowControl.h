#ifndef GASFLOWCONTROL_H
#define GASFLOWCONTROL_H

#include <Arduino.h>
#include "PID.h"
#include "TimerOne.h"

#define PIN_INPUT A4
#define PIN_OUTPUT 9
#define Valve Timer1

class GasFlow {
  private:
    double Setpoint, Input, Output;
    //double Kp=0.09099, Ki=0.4942, Kd=0.00029; //old
    double Kp= 0.0898, Ki=0.1829, Kd=0; //new

    PID gasPID; //&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT
    
  public:
    GasFlow();
    void Init();
    double Handler();
    void SetSetpoint(double);
    String GetFlow();
    String GetOutput();

    int GetSetpoint();

    double FromAnalogToFlow(double);
    int FromFlowToAnalog(double);
};

#endif
