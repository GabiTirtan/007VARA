"""Simple example showing how to get gamepad events."""

from __future__ import print_function


from inputs import get_gamepad
import threading
import socket
import struct

FLAG_SPEED = 0
FLAG_ANGLE = 1
FLAG_STOP = -1
class Controller(threading.Thread):
    def __init__(self,host,port,speedLow,speedHigh,angleLow,angleHigh):
        super().__init__()
        self.angle = 0
        self.speed = 0
        self.speedLow = speedLow
        self.speedHigh = speedHigh
        self.angleLow = angleLow
        self.angleHigh = angleHigh
        self.is_running = True
        self.host = host
        self.port = port
        self.address = (self.host,self.port)
        self.s = socket.socket()
        self.s.connect(self.address)
        self.file = self.s.makefile('wb')
        
    def stop(self):
        self.is_running = False
        self.writeFlag(FLAG_STOP)


    def slope(self, y1,y2,x1,x2):
        return (y2 - y1)/ (x2 - x1)

    def map(self,x,in1,in2,out1,out2):
        sl = self.slope(in1,in2,out1,out2)
        return (x - in1)/sl + out1

    def send(self,flag,number):
        self.writeFlag(flag)
        self.writeNumber(number)

    def writeFlag(self,flag):
        data = struct.pack('i',flag)
        #self.s.sendall(data)
        self.file.write(data)
        self.file.flush()

    def writeNumber(self,number):
        data = struct.pack('i',number)
        #self.s.sendall(data)
        self.file.write(data)
        self.file.flush

    def run(self):
        print("Thread started")
        while self.is_running:
            events = get_gamepad()
            for event in events:
                #print(event.ev_type,event.code,event.state)
                if (event.ev_type,event.code,event.state) == ('Key','BTN_EAST', 1):
                    print('close')
                    self.send(FLAG_STOP,0)
                    self.stop()
                elif event.ev_type == 'Absolute':
                    if event.code == 'ABS_X':               #ANGLE
                        X_coord = event.state
                        temp = int(self.map(X_coord,-1 << 15, 1 << 15, self.angleLow, self.angleHigh))
                        if self.angle != temp:
                            print("new angle:",temp)
                            self.angle = temp
                            self.send(FLAG_ANGLE,self.angle)
                    elif event.code == 'ABS_RZ':        #right trigger
                        Y_coord = event.state
                        temp = int(self.map(Y_coord,0, 255, self.speedLow, self.speedHigh))
                        if self.speed != temp:
                            if temp < self.speedHigh/2:
                                temp = 0
                            print("new speed:",temp)
                            self.speed = temp
                            self.send(FLAG_SPEED,self.speed)
                    elif event.code == 'ABS_Z':        #left trigger
                        Y_coord = event.state
                        temp = int(self.map(Y_coord,255, 0, -self.speedHigh, self.speedLow))
                        if self.speed != temp:
                            if temp > -self.speedHigh/2:
                                temp = 0
                            print("new speed:",temp)
                            self.speed = temp
                            self.send(FLAG_SPEED,self.speed)
                    # elif event.code == 'ABS_Y':             #moving left stick up 'SPEED'
                    #     Y_coord = event.state
                    #     temp = int(self.map(Y_coord,-1 << 15, 1 << 15, -20, 20))
                    #     if self.speed != temp:
                    #         print("new speed:",temp)
                    #         self.speed = temp
                    #         self.send(FLAG_SPEED,self.speed)



def main():
    """Just print out some event infomation when the gamepad is used."""
    while 1:
        events = get_gamepad()
        for event in events:
            print(event.ev_type,event.code,event.state)
    control = Controller('localhost',8888,0,40,-40,40)
    control.start()
    control.join()          


if __name__ == "__main__":
    main()