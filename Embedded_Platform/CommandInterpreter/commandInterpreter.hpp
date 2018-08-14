/**
  ******************************************************************************
  * @file    CommandInterpreter.hpp
  * @author  RBRO/PJ-IU
  * @version V1.0.0
  * @date    day-month-2017
  * @brief   This file contains the class declaration for the command interpreter
  *          functionality.
  ******************************************************************************
 */

/* Inclusion guard */
#ifndef COMMAND_INTERPRETER_HPP
#define COMMAND_INTERPRETER_HPP

/* The mbed library */
#include <mbed.h>
#include "MOVE.hpp"
#include "Queue.hpp"

//!  CTimer class. 
/*! 
 *  It is used for implementing the interpreter of commands received by Nucleo.
 * */
class CCommandInterpreter
{
public:
    /* Constuctor */
    CCommandInterpreter(MOVE& f_car);
    /* Interpret character */
    inline void interpretChar(unsigned char f_c);
    /* Execute command */
    inline void executeCommand();
private:
    /* Reset */
    inline void reset();
    /* Interpret command */
    inline void intepretCommand();
    /* buffer */
    CQueue<unsigned char, 7> m_buffer;
    /* reference to MOVE object */
    MOVE& m_car;
    unsigned char m_commandID;
    /* command value */
    float m_commandValue;
    /* speed value */
    float m_speedValue;
    /* angle value */
    float m_angleValue;
};

#endif