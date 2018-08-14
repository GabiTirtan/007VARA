/**
  ******************************************************************************
  * @file    SplineInterpreter.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class definition for the spline interpreter
  *          functionality.
  ******************************************************************************
 */

#include "SplineInterpreter.hpp"

/** \brief  Serial callback method for Spline command
  *
  * Serial callback attaching serial callback to spline interpreter object
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  message             string to read data from
  * \param[out] response            string to write data to
  * \return None
  */
void CSplineInterpreter::staticSerialCallback(void* obj,char const * message, char * response)
{
    CSplineInterpreter* self=static_cast<CSplineInterpreter*>(obj);
    self->serialCallback(message,response);
}

/** \brief  New data available informer method
  *
  * \param[in]  None
  * \return     Flag indicating whether new data is available
  */
bool CSplineInterpreter::isNewDataAvailable()
{
    return this->isNewData;
}

/** \brief  Get spline data
  *
  * \param[in]  None
  * \return     Spline interpreter data
  */
CSplineInterpreter::SplineInterpreter_Data CSplineInterpreter::getData()
{
    this->isNewData=false;
    return this->data;
}

/** \brief  Set spline data
  *
  * \param[in]  f_isForward        flag indicating forward/backward action
  * \param[in]  f_a_x              x coordinate for point A
  * \param[in]  f_a_y              y coordinate for point A
  * \param[in]  f_b_x              x coordinate for point B
  * \param[in]  f_b_y              y coordinate for point B
  * \param[in]  f_c_x              x coordinate for point C
  * \param[in]  f_c_y              y coordinate for point C
  * \param[in]  f_d_x              x coordinate for point D
  * \param[in]  f_d_y              y coordinate for point D
  * \param[in]  f_duration_sec     movement duration in seconds
  * \return     None
  */
void CSplineInterpreter::setData(bool        f_isForward,
                                 float       f_a_x,
                                 float       f_a_y,
                                 float       f_b_x,
                                 float       f_b_y,
                                 float       f_c_x,
                                 float       f_c_y,
                                 float       f_d_x,
                                 float       f_d_y,
                                 float       f_duration_sec)
{
    this->data.isForward=f_isForward;
    this->data.a=std::complex<float>(f_a_x,f_a_y);
    this->data.b=std::complex<float>(f_b_x,f_b_y);
    this->data.c=std::complex<float>(f_c_x,f_c_y);
    this->data.d=std::complex<float>(f_d_x,f_d_y);
    this->data.duration_sec=f_duration_sec;
    this->isNewData=true;
}

void CSplineInterpreter::serialCallback(char const * f_message, char * f_response){
     float a_x,a_y,b_x,b_y,c_x,c_y,d_x,d_y,duration_sec;
     int isForward=1;
     int32_t nrData=sscanf(f_message,"%d;%f;%f;%f;%f;%f;%f;%f;%f;%f",
                                    &isForward,
                                    &a_x,
                                    &a_y,
                                    &b_x,
                                    &b_y,
                                    &c_x,
                                    &c_y,
                                    &d_x,
                                    &d_y,
                                    &duration_sec);
    if(10==nrData && duration_sec>0 && (isForward==1 || isForward==0)){
        setData(isForward,a_x,a_y,b_x,b_y,c_x,c_y,d_x,d_y,duration_sec);
        sprintf(f_response,"ack;;");
    }
    else{
        sprintf(f_response,"sintax error;;");
    }
}