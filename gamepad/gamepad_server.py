import socket
import struct
import threading
import time

FLAG_SPEED = 0
FLAG_ANGLE = 1
FLAG_STOP = -1

class g_server(threading.Thread):
    def __init__(self,host,port):
        super().__init__()
        self.speed = 0
        self.angle = 0
        self.data = ''      #cariable in which we will store what is received from the network
        self.port = port
        self.host = host
        self.address = (self.host,self.port)
        self.s = socket.socket()
        self.running = True
        self.s.bind((self.host,self.port))
        self.s.listen(2)
        (self.con,self.addr) = self.s.accept()
        self.file = self.con.makefile('rb')

    def run(self):
        while self.running:
            flag = self.readFlag()
            if flag == FLAG_STOP:
                print('need to stop')
                self.stop()
            temp = self.readNumber()
            #print(flag,flag == FLAG_ANGLE, flag == FLAG_SPEED)
            if flag == FLAG_SPEED:
                self.speed = temp
            elif flag == FLAG_ANGLE:
                self.angle = temp
            

    def stop(self):
        self.running = False

    def readFlag(self):
        msg = b''
        msg = self.file.read(struct.calcsize('i'))
        (flag,) = struct.unpack('i',msg)
        return flag

    def readNumber(self):
        msg = self.file.read(struct.calcsize('i'))
        (number,) = struct.unpack('i',msg)
        return number

    #function that reads the flag that tells us if the next number is the angle or the speed
    def readFlagSlow(self):
        while len(self.data) < struct.calcsize('i'):        #read from the network until we have enough data to unpack an int
            self.data = self.con.recv(struct.calcsize('i')) 
            if self.data == '':                          #if the empty string is returned, then the connection closed, so stop the server
                self.stop()
        packed_msg = self.data[:struct.calcsize('i')]
        (flag,) = struct.unpack('i',packed_msg)
        self.data = self.data[struct.calcsize('i'):]
        
        return flag

    def readNumberSlow(self):
        while len(self.data) < struct.calcsize('i'):        #read from the network until we have enough data to unpack an int
            self.data = self.con.recv(struct.calcsize('i')) 
            if self.data == '':                          #if the empty string is returned, then the connection closed, so stop the server
                self.stop()
        packed_msg = self.data[:struct.calcsize('i')]
        (number,) = struct.unpack('i',packed_msg)
        self.data = self.data[struct.calcsize('i'):]
        return number

    def getAngle(self):
        return self.angle
    
    def getSpeed(self):
        return self.speed
            
        
def main():
    server = g_server('localhost',8888)
    server.start()
    viteza = 0
    unghi = 0
    try:
        while server.is_alive():
            temp1 = server.getAngle()
            temp2 = server.getSpeed()
        # print(temp1,temp2)
            if temp1 != unghi or temp2 != viteza:
                unghi = temp1
                viteza = temp2
                print('viteza: {0} unghi: {1}'.format(viteza,unghi))
    finally:
        server.join()

if __name__ == '__main__':
    main()
