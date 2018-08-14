/**
  ******************************************************************************
  * @file    Controller.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the controller
  *          functionality.
  ******************************************************************************
 */

/* Include guard */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include<cmath>
#include"PidController.hpp"
#include"TaskManager.hpp"
#include"EncoderFilter.hpp"

namespace task
{
    //!  CControl class. 
    /*!
    * It is used for implementing the control action.
    */
    class CControl
    {
        /* PID controller declaration*/
        template <class T>
        using  ControllerType=controller::siso::CController<T>;

        public:
            /* Construnctor */
            CControl(CEncoderFilter&                 f_encoder
                    ,ControllerType<double>&         f_pid);
            /* Set controller reference value */
            void setRef(double                       f_RefRps);
            /* Get controller reference value */
            double getRef();
            /* Get control value */
            double get();
            /* Get error */
            double getError();
            /* Clear PID parameters */
            void clear();
            /* Control action */
            double control();

        private:
            /* PWM onverter */
            double converter(double f_u);

            /* Enconder object reference */
            CEncoderFilter&             m_encoder;
            /* PID object reference */
            ControllerType<double>&     m_pid;
            /* Controller reference */
            double                      m_RefRps;
            /* Control value */
            double                      m_u;
            /* Error */
            double                      m_error;
    };
};

#endif