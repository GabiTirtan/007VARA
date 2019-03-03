/**
  ******************************************************************************
  * @file    BezierCurve.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class declaration for the Bezier Curve
  *          methods.
  ******************************************************************************
 */

/* include guard */
#ifndef BEZIERCURVE_HPP
#define BEZIERCURVE_HPP

/* complex numbers in cartesian form and several functions and overloads to operate with them */
#include <complex>
#include "PolynomialFunction.hpp"

#define BEZIER_ORDER 3

//!  BezierCurve class. 
/*! 
 *  It is used for implementing Bezier Curve related methods.
 * */
template<class T>
class BezierCurve
{
    public:
        /* Constructors */
        BezierCurve();
        BezierCurve(std::complex<T> points[BEZIER_ORDER+1]);
        BezierCurve(std::complex<T> a,std::complex<T> b,std::complex<T> c,std::complex<T> d);
        /* Destructor */
        virtual ~BezierCurve();
        /* Set curve */
        void setBezierCurve(std::complex<T> a,std::complex<T> b,std::complex<T> c,std::complex<T> d);
        /* Get value */
        std::complex<T> getValue(float input_value);
        /* First order derivate value */
        std::complex<T> get_FO_DerivateValue(float input_value);
        /* Second order derivate value */
        std::complex<T> get_SO_DerivateValue(float input_value);

        PolynomialFunction<std::complex<T>,BEZIER_ORDER > getBezierCurve();
        PolynomialFunction<std::complex<T>,BEZIER_ORDER-1> getFODerivate();
        PolynomialFunction<std::complex<T>,BEZIER_ORDER-2> getSODerivate();

    protected:

    private:
//        PRIVATE FUNCTIONS
//      Convert the complex points to polynomial function
        PolynomialFunction<std::complex<float>,BEZIER_ORDER> CP2PF();

//        PRIVATE PARAMETERS
        std::complex<T> points[BEZIER_ORDER+1];
        PolynomialFunction<std::complex<T>,BEZIER_ORDER> bezierCurve;
        PolynomialFunction<std::complex<T>,BEZIER_ORDER-1> FOder_bezierCurve;
        PolynomialFunction<std::complex<T>,BEZIER_ORDER-2> SOder_bezierCurve;
};

#include "BezierCurve.inl"

#endif // BEZIERCURVE_HPP
