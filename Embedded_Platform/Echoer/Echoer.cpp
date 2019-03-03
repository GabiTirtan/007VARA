/**
  ******************************************************************************
  * @file    Echoer.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the serial echoer
  *          functionality.
  ******************************************************************************
 */

#include "Echoer.hpp"

/** \brief  Class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_period       echoer execution period
 *  \param[in] f_serialPort   Serial communication object
 */
CEchoer::CEchoer(uint32_t f_period, Serial& f_serialPort) 
    : CTask(f_period)
    , m_serialPort(f_serialPort)
{
}

/** \brief  Method called each f_period
 * 
 *  \param[in]  None
 *  \param[out] None
 */
void CEchoer::_run()
{
    m_serialPort.printf(".\n\r");
}   