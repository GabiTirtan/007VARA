/**
  ******************************************************************************
  * @file    SplineInterpreter.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the spline interpreter
  *          functionality.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef SPLINE_INTERPRETER_HPP
#define SPLINE_INTERPRETER_HPP

#include <mbed.h>
#include <Complex>
#include "Queue.hpp"

/* Module defines */
#define SPLINE_MESSAGE_LENGTH 100

#define NR_ACCESSED_SPLN_INTR 2

//!  CSplineInterpreter class. 
/*! 
 *  It is used for implementing the spline itnerpreter.
 * */
class CSplineInterpreter{
    public:
    
        typedef struct{
            bool isForward;
            std::complex<float> a,b,c,d;
            float duration_sec;
        }SplineInterpreter_Data;
        /* Constructor */
        CSplineInterpreter():data(){};
        /* Destructor */
        ~CSplineInterpreter(){};
        /* Serial callback method */
        static void staticSerialCallback(void* obj,char const * message, char * response);
        /* New data available informer */
        bool isNewDataAvailable();
        /* Get data */
        SplineInterpreter_Data getData();
    private:
        /* Serial callback actions */
        void serialCallback(char const * f_message, char * f_response);
        /* Set data method */
        inline void setData(bool        isForward,
                            float       f_a_x,
                            float       f_a_y,
                            float       f_b_x,
                            float       f_b_y,
                            float       f_c_x,
                            float       f_c_y,
                            float       f_d_x,
                            float       f_d_y,
                            float       f_duration_sec);
        /* New data flag */
        bool isNewData;
        /* Interpreter data  */
        SplineInterpreter_Data data;
};

#endif