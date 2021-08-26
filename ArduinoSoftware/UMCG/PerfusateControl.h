#ifndef PERFUSATECONTROL_H
#define PERFUSATECONTROL_H

#include <Arduino.h>
#include "PID.h"
#include "GasFlowControl.h"

class Perfusate {
  private:
    GasFlow gasFlow;
    unsigned long gasFlowTime;
    
    double Setpoint, Input, Output;
    //double Kp=75.225, Ki=9.403, Kd=0; //Venous concentration 
    //double Kp=0.0278, Ki=0.00108, Kd=0; //Arterial concentration
    //double Kp=878.613, Ki=109.827, Kd=0; //Venous flow
    //double Kp=0.320, Ki=0.0116, Kd=0; //Arterial flow
    //double Kp = 3, Ki=0.1, Kd=0; first thesis validation
    //double Kp = 5.75, Ki = 1.045, Kd = 1.04;
    //double Kp = 2.375, Ki = .5225, Kd = 0.52;
    double Kp = 2.375, Ki = .5225, Kd = 0;
    
    PID perfusatePID; //&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT
    
  public:
    Perfusate();
    void Init(GasFlow*);
    double Handler(double);
    void SetSetpoint(int);
    String GetOutput();

    int GetSetpoint();
};

#endif
