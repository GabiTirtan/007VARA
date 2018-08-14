/** \brief  BezierCurve class constructor
 *
 *  Constructor method
 *
 *  \param[in] None
 */
template<class T>
BezierCurve<T>::BezierCurve(){
}

/** \brief  Bezier Curve class constructor
 *
 *  Constructor method
 *
 *  \param[in] a       point A
 *  \param[in] b       point B
 *  \param[in] c       point C
 *  \param[in] d       point D 
 */
template<class T>
BezierCurve<T>::BezierCurve(std::complex<T> a,std::complex<T> b,std::complex<T> c,std::complex<T> d){
    this->points[0]=a;
    this->points[1]=b;
    this->points[2]=c;
    this->points[3]=d;

    this->bezierCurve=this->CP2PF();
    this->FOder_bezierCurve=this->bezierCurve.derivateFO();
    this->SOder_bezierCurve=this->FOder_bezierCurve.derivateFO();
}

/** \brief  Bezier Curve class constructor
 *
 *  Constructor method
 *
 *  \param[in] points[BEZIER_ORDER+1]    Array of points
 */
template<class T>
BezierCurve<T>::BezierCurve(std::complex<T> points[BEZIER_ORDER+1])
{
    for(int32_t i=0;i<=BEZIER_ORDER;++i){
        this->points[i]=points[i];
    }
    this->bezierCurve=this->CP2PF();
    this->FOder_bezierCurve=this->bezierCurve.derivateFO();
    this->SOder_bezierCurve=this->FOder_bezierCurve.derivateFO();
}

/** \brief  Bezier Curve class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
template<class T>
BezierCurve<T>::~BezierCurve()
{
    //dtor
}

/** \brief  Complex points to polynomial function converter method
 *
 *  Converts the complex points to polynomial function
 *
 *  \param[in] None
 *  \return    Plynomial function
 */
template<class T>
PolynomialFunction<std::complex<float>,BEZIER_ORDER> BezierCurve<T>::CP2PF(){
    PolynomialFunction<std::complex<float>,BEZIER_ORDER> pf;

    const std::complex<T> temp_cst_3(3,0);
    const std::complex<T> temp_cst_2(2,0);

    std::complex<T> coef1=temp_cst_3*(this->points[1]-this->points[0]);
    std::complex<T> coef2=temp_cst_3*(this->points[0]-temp_cst_2*this->points[1]+this->points[2]);
    std::complex<T> coef3=temp_cst_3*(this->points[1]-this->points[2])+this->points[3]-this->points[0];
    pf.setCoefficientValue(0,this->points[0]);
    pf.setCoefficientValue(1,coef1);
    pf.setCoefficientValue(2,coef2);
    pf.setCoefficientValue(3,coef3);
    return pf;
}

/** \brief  Get value method
 *
 *  \param[in] input_value
 *  \return    Computed value
 */
template<class T>
std::complex<T> BezierCurve<T>::getValue(float input_value){
    T input_value_T=static_cast<T>(input_value);
    return this->bezierCurve.calculateValue(input_value);
}

/** \brief  Get first order derivate value
 *
 *  \param[in] input_value
 *  \return    Computed value
 */
template<class T>
std::complex<T> BezierCurve<T>::get_FO_DerivateValue(float input_value){
    T input_value_T=static_cast<T>(input_value);
    return this->FOder_bezierCurve.calculateValue(input_value_T);
}

/** \brief  Get second order derivate value
 *
 *  \param[in] input_value
 *  \return    Computed value
 */
template<class T>
std::complex<T> BezierCurve<T>::get_SO_DerivateValue(float input_value){
    T input_value_T=static_cast<T>(input_value);

    return this->SOder_bezierCurve.calculateValue(input_value_T);
}

/** \brief  Get Bezier Curve
 *
 *  \param[in] None
 *  \return    Bezier Curve
 */
template<class T>
PolynomialFunction<std::complex<T>,BEZIER_ORDER> BezierCurve<T>::getBezierCurve(){
    return this->bezierCurve;
}

/** \brief  Get first order derivate
 *
 *  \param[in] input_value
 *  \return    First order derivate
 */
template<class T>
PolynomialFunction<std::complex<T>,BEZIER_ORDER-1> BezierCurve<T>::getFODerivate(){
    return this->FOder_bezierCurve;
}

/** \brief  Get second order derivate
 *
 *  \param[in] input_value
 *  \return    Second order derivate
 */
template<class T>
PolynomialFunction<std::complex<T>,BEZIER_ORDER-2> BezierCurve<T>::getSODerivate(){
    return this->SOder_bezierCurve;
}

/** \brief  Set Bezier Curve parameters
 *
 *  \param[in] a       point A
 *  \param[in] b       point B
 *  \param[in] c       point C
 *  \param[in] d       point D 
 *  \return    None
 */
template<class T>
void  BezierCurve<T>::setBezierCurve(std::complex<T> a,std::complex<T> b,std::complex<T> c,std::complex<T> d){
    this->points[0]=a;
    this->points[1]=b;
    this->points[2]=c;
    this->points[3]=d;

    this->bezierCurve=this->CP2PF();
    this->FOder_bezierCurve=this->bezierCurve.derivateFO();
    this->SOder_bezierCurve=this->FOder_bezierCurve.derivateFO();
}
