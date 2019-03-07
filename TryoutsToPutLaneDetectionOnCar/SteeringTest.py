import SerialHandler
import threading
import serial
import time
import sys
import SaveEncoder
import curses
from Lane import *

global serialHandler

if __name__ == "__main__":
    # Initiliazation
    #global serialHandler
    serialHandler = SerialHandler.SerialHandler()
    serialHandler.startReadThread()

    pwm = 0.0
    angle = 0.0

    # while get frame from camera
    
    angle = process_frame_get_angle(frame, angle)

    serialHandler.sendMove(pwm, angle)