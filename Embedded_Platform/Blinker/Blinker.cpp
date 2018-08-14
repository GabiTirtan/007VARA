/**
  ******************************************************************************
  * @file    Blinker.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the blinker
  *          functionality.
  ******************************************************************************
 */

#include "Blinker.hpp"

/** \brief  Class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_period       LED toggling reading period
 *  \param[in] f_led          Digital output line to which the LED is connected     
 */
CBlinker::CBlinker(uint32_t f_period, DigitalOut f_led) 
    : CTask(f_period)
    , m_led(f_led) 
{
    m_led = 1;
}

/** \brief  Method called each f_period
 * 
 *  \param[in]  None
 *  \param[out] None
 */
void CBlinker::_run()
{
    m_led = !m_led;
}