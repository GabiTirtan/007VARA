/**
  ******************************************************************************
  * @file    Timer.inl
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the timer functionality.
  *          Inline implementation.
  ******************************************************************************
 */

/** \brief  CTimer class constructor
 *
 *  Constructor method
 *
 *  \param[in] None
 */
template <unsigned int N, unsigned int D>
CTimer<N,D>::CTimer()
    : m_ticker()
    , m_millisValue(0)
    , m_started(0)
{
}

/** \brief  CTimer class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
template <unsigned int N, unsigned int D>
CTimer<N,D>::~CTimer() 
{ 
    if (m_started) 
        stop ();
}

/** \brief  Start timer
 *
 *  \param[in] None
 *  \return    None
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::start () 
{
    m_started = 1;
    m_ticker.attach (mbed::callback(&CTimer::callback, this), 1.f*N/D);  
}
    
/** \brief  Stop timer
 *
 *  \param[in] None
 *  \return    None
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::stop() 
{
    m_started = 0;
    m_ticker.detach ();
}

/** \brief  Get timer
 *
 *  \param[in] None
 *  \return    Milliseconds value
 */
template <unsigned int N, unsigned int D>
uint32_t CTimer<N,D>::get()
{
    return m_millisValue;
}

/** \brief  Timer callback
 *
 *  \param[in] thisPointer   The object pointer.
 *  \return    None
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::callback(void *thisPointer)
{
    CTimer<N,D>* self = static_cast<CTimer<N,D>*>(thisPointer);
    self->millisTicker(); 
}

/** \brief Increase milliseconds value
 *
 *  \param[in] None
 *  \return    None
 */
template <unsigned int N, unsigned int D>
void CTimer<N,D>::millisTicker()
{
    m_millisValue ++;
}