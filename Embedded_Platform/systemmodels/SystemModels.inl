/******************************************************************************/
/** \brief  CDiscreteTransferFucntion class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_dt      Time step
 */
template <class T,uint32_t NNum,uint32_t NDen>
systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::CDiscreteTransferFucntion()
    :m_num(CNumType::zeros())
    ,m_den(CDenModType::zeros())
    ,m_memInput()
    ,m_memOutput()
{
}

/** \brief  Clear memory
 *
 *  \param[in] None
 *  \return    None
 */
template <class T,uint32_t NNum,uint32_t NDen>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::clearMemmory()
{
    m_memInput=CInputMem::zeros();
    m_memOutput=COutputMem::zeros();
}

/** \brief  Clear memory
 *
 *  \param[in] f_input      reference to input
 *  \return    l_output
 */
template <class T,uint32_t NNum,uint32_t NDen>
T systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::operator()(const T& f_input)
{
    shiftMemory<NNum>(m_memInput);
    m_memInput[0][0]=f_input;
    T l_output=(m_memInput*m_num-m_memOutput*m_den)[0][0];
    shiftMemory<NDen-1>(m_memOutput);
    m_memOutput[0][0]=l_output;
    return l_output;
}

/** \brief  Set num 
 *
 *  \param[in] f_num      reference to nominator
 *  \return    None
 */
template <class T,uint32_t NNum,uint32_t NDen>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::setNum(const CNumType& f_num)
{
    for(uint32_t i=0;i<NNum;++i)
    {
        m_num[i][0]=f_num[i][0];
    }
}

/** \brief  Set den 
 *
 *  \param[in] f_den      reference to denominator
 *  \return    None
 */
template <class T,uint32_t NNum,uint32_t NDen>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::setDen(const CDenType& f_den)
{
    for(uint32_t i=1;i<NDen;++i)
    {
        m_den[i-1][0]=f_den[i][0]/f_den[0][0];
    }
}

/** \brief  Get output 
 *
 *  \param[in] None
 *  \return    Output memory
 */
template <class T,uint32_t NNum,uint32_t NDen>
T systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::getOutput()
{
    return m_memOutput[0][0];
}

/** \brief  Shift memory
 *
 *  \param[in] f_mem           reference to memory matrix
 *  \return    none
 */
template <class T,uint32_t NNum,uint32_t NDen>
template<uint32_t N>
void systemmodels::lti::siso::CDiscreteTransferFucntion<T,NNum,NDen>::shiftMemory(linalg::CMatrix<T,1,N>& f_mem)
{
    for(uint32_t i=N-1;i>0;--i)
    {
        f_mem[0][i]=f_mem[0][i-1];
    }
}

/******************************************************************************/
/** \brief  CDiscreteTimeSystemModel class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_dt      Time step
 */
template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>::CDiscreteTimeSystemModel(const double f_dt)
    : m_states()
    , m_dt(f_dt)
{
}

/** \brief  CDiscreteTimeSystemModel class constructor
 *
 *  Constructor method
 *
 *  \param[in] f_states  States
 *  \param[in] f_dt      Time step
 */
template <class T,uint32_t NA, uint32_t NB,uint32_t NC>
systemmodels::nlti::mimo::CDiscreteTimeSystemModel<T,NA,NB,NC>::CDiscreteTimeSystemModel(
        const CStatesType&     f_states
       ,const double           f_dt)
    : m_states(f_states)
    , m_dt(f_dt)
{
}