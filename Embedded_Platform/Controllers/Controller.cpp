/**
  ******************************************************************************
  * @file    Controller.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the controller
  *          functionality.
  ******************************************************************************
 */

#include "Controller.hpp"

/** \brief  Class constructor
  *
  * Constructor method
  *
  * \param[in] f_encoder           reference to encoder object
  * \param[in] f_pid               reference to PID onject
  */
task::CControl::CControl(CEncoderFilter&          f_encoder
                        ,ControllerType<double>&  f_pid)
    :m_encoder(f_encoder)
    ,m_pid(f_pid)
{
}

/** \brief  Set ref
  *
  * Set controller reference method
  *
  * \param[in] f_RefRps            controller reference value
  * \return None
  */
void task::CControl::setRef(double f_RefRps)
{
    m_RefRps=f_RefRps;
}

/** \brief  Get ref
  *
  * Get controller reference method
  *
  * \param[in] None
  * \return                         controller reference value  
  */
double task::CControl::getRef()
{
    return m_RefRps;
}

/** \brief  Get control value
  *
  * Get control value
  *
  * \param[in] None
  * \return                         control value
  */
double task::CControl::get()
{
    return m_u;
}

/** \brief  Get error
  *
  * Get error value
  *
  * \param[in] None
  * \return                         error value  
  */
double task::CControl::getError()
{
    return m_error;
}

/** \brief  Clear PID params
  *
  * Clear PID params
  *
  * \param[in] None
  * \return     None  
  */
void task::CControl::clear()
{
    m_pid.clear();
}

/** \brief  Performs control action
  *
  * Control action
  *
  * \param[in] None
  * \return     None  
  */
double task::CControl::control()
{
    double l_MesRps=m_encoder.getRpsFiltered();
    float  l_ref=std::abs(m_RefRps);
    double l_error=l_ref-l_MesRps;
    m_u=converter(m_pid(l_error));
    m_error=l_error;
    if(m_RefRps<0)
    {
        m_u=m_u*-1.0;
    }
    return m_u;
}

/** \brief  PWM converter
  *
  * PWM converter
  *
  * \param[in] f_u                 PWM value
  * \return                         converted (scaled) PWM value   
  */
double task::CControl::converter(double f_u)
{
    double l_pwm=f_u;
    // (f_u-(-9.73795803))/2.14326143;
    //Modified  need
    if(l_pwm<-10)
    {
        return -10;
    }
    else if(l_pwm>20.0)
    {
        return 20.0;
    }
    return l_pwm;
}