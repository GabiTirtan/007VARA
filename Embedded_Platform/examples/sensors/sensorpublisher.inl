/** \brief  Constructor for the CEncoderFilter class
 *
 *  Constructor method
 *
 *  \param[in] f_period       period value 
 *  \param[in] f_sensors      sensor array 
 *  \param[in] f_serial       reference to the serial object
 */
template <class C_Sensor,uint Nr_Senrsor> 
ProximityPublisher<C_Sensor,Nr_Senrsor>::ProximityPublisher(uint32_t        f_period
                                                           ,SensorArrayT    f_sensors
                                                           ,Serial&         f_serial)
    :CTask(f_period)
    ,m_sensors(f_sensors)
    ,m_serial(f_serial)
    ,m_isActivate(false)
{
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
template <class C_Sensor,uint Nr_Senrsor> 
void ProximityPublisher<C_Sensor,Nr_Senrsor>::staticSerialCallback(void* obj,char const * a, char * b)
{
    ProximityPublisher* self = static_cast<ProximityPublisher*>(obj);
    self->serialCallback(a,b);
}

/** \brief  Serial callback actions 
  *
  * Serial callback
  *
  * \param[in]  obj                 PID controller object
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
template <class C_Sensor,uint Nr_Senrsor> 
void ProximityPublisher<C_Sensor,Nr_Senrsor>::serialCallback(char const * a, char * b){
    int l_isActivate=0;
    uint32_t l_res = sscanf(a,"%d",&l_isActivate);
    if(l_res==1){
        m_isActivate=(l_isActivate>=1);
        sprintf(b,"ack;;");
    }else{
        sprintf(b,"sintax error;;");
    }
}

/** \brief  Method called each f_period
 * 
 *  \param[in]  None
 *  \param[out] None
 */
template <class C_Sensor,uint Nr_Senrsor> 
void ProximityPublisher<C_Sensor,Nr_Senrsor>::_run(){
    if(!m_isActivate) return;
    char l_buf[100];
    sprintf(l_buf,"@DSPB:");
    for(uint8_t i=0;i<Nr_Senrsor;++i){
        
        sprintf(l_buf,"%s%2.2f;",l_buf,m_sensors[i]->getValue());
    }
    sprintf(l_buf,"%s;\n\r",l_buf);
    m_serial.printf("%s",l_buf);

}