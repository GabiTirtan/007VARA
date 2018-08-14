/**
  ******************************************************************************
  * @file    MotionController.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the motion controller
  *          functionality.
  ******************************************************************************
 */

#ifndef MOTION_CONTROLLER_HPP
#define MOTION_CONTROLLER_HPP

/* The mbed library */
#include <mbed.h>
#include "TaskManager.hpp"
#include "MOVE.hpp"
#include "SplineInterpreter.hpp"
#include "BezierMotionPlanner.hpp"
#include "Controller.hpp"
#include "SafetyStopFunction.hpp"

//!  CMotionController class. 
/*!
 *  It inherits class CTask. 
 *  It is used for executing move commands.
 * */
class CMotionController : public CTask
{
public:
    CMotionController(
        uint32_t f_period, 
        Serial& f_serialPort, 
        MOVE& f_car,
        CSplineInterpreter& f_splineInterpreter,
        float f_period_sec, 
        CSafetyStopFunction& f_safetyStop,
        task::CControl& f_control);

    /* Serial callback method */
    static void staticSerialCallback(void* obj,char const * a, char * b);
    /* Serial callback method for BRAKE command */
    static void staticSerialCallbackBrake(void* obj,char const * a, char * b);
    /* Serial callback method for hard BRAKE command */
    static void staticSerialCallbackHardBrake(void* obj,char const * a, char * b) ;
    /* Serial callback method for PID activation command */
    static void staticSerialCallbackPID(void* obj,char const * a, char * b);
    /* Reset method */
    void reset();
    /* Get speed method */
    float getSpeed();
    /* Get angle method */
    float getAngle();
    /* BRAKE callback method */
    void BrakeCallback();
    /* Set state method */
    void setState(int f_state);
private:
    /* Run method */
    virtual void _run();
    /* Serial callback implementation */ 
    void serialCallback(char const * a, char * b);
    /* BRAKE serial callback */
    void serialCallbackBrake(char const * a, char * b);
    /* Hard BRAKE serial callback */
    void serialCallbackHardBrake(char const * a, char * b);
    /* PID serial callback */
    void serialCallbackPID(char const * a, char * b);
    /* Velocity to PWM function */
    static float VEL2PWM(float vel);
    /* reference to Serial object */
    Serial& m_serialPort;
    /* reference to MOVE object */
    MOVE& m_car;
    /* Speed */
    float m_speed;
    /* Angle */
    float m_angle;
    /* PEriod i nseconds */
    float   m_period_sec;
    /* Spline activation state */
    bool    m_isSplineActivated;
    /* State machine state */
    uint8_t m_state;
    /* PID activation state */
    bool    m_ispidActivated;
    // 0-none
    // 1-normal
    // 2-brake regeneration
    /* Reference to spline interpreter */
    CSplineInterpreter&     m_splineinterpreter;
    /* motion planner object */
    CBezierMotionPlanner    m_motionPlanner;
    /* Timeout */
    Timeout                 m_hbTimeOut;
    /* Reference to control object */
    task::CControl&         m_control;
    /* Reference to safety stop function */
    CSafetyStopFunction&    m_safetyStop;
};

#endif // MOTION_CONTROLLER_H 
