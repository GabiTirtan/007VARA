#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "linalg.h"
#include "SystemModels.hpp"

namespace controller
{
    namespace siso
    {
        //!  CController class. 
        template<class T>
        class CController{
            public:
                virtual T operator()(const T&)=0;
                virtual void clear()=0;
        };

        //!  CPidController class. 
        /*!
        * PID Controller class 
        * It is used for implementing PID control.
        */
        template<class T>
        class CPidController:public CController<T>
        {
            public:
                /* Transfer functions */
                using CIntegratorTF = systemmodels::lti::siso::CDiscreteTransferFucntion<T,2,2>;
                using CDerivatorTF  = systemmodels::lti::siso::CDiscreteTransferFucntion<T,2,2>;
                /* Constructor */
                CPidController(T              f_kp
                              ,T              f_ki
                              ,T              f_kd
                              ,T              f_tf
                              ,T              f_dt);
                /* Overloaded operator */
                T operator()(const T& f_input);
                /* Serial callback */
                static void staticSerialCallback(void* obj,char const * a, char * b);
                /* Initialization */
                void clear();
            private:
                /* Controller setting */
                void setController(T              f_kp
                                  ,T              f_ki
                                  ,T              f_kd
                                  ,T              f_tf);
                /* Serial callback implementation */
                void serialCallback(char const * a, char * b);

                /* Proportional factor */
                T                       m_kp;
                /* Integrator transfer fucntion */
                CIntegratorTF           m_intTf;
                /* Derivator transfer fucntion */
                CDerivatorTF            m_derTf;
                /* delta-time term */
                float                   m_dt;

        };
    }
}

/* Include function definitions */
#include "PidController.inl"

#endif