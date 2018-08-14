/**
  ******************************************************************************
  * @file    PolynomialFunction.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the Plynomial Function
  *          methods.
  ******************************************************************************
 */

/* include guard */
#ifndef POLYNOMIALFUNCTION_HPP
#define POLYNOMIALFUNCTION_HPP

#include <stdint.h>
#include <math.h>
#include <iostream>

//!  PolynomialFunction class. 
/*! 
 *  It is used for implementing PolynomialFunction related methods.
 * */
template <class T,int32_t N>
class PolynomialFunction
{
    public:
        /* Constructors  */
        PolynomialFunction();
        PolynomialFunction(T coefficients[N+1]);
        /* Destructors  */
        virtual ~PolynomialFunction();

//        template<int32_t N2> void add(PolynomialFunction<T,N2> poli);
        /* Add */
        template<int32_t N2> PolynomialFunction<T,(N2<N?N:N2)> add(PolynomialFunction<T,N2> b);
        /* Multiply */
        template<int32_t N2> PolynomialFunction<T,(N2+N)> multip(PolynomialFunction<T,N2> b);
        /* Calculate value */
        T calculateValue(T input_value);
        /* First order derivate */
        PolynomialFunction<T,N-1> derivateFO();
        /* Get grade */
        int32_t getGrade();
        /* Get coefficient value */
        T getCoefficientValue(int32_t index);
        /* Set coefficient value */
        void setCoefficientValue(int32_t index,T value );
    private:
        /* Coefficients array */
        T coefficients[N+1];
};

#include "PolynomialFunction.inl"

#endif // POLYNOMIALFUNCTION_HPP
