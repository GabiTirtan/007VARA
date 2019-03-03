/**
  ******************************************************************************
  * @file    TaskManager.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for task manager.
  ******************************************************************************
 */

#include "TaskManager.hpp"

/******************************************************************************/
/** \brief  CTask class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_period      execution period
 */
CTask::CTask(uint32_t f_period) 
    : m_period(f_period)
    , m_ticks(0)
    , m_triggered(false) 
{
}

/** \brief  CTask class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
CTask::~CTask() 
{
}

/** \brief  Run method
 *
 *  Destructor method
 *
 *  \param[in] None
 *  \return    None
 */
void CTask::run()
{
    if (m_triggered)
    {
        m_triggered = false;
        _run();
    }
}

/******************************************************************************/
/** \brief  CTaskManager class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_taskList      task list
 *  \param[in] f_taskCount     task count
 *  \param[in] f_baseFreq      base frequency
 */
CTaskManager::CTaskManager(CTask** f_taskList, uint32_t f_taskCount, float f_baseFreq)
    : m_taskList(f_taskList)
    , m_taskCount(f_taskCount) 
{
    m_ticker.attach(mbed::callback(&CTaskManager::callback, this), f_baseFreq);
}

/** \brief  CTaskManager class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
CTaskManager::~CTaskManager() 
{
    m_ticker.detach();
}

/** \brief  Callback method
 *
 *  \param[in] thisPointer       The object pointer
 */
void CTaskManager::callback(void *thisPointer)
{
    CTaskManager* self = static_cast<CTaskManager*>(thisPointer);
    self->timerCallback(); 
}