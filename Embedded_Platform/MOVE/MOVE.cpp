/**
  ******************************************************************************
  * @file    MOVE.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the moving
  *          functionality.
  ******************************************************************************
 */

#include "mbed.h"
#include "MOVE.hpp"
//#include <array.h>

/** \brief  MOVE Class constructor
 *
 *  Constructor method
 *
 *  \param[in] _pwm_servo
 *  \param[in] _pwm_driver
 *  \param[in] _ina_driver
 *  \param[in] _inb_driver
 *  \param[in] _current_driver
 */
MOVE::MOVE(
        PinName _pwm_servo,
        PinName _pwm_driver, 
        PinName _ina_driver, 
        PinName _inb_driver, 
        PinName _current_driver)
    :servo(_pwm_servo),
    vnh(_pwm_driver,_ina_driver,_inb_driver,_current_driver)
{
};

/** \brief  MOVE Class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
MOVE::~MOVE()
{
};

/** \brief  Steer method
 *
 *  \param[in] angle
 *  \return    None
 */
void MOVE::Steer(float angle)
{
    if ((angle <= 23) && (angle >= -23))
        servo.SetAngle(angle);
};

/** \brief  Move method
 *
 *  \param[in] speed
 *  \return    None
 */
void MOVE::Speed(float speed)
{
    speed /=100;
    vnh.Run(speed);
};

/** \brief  Brake method
 *
 *  \param[in] None
 *  \return    None
 */
void MOVE::Brake()
{
    vnh.Brake();
}

/** \brief  Hard brake method
 *
 *  \param[in] speed
 *  \return    None
 */
void MOVE::Inverse(float f_speed)
{
    f_speed /=100;
    vnh.Inverse(f_speed);
}

/** \brief  Car testing method
 *
 *  \param[in] None
 *  \return    None
 */
void MOVE::TestCar()
{
    Steer(20);
    wait(1);
    Steer(0);
    wait(1);
    Steer(-20);
    wait(1);
    ResetCar();
    wait(1);
    Speed(25);
    wait(1);
    Speed(0);
    wait(1);
    Speed(-25);
    wait(1);
    ResetCar();
    wait(1);
};

/** \brief  Reset speed and steer method
 *
 *  \param[in] None
 *  \return    None
 */
void MOVE::ResetCar()
{
    Steer(0);
    Speed(0);
};

/** \brief  Get bridge (driver) object reference method
 *
 *  \param[in] None
 *  \return    reference to VNH object
 */
VNH& MOVE::getVNH() 
{ 
    return vnh; 
}