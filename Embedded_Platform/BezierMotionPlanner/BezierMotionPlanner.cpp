/**
  ******************************************************************************
  * @file    BezierMotionPlanner.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class definition for the Bezier Motion
  *          Planner methods.
  ******************************************************************************
 */

#include <iostream>
#include "BezierMotionPlanner.hpp"

/** \brief  CBezierMotionPlanner class constructor
 *
 *  Constructor method without initialization
 *
 *  \param[in] None
 */
CBezierMotionPlanner::CBezierMotionPlanner()
{
    this->isInitialized=false;
}

/** \brief  CBezierMotionPlanner class constructor
 *
 *  Constructor method with initialization
 *
 *  \param[in]isForward           forward movement flag
 *  \param[in]a                   point A
 *  \param[in]b                   point B
 *  \param[in]c                   point C
 *  \param[in]d                   point D
 *  \param[in]motion_duration_i   motion duration in seconds
 *  \param[in]timestep_i          time step
 */
CBezierMotionPlanner::CBezierMotionPlanner(bool                    isForward,
                                           std::complex<float>     a,
                                           std::complex<float>     b,
                                           std::complex<float>     c,
                                           std::complex<float>     d,
                                           float                   motion_duration_i,
                                           float                   timestep_i)
    : isForward(isForward)
    , bezierCurve(a,b,c,d)
    , motion_duration(motion_duration_i)
    , time_step(timestep_i)
{
    this->bezierValueInput_step=1.0/(int)(this->motion_duration/this->time_step);
    this->next_bezierValueInput=0.0;
    this->isInitialized=true;
}

/** \brief  Set motion planner parameters
 *
 *  \param[in]isForward           forward movement flag
 *  \param[in]a                   point A
 *  \param[in]b                   point B
 *  \param[in]c                   point C
 *  \param[in]d                   point D
 *  \param[in]motion_duration_i   motion duration in seconds
 *  \param[in]timestep_i          time step
 *  \return None
 */
void CBezierMotionPlanner::setMotionPlannerParameters(bool                    isForward,
                                                      std::complex<float>     a,
                                                      std::complex<float>     b,
                                                      std::complex<float>     c,
                                                      std::complex<float>     d,
                                                      float                   motion_duration_i,
                                                      float                   timestep_i)
{
    this->isForward=isForward;
    this->motion_duration=motion_duration_i;
    this->time_step=timestep_i;
    this->bezierCurve.setBezierCurve(a,b,c,d);
    this->bezierValueInput_step=1.0/(int)(this->motion_duration/this->time_step);
    this->next_bezierValueInput=0.0;
    this->isInitialized=true;                        
}

/** \brief  CBezierMotionPlanner class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
CBezierMotionPlanner::~CBezierMotionPlanner()
{
}

/** \brief  Get Bezier Curve
 *
 *  Destructor method
 *
 *  \param[in] None
 *  \return Bezier Curve
 */
BezierCurve<float> CBezierMotionPlanner::getBezierCurve()
{
    return this->bezierCurve;
}

/** \brief  Get next velocity
 *
 *  \param[in] None
 *  \return Next velocity value (longitudinal and angular)
 */
std::pair<float,float> CBezierMotionPlanner::getNextVelocity(){
    std::pair<float,float> commands=this->getVelocity(next_bezierValueInput);
    this->next_bezierValueInput+=this->bezierValueInput_step;
    return commands;
}

/** \brief  Get velocity
 *
 *  \param[in] input_value
 *  \return Velocity value (longitudinal and angular; Input value [0,1])
 */
std::pair<float,float> CBezierMotionPlanner::getVelocity(float input_value)
{
    if(!this->isInitialized) return std::pair<float,float>(0,0);
    
    std::complex<float> dS=this->bezierCurve.get_FO_DerivateValue(input_value);

    float dl_absolute=sqrt((dS*std::conj(dS)).real());//[0,1]//Length of the vector
    float dl_real=dl_absolute/this->motion_duration;

    std::complex<float> ddS=this->bezierCurve.get_SO_DerivateValue(input_value);

    std::complex<float> correctorValue(0,-2);
    std::complex<float> temp1=(dS*std::conj(ddS)-std::conj(dS)*ddS)/correctorValue;
    float num=temp1.real();
    if(dl_absolute==0)
    {
        return std::pair<float,float>(dl_real,0);
    }

    float k=num/pow(dl_absolute,3);

    float angle_rad=atan(k*WHEELBASE);
    float angle_deg=(180.f/M_PI)*angle_rad;

    std::pair<float,float> commands(dl_real,angle_deg);
    return commands;
}

/** \brief  Next value valid
 *
 *  \param[in] None
 *  \return Boolean indicating whether next value is valid
 */
bool CBezierMotionPlanner::hasValidValue()
{
    return (next_bezierValueInput>=0 && next_bezierValueInput<=1);
}

/** \brief  Is movement forward
 *
 *  \param[in] None
 *  \return Boolean indicating whether movement is forward
 */
bool CBezierMotionPlanner::getForward()
{
    return this->isForward;
}