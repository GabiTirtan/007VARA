/** \brief  PolynomialFunction class constructor
 *
 *  Constructor method
 *
 *  \param[in] None
 */
template<class T,int32_t N>
PolynomialFunction<T,N>::PolynomialFunction()
{
    for(int i=0;i<=N;++i){
        this->coefficients[i]=static_cast<T>(0);
    }
}

/** \brief  PolynomialFunction class constructor
 *
 *  Constructor method
 *
 *  \param[in] coefficients[N+1]     coefficients array
 */
template<class T,int32_t N>
PolynomialFunction<T,N>::PolynomialFunction(T coefficients[N+1])
{
    for(int i=0;i<=N;++i){
        this->coefficients[i]=coefficients[i];
    }
}

/** \brief  PolynomialFunction class destructor
 *
 *  Destructor method
 *
 *  \param[in] None
 */
template<class T,int32_t N>
PolynomialFunction<T,N>::~PolynomialFunction()
{
}

/** \brief  Get grade method
 *
 *  \param[in] None
 *  \return    Grade
 */
template<class T,int32_t N>
int32_t PolynomialFunction<T,N>::getGrade()
{
    return N;
}

/** \brief  Get coefficient value method
 *
 *  \param[in]index          index of coefficient
 *  \return   coefficient value
 */
template<class T,int32_t N>
T PolynomialFunction<T,N>::getCoefficientValue(int32_t index)
{
    if(index>N || index<0) return static_cast<T>(0);
    return this->coefficients[index];
}

/** \brief  Get coefficient value method
 *
 *  \param[in] index          index of coefficient
 *  \param[in] value          value to which coefficient is set
 *  \return    None
 */
template<class T,int32_t N>
void PolynomialFunction<T,N>::setCoefficientValue(int32_t index, T value ){
    if(index<=N){
        this->coefficients[index]=value;
    }
}

/** \brief  Add method
 *
 *  \param[in] b          polynomial function to be added
 *  \return    New polynomial function value
 */
template<class T,int32_t N>
template<int32_t N2>
PolynomialFunction<T,(N2<N?N:N2)> PolynomialFunction<T,N>::add(PolynomialFunction<T,N2> b){
    PolynomialFunction<T,N2<N?N:N2> p;
    for(int i=0;i<=N2 || i<=N;++i){
        p.setCoefficientValue(i,this->getCoefficientValue(i)+b.getCoefficientValue(i));
    }
    return p;
}

/** \brief  Multiply method
 *
 *  \param[in] b          polynomial function to be multiplied by
 *  \return    New polynomial function value
 */
template<class T,int32_t N>
template<int32_t N2>
PolynomialFunction<T,(N2+N)> PolynomialFunction<T,N>::multip(PolynomialFunction<T,N2> b){
    PolynomialFunction<T,(N2+N)> result;
    for(int32_t i=0;i<=N2+N;++i){
        T sum=static_cast<T>(0);
        for(int32_t j=0;j<=i;++j){
            sum+=this->getCoefficientValue(j)*b.getCoefficientValue(i-j);
        }
        result.setCoefficientValue(i,sum);
    }

    return result;
}

/** \brief  caluclate value method
 *
 *  \param[in] input_value          input value
 *  \return    Computed value
 */
template<class T,int32_t N>
T PolynomialFunction<T,N>::calculateValue(T input_value){


    T output_value=static_cast<T>(0);
    for(int32_t i=0;i<=N;++i){
        output_value+=this->coefficients[i]*static_cast<T>(pow(input_value,i));
    }
//    std::cout<<"OOO"<<output_value<<std::endl;
    return output_value;
}

/** \brief  First order derivate method
 *
 *  \param[in] None
 *  \return    Derivate
 */
template<class T, int32_t N>
PolynomialFunction<T,N-1> PolynomialFunction<T,N>::derivateFO(){
    PolynomialFunction<T,N-1> derivate;
    for(int32_t i=0;i<N;++i){
        T coeff=static_cast<T>(i+1);
        derivate.setCoefficientValue(i,coeff*this->getCoefficientValue(i+1));
    }
    return derivate;
};
