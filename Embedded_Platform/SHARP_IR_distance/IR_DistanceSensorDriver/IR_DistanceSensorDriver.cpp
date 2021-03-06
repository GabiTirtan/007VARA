/**
  ******************************************************************************
  * @file    IR_DistanceSensorDriver.cpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    08-January-2018
  * @brief   This file contains the class implementation for the SHARP IR sensor read
  *          methods.
  ******************************************************************************
 */

#include "IR_DistanceSensorDriver.hpp"

/** \brief  Class constructor

    Constructor method

    \param[in] _ain_pin       analog entrance pin
    \param[in] _en_pin        digital enable pin
    \param[in] f_sensor_list  the list of sensor objects
    \param[in] f_serial       serial communication object      
 */
C_IR_DistanceSensorDriver::C_IR_DistanceSensorDriver(AnalogIn _ain_pin, DigitalOut _en_pin)
    : ain_pin(_ain_pin)
    , en_pin(_en_pin)
    , value(0)
{
};

/** \brief  Class destructor

    Destructor method
 */
C_IR_DistanceSensorDriver::~C_IR_DistanceSensorDriver()
{
};

/** \brief  Method for enabling sensor

    \param[in] none
    \return     none
 */
void  C_IR_DistanceSensorDriver::enable(){
    en_pin=1;
}

/** \brief  Method for disabling sensor

    \param[in] none
    \return     none
 */
void  C_IR_DistanceSensorDriver::disable(){
    en_pin=0;
}
/*
void  C_IR_DistanceSensorDriver::setValue(float val){
    this->value=val;
}
*/

/** \brief  Method for getting value read from sensor

    \param[in] none
    \return     value read from sensor
 */
float  C_IR_DistanceSensorDriver::getValue(){
    return this->value;
}

/** \brief  Method for getting scaled value read from sensor

    \param[in] none
    \return     scaled value read from sensor
 */
float C_IR_DistanceSensorDriver::ReadDistance()
{
    // read only if sensor enabled
    if (this->en_pin == 1){
        //read analog pin
        float current_value=ain_pin.read();
        //-scaling
        float scaled_value =   C_IR_DistanceSensorDriver_A * pow(current_value,5)
                                + C_IR_DistanceSensorDriver_B * pow(current_value,4)
                                + C_IR_DistanceSensorDriver_C * pow(current_value,3)
                                + C_IR_DistanceSensorDriver_D * pow(current_value,2)
                                + C_IR_DistanceSensorDriver_E * current_value
                                + C_IR_DistanceSensorDriver_F;
                                
        //float scaled_value=current_value; // without scaling, gross signal
        this->value=scaled_value;
        return this->value;
    }
    else return 0;
}