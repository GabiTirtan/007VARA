/**
  ******************************************************************************
  * @file    MotionController.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the motion controller
  *          functionality.
  ******************************************************************************
 */

#include "MotionController.hpp"

/* Define for gamma in velocity to pwm function */
#define GAMMA 0.074255

/** \brief  CMotionController Class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_period              period for controller execution
 *  \param[in] f_serialPort          reference to serial communication object
 *  \param[in] f_car                 reference to MOVE object
 *  \param[in] f_splineInterpreter   reference to spline interpreter object
 *  \param[in] f_period_sec          period for controller execution in seconds
 *  \param[in] f_safetyStop          reference to spline safery stop object
 *  \param[in] f_control             reference to controller object
 */
CMotionController::CMotionController(
        uint32_t f_period,
        Serial& f_serialPort,
        MOVE& f_car,
        CSplineInterpreter& f_splineInterpreter,
        float f_period_sec,
        CSafetyStopFunction& f_safetyStop,
        task::CControl& f_control) : CTask(f_period)
    , m_serialPort(f_serialPort)
    , m_car(f_car)
    , m_speed()
    , m_angle()
    , m_period_sec(f_period_sec)
    , m_isSplineActivated(false)
    , m_ispidActivated(false)
    , m_splineinterpreter(f_splineInterpreter)
    , m_motionPlanner()
    , m_hbTimeOut()
    , m_control(f_control)
    , m_safetyStop(f_safetyStop)
{
}

/** \brief  Serial callback method
  *
  * Serial callback attaching serial callback to controller object
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::staticSerialCallback(void* obj,char const * a, char * b)
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallback(a,b);
}

/** \brief  Serial callback method for BRAKE command
  *
  * Serial callback attaching serial callback to controller object
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::staticSerialCallbackBrake(void* obj,char const * a, char * b)
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackBrake(a,b);
}

/** \brief  Serial callback method for hard BRAKE command
  *
  * Serial callback attaching serial callback to controller object
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::staticSerialCallbackHardBrake(void* obj,char const * a, char * b) 
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackHardBrake(a,b);
}

/** \brief  Serial callback method for PID activation command
  *
  * Serial callback attaching serial callback to controller object
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::staticSerialCallbackPID(void* obj,char const * a, char * b) 
{
    CMotionController* self = static_cast<CMotionController*>(obj);
    self->serialCallbackPID(a,b);
}

/** \brief  Reset method
 * 
 *  \param[in]  None
 *  \return     None
 */
void CMotionController::reset()
{   
    m_speed = 0;
    m_angle = 0;
}

/** \brief  Get speed method
 * 
 *  \param[in]  None
 *  \return     Speed
 */
float CMotionController::getSpeed() 
{
    return m_speed;
}

/** \brief  Get angle method
 * 
 *  \param[in]  None
 *  \return     Angle
 */
float CMotionController::getAngle() 
{
    return m_angle;
}

/** \brief  BrakeCallback method
 * 
 *  \param[in]  None
 *  \return     None
 */
void CMotionController::BrakeCallback(){
    m_state=2;
}

/** \brief  Set state method
 * 
 *  \param[in]  f_state
 *  \return     None
 */
void CMotionController::setState(int f_state){
    m_state = f_state;
}

/** \brief  Method called each f_period
 * 
 *  \param[in]  None
 *  \return     None
 */
