/**
  ******************************************************************************
  * @file    main.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   Application entry point.
  ******************************************************************************
 */

/* The mbed library */
#include <mbed.h>
/* Header that defines the map and multimap container classes */
#include <map>
/* Header that defines the fixed-size array container class */
#include <array>
/* header introduces string types, character traits and a set of converting functions */
#include <string>
/* Header providing string stream classes */
#include <sstream>

/* Header file for the queue functionality */
#include "Queue.hpp"
/* Timer functionality */
#include "Timer.hpp"
/* Interpreter for serial commands */
#include "CommandInterpreter.hpp"
/* Task manager */
#include"TaskManager.hpp"
/* Header file for the MOVE functionality */
#include "MOVE.hpp"
/* Header file for the blinker functionality */
#include "Blinker.hpp"
/* Header file for the serial echoer functionality */
#include "Echoer.hpp"
/* Header file for the serial communication functionality */
#include "SerialMonitor.hpp"
/* Header file for the motion controller functionality */
#include "MotionController.hpp"
/* Header file for the spline interpreter functionality */
#include "SplineInterpreter.hpp"
/* Header file for the encoder filter functionality */
#include "EncoderFilter.hpp"
/* Header file for the sensor task functionality */
#include "SensorTask.hpp"
/* Header file  for the controller functionality */
#include "Controller.hpp"
/* Header file  for the sendor publisher functionality */
#include "SensorPublisher.hpp"
/* Safety stop functionality */
#include "SafetyStopFunction.hpp"
/* Distrance sensors read methods */
#include "DistanceSensors.hpp"
/* SHARP IR sensor */
#include "IR_DistanceSensorDriver.hpp"


Serial          g_rpi(USBTX, USBRX); 
MOVE            g_car(D9, D3, D2, D4, A0);
const float     g_ir_sensor_reading_period = 0.2; //seconds
const float     g_safetyStop_period = 0.25; //seconds
const uint8_t   g_front_ir_sensor_number = 5; 
CTimer_100us    timer_100us;
const float     g_baseTick = 0.0001; // seconds
CBlinker        g_blinker       (0.5    / g_baseTick, LED1);
CEchoer         g_echoer        (10     / g_baseTick, g_rpi);
float           g_period_Encoder=0.025;

filter::lti::siso::CIIRFilter<float,1,2> g_encoderFilter(linalg::CRowVector<float,1>({-0.72972973}) //A
                                                        ,linalg::CRowVector<float,2>({ 0.13513514  , 0.13513514  })); //B


CEncoderFilter  g_opticalEncoder(g_period_Encoder   / g_baseTick, g_period_Encoder, 20, D10,g_encoderFilter);                                                    
controller::siso::CPidController<double> g_pid(0.32465*3.0,1.0221*3.0,0.014343*3.0,0.0023102,g_period_Encoder);

CSplineInterpreter      g_splineInterpreter;

C_IR_DistanceSensorDriver g_ir_sensor1(A1, D12);// left front sensor
C_IR_DistanceSensorDriver g_ir_sensor2(A3, D8); // mid left front sensor
C_IR_DistanceSensorDriver g_ir_sensor3(A4, D5); // center frint sensor
C_IR_DistanceSensorDriver g_ir_sensor4(A5, D6); // mid right front sensor
C_IR_DistanceSensorDriver g_ir_sensor5(D11, D7); // right front

std::array<C_IR_DistanceSensorDriver*, 5> g_front_sersor_array = {&g_ir_sensor1,&g_ir_sensor2, &g_ir_sensor3, &g_ir_sensor4, &g_ir_sensor5};
CDistanceSensors<std::array<C_IR_DistanceSensorDriver*, 5>> g_frontSensors(g_ir_sensor_reading_period/g_baseTick, g_baseTick, g_front_sersor_array);
CSafetyStopFunction g_safetyStop(g_ir_sensor1, g_ir_sensor2, g_ir_sensor3, g_ir_sensor4, g_ir_sensor5);


std::array<C_IR_DistanceSensorDriver*,5> g_sensors={&g_ir_sensor1,&g_ir_sensor2,&g_ir_sensor3,&g_ir_sensor4,&g_ir_sensor5};
sensors::ProximityPublisher<C_IR_DistanceSensorDriver,5>                           g_sensorPub(0.05/g_baseTick,g_sensors,g_rpi);

task::CControl              g_controller(g_opticalEncoder,g_pid);
CMotionController           g_motionController(g_period_Encoder / g_baseTick, g_rpi, g_car,g_splineInterpreter,g_period_Encoder, g_safetyStop,g_controller);
examples::CEncoderSender    g_encoderSender(0.1 / g_baseTick,g_opticalEncoder,g_controller,g_rpi);

CSerialMonitor::CSerialSubscriberMap g_serialMonitorSubscribers = {
    {"MCTL",mbed::callback(CMotionController::staticSerialCallback,&g_motionController)},
    {"BRAK",mbed::callback(CMotionController::staticSerialCallbackBrake,&g_motionController)},
    {"HBRA",mbed::callback(CMotionController::staticSerialCallbackHardBrake,&g_motionController)},
    {"PIDA",mbed::callback(CMotionController::staticSerialCallbackPID,&g_motionController)},
    {"SPLN",mbed::callback(CSplineInterpreter::staticSerialCallback,&g_splineInterpreter)},
    {"SFBR",mbed::callback(CSafetyStopFunction::staticSerialCallback,&g_safetyStop)},
    {"PIDS",mbed::callback(controller::siso::CPidController<double>::staticSerialCallback,&g_pid)},
    {"DSPB",mbed::callback(sensors::ProximityPublisher<C_IR_DistanceSensorDriver,5>::staticSerialCallback,&g_sensorPub)},
    {"ENPB",mbed::callback(examples::CEncoderSender::staticSerialCallback,&g_encoderSender)}
};

CSerialMonitor g_serialMonitor(g_rpi, g_serialMonitorSubscribers);


CTask* g_taskList[] = {
    &g_blinker,
    &g_opticalEncoder,
    &g_motionController,
    &g_serialMonitor,
    &g_encoderSender,
    &g_sensorPub,
    &g_frontSensors
    // &g_echoer
}; 

CTaskManager g_taskManager(g_taskList, sizeof(g_taskList)/sizeof(CTask*), g_baseTick);

uint32_t setup()
{
    g_rpi.baud(460800);  
    g_rpi.printf("\r\n\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#   I'm alive   #\r\n");
    g_rpi.printf("#               #\r\n");
    g_rpi.printf("#################\r\n");
    g_rpi.printf("\r\n");

    timer_100us.start();  
    return 0;    
}

uint32_t loop()
{
    g_taskManager.mainCallback();
    return 0;
}

int main() 
{
    uint32_t  l_errorLevel = setup(); 
    while(!l_errorLevel) 
    {
        l_errorLevel = loop();
    }
    g_rpi.printf("exiting with code: %d",l_errorLevel);
    return l_errorLevel;
}