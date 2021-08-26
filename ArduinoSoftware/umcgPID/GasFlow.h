#ifndef GASFLOW_H
#define GASFLOW_H

#include <Arduino.h>
#include "PID.h"
#include "TimerOne.h"

#define PIN_INPUT A4
#define PIN_OUTPUT 9
#define Valve Timer1

class GasFlow {
  private:
//    int PIN_OUTPUT = 9;
    double Setpoint, Input, Output;
    double Kp= 0.13, Ki=0.19, Kd=0; //VALIDATION
    // double Kp= 0.0898, Ki=0.1829, Kd=0; //new
    //double Kp=0.09099, Ki=0.4942, Kd=0.00029; //Deze
    //double Kp= 0.1818, Ki=1, Kd=0.00058;
//    double Kp = 0.001, Ki = 0.001, Kd = 0.001;
    PID gasPID; //&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT
    
  public:
    GasFlow();
    void Init();
    void Handler();
    void SetSetpoint(int);
    String GetFlow();
    String GetOutput();

    int GetSetpoint();

    double FromAnalogToFlow(double);
    int FromFlowToAnalog(double);
};

#endif