void CMotionController::_run()
{
    if(m_splineinterpreter.isNewDataAvailable())
    {
        m_isSplineActivated=true;
        m_state=1;
        CSplineInterpreter::SplineInterpreter_Data spline=m_splineinterpreter.getData();
        m_motionPlanner.setMotionPlannerParameters(spline.isForward,spline.a,spline.b,spline.c,spline.d,spline.duration_sec,m_period_sec);
        // m_serialPort.printf("@SPLN:%d;%4.4f;%4.4f;%4.4f;%4.4f;%4.4f;%4.4f;%4.4f;%4.4f;%4.4f;;\r\n"
        //                         ,spline.isForward
        //                         ,spline.a.real()
        //                         ,spline.a.imag()
        //                         ,spline.b.real()
        //                         ,spline.b.imag()
        //                         ,spline.c.real()
        //                         ,spline.c.imag()
        //                         ,spline.d.real()
        //                         ,spline.d.imag()
        //                         ,spline.duration_sec);
    }

    if(m_isSplineActivated) 
    {
        if(m_motionPlanner.hasValidValue())
        {
            std::pair<float,float> motion=m_motionPlanner.getNextVelocity();  
            float l_dir=m_motionPlanner.getForward()?1:-1;
            if(m_ispidActivated)
            {
                m_speed=motion.first*100.0*l_dir;
                // m_serialPort.printf("@SPLN:%4.4f;%4.4f;;\r\n",m_speed,m_angle);
            }
            else
            {
                m_speed=VEL2PWM(motion.first)*l_dir;
                // m_serialPort.printf("@SPLN:%4.4f;%4.4f;;\r\n",m_speed,m_angle);
            }
            m_angle=motion.second;
            // m_serialPort.printf("@SPLN:%4.4f;%4.4f;;\r\n",m_speed,m_angle);
        }
        else
        {
            m_serialPort.printf("@SPLN:Stop;;\r\n");
            m_speed=0.0;
            m_state=2;
            m_isSplineActivated=false;
        }
    }
    
    //check if safety stop active
    //safety stop function
    if ((m_state!=2)&&(m_safetyStop.isSafetyStopActive(m_speed,m_angle)==true))
    {
        m_state = 2;
    }

    switch(m_state)
    {
        // Moving 
        case 1:
            m_car.Steer(m_angle);
            if(m_ispidActivated)
            {
                // *0.430875709375
                // m_serialPort.printf("MCTL:%4.4f;%4.4f;;\r\n",m_speed,m_angle);
                // m_control.setRef(m_speed*0.3804);//U
                m_control.setRef(m_speed*0.430875709375);
                m_control.control();//calculate -U->Y
                m_car.Speed(m_control.get());//Y
            }
            else
            {
                if(m_speed<20 && m_speed>-20)
                {
                    m_car.Speed(m_speed);
                }
            }
            break;
        // Braking 
        case 2:
            m_car.Steer(m_angle);
            m_car.Brake();
            m_control.clear();
            break;
    }
}

/** \brief  Serial callback actions
  *
  * Serial callback method setting controller to values received
  *
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::serialCallback(char const * a, char * b)
{
    float l_speed;
    float l_angle;
    uint32_t l_res = sscanf(a,"%f;%f",&l_speed,&l_angle);
    if (2 == l_res)
    {
        
        m_speed = l_speed;
        m_angle = l_angle; 
        m_isSplineActivated=false;
        m_state=1;
        sprintf(b,"ack;;");
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Serial callback actions for BRAKE command
  *
  * Serial callback method setting controller to values received
  *
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::serialCallbackBrake(char const * a, char * b)
{
    float l_angle;
    uint32_t l_res = sscanf(a,"%f",&l_angle);
    if(1 == l_res)
    {
        m_speed = 0;
        m_angle = l_angle; 
        m_state = 2;
        m_control.setRef(0);
        sprintf(b,"ack;;");           
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Serial callback actions for hard BRAKE command
  *
  * Serial callback method setting controller to values received
  *
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::serialCallbackHardBrake(char const * a, char * b)
{
    float l_brake,l_angle;
    uint32_t l_res = sscanf(a,"%f;%f",&l_brake,&l_angle);
    if(2 == l_res && m_state!=0)
    {
        m_speed=0;
        m_angle = l_angle; 
        m_car.Inverse(l_brake);
        m_hbTimeOut.attach(callback(this,&CMotionController::BrakeCallback),0.04);
        m_state=0;
        sprintf(b,"ack;;");           
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Serial callback actions for hard PID activation command
  *
  * Serial callback method setting controller to values received
  *
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void CMotionController::serialCallbackPID(char const * a, char * b)
{
    int l_isActivate=0;
    uint32_t l_res = sscanf(a,"%d",&l_isActivate);
    if(l_res==1)
    {
        m_ispidActivated=(l_isActivate>=1);
        sprintf(b,"ack;;");
    }else
    {
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Velocity to PWM conversion
  *
  *
  * \param[in]  vel                   velocity
  * \paramreturn                      PWM value
  * \return None
  */
float CMotionController::VEL2PWM(float vel)
{
    return (vel/GAMMA)*0.41461003105583522+4.9068500777251405;
}
