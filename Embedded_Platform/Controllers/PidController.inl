/** \brief  Class constructor
  *
  * Constructor method
  *
  * \param[in] f_kp                proportional factor
  * \param[in] f_ki                integral factor
  * \param[in] f_kd                derivative factor
  * \param[in] f_tf                derivative time filter constant
  * \param[in] f_dt                sample time
  */
template<class T>
controller::siso::CPidController<T>::CPidController(T              f_kp
                                                   ,T              f_ki
                                                   ,T              f_kd
                                                   ,T              f_tf
                                                   ,T              f_dt)
    :m_kp(f_kp)
    ,m_intTf()
    ,m_derTf()
    ,m_dt(f_dt)
{

    /* This is a parallel and trapezoidal PID */
    linalg::CMatrix<T,2,1> l_numIntM({std::array<T,1>({f_ki*f_dt}),std::array<T,1>({f_ki*f_dt})});
    m_intTf.setNum(l_numIntM);
    linalg::CMatrix<T,2,1> l_denIntM({std::array<T,1>({2}),std::array<T,1>({-2})});
    m_intTf.setDen(l_denIntM);

    linalg::CMatrix<T,2,1> l_numDerM({std::array<T,1>({2*f_kd}),std::array<T,1>({-2*f_kd})});
    m_derTf.setNum(l_numDerM);
    linalg::CMatrix<T,2,1> l_denDerM({std::array<T,1>({2*f_tf+f_dt}),std::array<T,1>({f_dt-2*f_tf})});
    m_derTf.setDen(l_denDerM);
    
    // linalg::CMatrix<T,2,1>({std::array<T,1>({2*f_kd}),std::array<T,1>({-2*f_kd})}),linalg::CMatrix<T,2,1>({std::array<T,1>({f_tf*2+f_dt}),std::array<T,1>({f_dt-2*f_tf})})
}

/** \brief  Operator
  *
  * \param[in] f_input             input function
  * \return                         control value
  */
template<class T>
T controller::siso::CPidController<T>::operator()(const T& f_input)
{
    return m_kp*f_input+m_intTf(f_input)+m_derTf(f_input);
}

/** \brief  Serial callback method
  *
  * Serial callback attaching serial callback to controller object
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
template<class T>
void controller::siso::CPidController<T>::staticSerialCallback(void* obj,char const * a, char * b)
{
    CPidController* self = static_cast<CPidController*>(obj);
    self->serialCallback(a,b);
}

/** \brief  Initialization method
  *
  * Sets all model params. to zero
  *
  * \param[in] None
  * \return None
  */
template<class T>
void controller::siso::CPidController<T>::clear()
{
    m_intTf.clearMemmory();
    m_derTf.clearMemmory();
}

/** \brief  Controller setting
  *
  * Controller setting
  *
  * \param[in] f_kp                proportional factor
  * \param[in] f_ki                integral factor
  * \param[in] f_kd                derivative factor
  * \param[in] f_tf                derivative time filter constant
  * \return None
  */
template<class T>
void controller::siso::CPidController<T>::setController(
    T              f_kp,
    T              f_ki,
    T              f_kd,
    T              f_tf)
{
    m_kp=f_kp;
    linalg::CMatrix<T,2,1> l_numIntM({std::array<T,1>({f_ki*m_dt}),std::array<T,1>({f_ki*m_dt})});
    m_intTf.setNum(l_numIntM);
    linalg::CMatrix<T,2,1> l_denIntM({std::array<T,1>({2}),std::array<T,1>({-2})});
    m_intTf.setDen(l_denIntM);

    linalg::CMatrix<T,2,1> l_numDerM({std::array<T,1>({2*f_kd}),std::array<T,1>({-2*f_kd})});
    m_derTf.setNum(l_numDerM);
    linalg::CMatrix<T,2,1> l_denDerM({std::array<T,1>({2*f_tf+m_dt}),std::array<T,1>({m_dt-2*f_tf})});
    m_derTf.setDen(l_denDerM);
}

/** \brief  Serial callback actions
  *
  * Serial callback method setting controller to values received
  *
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
template<class T>
void controller::siso::CPidController<T>::serialCallback(char const * a, char * b)
{
    float l_kp,l_ki,l_kd,l_tf;
    uint32_t l_res = sscanf(a,"%f;%f;%f;%f;",&l_kp,&l_ki,&l_kd,&l_tf);
    if (4 == l_res)
    {
        setController(l_kp,l_ki,l_kd,l_tf);
        sprintf(b,"ack;;%2.5f;%2.5f;%2.5f;%2.5f;",l_kp,l_ki,l_kd,l_tf);
    }
    else
    {
        sprintf(b,"sintax error;;");
    }
}