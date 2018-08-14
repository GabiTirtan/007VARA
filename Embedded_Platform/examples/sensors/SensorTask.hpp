/**
  ******************************************************************************
  * @file    SensorTask.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the sensor task
  *          functionality.
  ******************************************************************************
 */

/* Include guard */
#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

/* Standard C library for basic mathematical operations */
#include <math.h>
#include"TaskManager.hpp"
#include "Encoder.hpp"
#include "VNH.hpp"
#include "SerialMonitor.hpp"
#include "EncoderFilter.hpp"
#include "Controller.hpp"
#include "SystemModels.hpp"

namespace examples
{
    //!  CDISTFTest class. 
    /*!
    * It inherits class CTask. 
    * It is used for testing distance sensors operation.
    */
    class CDISTFTest:public CTask
    {
        public:
            /* Constructor */
            CDISTFTest(uint32_t        f_period
                    ,Serial&         f_serial);
            
        private:
            /* Run method */
            void _run();
            
            /* Reference to serial object */
            Serial&                                                       m_serial;
            /* Transfer function */
            systemmodels::lti::siso::CDiscreteTransferFucntion<float,3,3> m_tf;
    };

    //!  CEncoderSender class. 
    /*!
    * It inherits class CTask. 
    * It is used for sending acquired data.
    */
    class CEncoderSender:public CTask
    {
        public:
            /* Constructor */
            CEncoderSender(uint32_t            f_period
                           ,CEncoderFilter&    f_encoder
                           ,task::CControl&    f_control
                           ,Serial&            f_serial);
            /* Serial callback */
            static void staticSerialCallback(void* obj,char const * a, char * b);
        private:
            /* Serial callback implementation */
            void serialCallback(char const * a, char * b);
            /* Run method */
            void _run();

            /* Active flag */
            bool                m_isActived;
            /* Filtered encoder value */
            CEncoderFilter&     m_encoder;
            /* Controller object reference */
            task::CControl&     m_control;
            /* Serial communication member*/
            Serial&             m_serial;
    };

 
};


#endif