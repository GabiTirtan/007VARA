/**
  ******************************************************************************
  * @file    SensorTask.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-year
  * @brief   This file contains the class implementation for the sensor task
  *          functionality.
  ******************************************************************************
 */
#include "SensorTask.hpp"

/** \brief  Constructor for the CCounter class
 *
 *  Constructor method
 *
 *  \param[in] f_period       period value
 *  \param[in] f_serial       reference to the serial object
 */
examples::CDISTFTest::CDISTFTest(uint32_t        f_period
                                ,Serial&         f_serial)
    :CTask(f_period)
    ,m_serial(f_serial)
    ,m_tf()
{
    std::array<std::array<float,1>,3> l_num({std::array<float,1>({3}),std::array<float,1>({-5.897}),std::array<float,1>({2.9})});
    std::array<std::array<float,1>,3> l_den({std::array<float,1>({1}),std::array<float,1>({-1.949}),std::array<float,1>({0.9512})});
    m_tf.setNum(l_num);
    m_tf.setDen(l_den);
}

/** \brief  Method called each f_period
 * 
 *  \param[in]  None
 *  \param[out] None
 */
void examples::CDISTFTest::_run()
{
    float l_input=1.0;
    float l_output=m_tf(l_input);
    m_serial.printf("%.4f\n",l_output);
}

/** \brief  Constructor for the CCounter class
 *
 *  Constructor method
 *
 *  \param[in] f_period       period value
 *  \param[in] f_encoder      reference to encoder object
 *  \param[in] f_control      reference to the controller object
 *  \param[in] f_serial       reference to the serial object
 */
examples::CEncoderSender::CEncoderSender(uint32_t            f_period
                                        ,CEncoderFilter&     f_encoder
                                        ,task::CControl&     f_control
                                        ,Serial&             f_serial)
    :CTask(f_period)
    ,m_isActived(false)
    ,m_encoder(f_encoder)
    ,m_control(f_control)
    ,m_serial(f_serial)
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
void examples::CEncoderSender::staticSerialCallback(void* obj,char const * a, char * b){
                CEncoderSender* self = static_cast<CEncoderSender*>(obj);
                self->serialCallback(a,b);
            }

/** \brief  Serial callback actions
  *
  * Serial callback method setting controller to values received
  *
  * \param[in]  a                   string to read data from
  * \param[out] b                   string to write data to
  * \return None
  */
void examples::CEncoderSender::serialCallback(char const * a, char * b){
                int l_isActivate=0;
                uint32_t l_res = sscanf(a,"%d",&l_isActivate);
                if(l_res==1){
                    m_isActived=(l_isActivate>=1);
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
void examples::CEncoderSender::_run()
{
    if(!m_isActived) return;
    float l_filtRps=m_encoder.getRPS();
    // float l_u=m_control.get();
    // m_serial.printf("@ENPB:%.2f;%.2f;;\r\n",l_filtRps,l_u);  
    m_serial.printf("@ENPB:%.2f;;\r\n",l_filtRps);  
}                        
