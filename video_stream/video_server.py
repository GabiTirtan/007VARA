import io
import socket
import threading
import cv2
import time
import numpy as np
import time

class video_server(threading.Thread):
    def __init__(self,host,port):
        super().__init__()
        self.isRunning = True
        self.stream = b''
        self.s = socket.socket()
        self.host = host
        self.port = port
        self.address = (host,port)
        self.s.bind(self.address)
        self.s.listen(0)
        (self.con, self.conAddr) = self.s.accept()
        print("Connected")
        self.file = self.con.makefile('rb')

    def run(self):
        lastTime = time.time()
        currTime = 0
        while self.isRunning:
            self.stream += self.file.read(1024)
            first = self.stream.find(b'\xff\xd8')
            last = self.stream.find(b'\xff\xd9')
            if last != -1 and first != -1:
                #print('frame received')
                jpg = self.stream[first:last + 2]
                self.stream = self.stream[last + 2:]
                frame = cv2.imdecode(np.frombuffer(jpg,dtype=np.uint8),cv2.IMREAD_COLOR)
                currTime = time.time()
                fps = 1/(currTime - lastTime)
                lastTime = currTime
                print(fps)
                cv2.imshow('frame',frame)
                cv2.waitKey(1)
            



    def stop(self):
        self.isRunning = False
        self.file.close()

def main():
    video = video_server('192.168.1.10',8888)
    video.start()
    time.sleep(100)
    video.stop()
    video.join()
    print('Done')


if __name__ == '__main__':
    main()