import SerialHandler
import threading
import serial
import time
import sys
import SaveEncoder
import curses


global serialHandler
# A=0.3386615784101943-0.5865790603779275j
# B=0.8997744537076356-0.26262039077918475j
# C=0.8997744537076356+0.26262039077918475j
# D=0.3386615784101943+0.5865790603779275j
# timer=4.0

A = 0.0+0.0j
B = 0.0+0.3333j
C = 0.0+0.66j
D = 0.0+1.0j
timer = 5.0


def testBezier(serialHandler):
    # Event test
    ev1 = threading.Event()
    serialHandler.readThread.addWaiter("SPLN", ev1)
    time.sleep(2.0)
    print("Start state0")
    # sent=serialHandler.sendBezierCurve(0.5051175777578528+0.5051175777578528j,0.7840863128094306+0.22614884270627506j,0.7840863128094306-0.22614884270627506j,0.5051175777578528-0.5051175777578528j,3.0,False)
    sent = serialHandler.sendBezierCurve(A, B, C, D, timer, False)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Response was received!")
        else:
            raise ConnectionError('Response', 'Response was not received!')

        ev1.clear()
        isConfirmed = ev1.wait(timer+0.5)
        if(isConfirmed):
            print("Terminated")
        else:
            raise ConnectionError('Response', 'Response was not received!')

    else:
        print("Sending problem")
    print("END_TEST")
    serialHandler.readThread.deleteWaiter("SPLN", ev1)
    time.sleep(0.0)


def testMOVEAndBrake(serialHandler):
    # Event test
    pwm = -10.0
    e = SaveEncoder.SaveEncoder("Encoder%.2f" % pwm+".csv")
    e.open()

    ev1 = threading.Event()
    ev2 = threading.Event()
    serialHandler.readThread.addWaiter("MCTL", ev1, e.save)
    serialHandler.readThread.addWaiter("BRAK", ev1, e.save)
    serialHandler.readThread.addWaiter("ENPB", ev2, e.save)

    time.sleep(1.0)
    print("Start moving")
    sent = serialHandler.sendMove(pwm,-22.0)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Moving was confirmed!")
        else:
            raise ConnectionError('Response', 'Response was not received!')

    else:
        print("Sending problem")
    time.sleep(3.0)
    ev1.clear()
    print("Start braking")
    sent = serialHandler.sendBrake(0.0)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Braking was confirmed!")
        else:
            raise ConnectionError('Response', 'Response was not received!')
    else:
        print("Sending problem")

    time.sleep(1.0)
    print("END_TEST")
    serialHandler.readThread.deleteWaiter("BRAK", ev1)
    serialHandler.readThread.deleteWaiter("MCTL", ev1)
    serialHandler.readThread.deleteWaiter("ENPB", ev2)
    e.close()


def testBrake(serialHandler):
    # Event test
    ev1 = threading.Event()
    serialHandler.readThread.addWaiter("BRAK", ev1)
    time.sleep(2.0)

    print("Start moving")
    for i in range(0, 8):
        if i % 2 == 0:
            sent = serialHandler.sendBrake(-20.0)
        else:
            sent = serialHandler.sendBrake(20.0)
        if sent:
            isConfirmed = ev1.wait(timeout=1.0)
            if(isConfirmed):
                print("Response was received!")
            else:
                raise ConnectionError('Response', 'Response was not received!')
        else:
            print("Sending problem")
        time.sleep(1)
        ev1.clear()

    time.sleep(1)
    # sent=serialHandler.sendBrake(0.0)
    if sent:
        ev1.wait()
        print("Confirmed")
    serialHandler.readThread.deleteWaiter("MCTL", ev1)


def testPid(SerialHandler):
    ev1 = threading.Event()
    serialHandler.readThread.addWaiter("PIDA", ev1)
    sent = serialHandler.sendPidActivation(True)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Response was received!")
        else:
            raise ConnectionError('Response', 'Response was not received!')

    else:
        print("Sending problem")
    serialHandler.readThread.deleteWaiter("PIDA", ev1)


def testSafetyStop(SerialHandler):
    ev1 = threading.Event()
    serialHandler.readThread.addWaiter("SFBR", ev1)
    sent = serialHandler.sendSafetyStopActivation(True)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Response was received!")
        else:
            raise ConnectionError('Response', 'Response was not received!')

    else:
        print("Sending problem")
    serialHandler.readThread.deleteWaiter("SFBR", ev1)


def testDistancePub(SerialHandler):
    ev1 = threading.Event()
    serialHandler.readThread.addWaiter("DSPB", ev1)
    sent = serialHandler.sendDistanceSensorsPublisher(False)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Response was received!")
        else:
            raise ConnectionError('Response', 'Response was not received!')

    else:
        print("Sending problem")
    serialHandler.readThread.deleteWaiter("DSPB", ev1)


def testPidValue(SerialHandler):
    ev1 = threading.Event()
    serialHandler.readThread.addWaiter("PIDS", ev1)
    # (kp,ki,kd,tf)=(2.8184,7.0832,0.28036,0)
    (kp, ki, kd, tf) = (0.93143, 2.8, 0.0, 0.0001)
    sent = serialHandler.sendPidValue(kp, ki, kd, tf)
    if sent:
        isConfirmed = ev1.wait(timeout=1.0)
        if(isConfirmed):
            print("Response was received!")
        else:
            raise ConnectionError('Response', 'Response was not received!')

    else:
        print("Sending problem")
    serialHandler.readThread.deleteWaiter("PIDS", ev1)


#def main():
#    # Initiliazation
#    global serialHandler
#    serialHandler = SerialHandler.SerialHandler("COM7")
#    serialHandler.startReadThread()

#    testMOVEAndBrake(serialHandler)
#    # testBrake(serialHandler)
#    time.sleep(0.5)
#    serialHandler.close()

def main(win):
    # Initiliazation
    global serialHandler
    serialHandler = SerialHandler.SerialHandler()
    serialHandler.startReadThread()

    #testPid(serialHandler)
    #testPidValue(serialHandler)

    pwm = 0.0
    angle = 0.0
    while true:
        try:                 
            key = win.getkey()         
            win.clear()   
            
            if pwm >= 10.0:
                pwm -= 2.5
            if pwm <= 10.0:
                pwm += 2.0

            if angle > 0:
                angle -= 2.0
            if angle < 0:
                angle += 2.0


            if str(key) is 'w':
                if (pwm < 20.0):
                    pwm += 4.0
            if str(key) is 's':
                if (pwm > -20.0):
                    pwm -= 4.0
            if str(key) is 'a':
                if (angle > -20):
                    angle -= 4.0
            if str(key) is 'd':
                if (angle < 20):
                    angle += 4.0
            if str(key) is 'q':
                serialhandler.sendbrake(angle)
                break
            serialhandler.sendmove(pwm, angle)
        except exception as e:
           # no input   
           pass

    time.sleep(0.5)
    serialHandler.close()

curses.wrapper(main)

if __name__ == "__main__":
    main()
