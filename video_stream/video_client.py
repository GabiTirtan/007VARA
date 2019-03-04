import socket
import cv2
import threading
import io

class video_client(threading.Thread):
    def __init__(self,host, port):
        super().__init__()
        self.isRunning = True
        self.frame = io.BytesIO()
        self.newFrame = False
        self.host = host
        self.port = port
        self.address = (self.host, self.port)
        self.s = socket.socket()
        self.s.connect(self.address)
        self.file = self.s.makefile('wb')

    def run(self):
        while self.isRunning:
            if self.newFrame:
                #print('sending...')
                cv2.line(self.frame,(0,0),(200,200),(0,255,0))
                retval, stream = cv2.imencode('\.jpeg',self.frame)
                self.file.write(stream)
                #print('Frame sent')
                self.newFrame = False
        print('streaming client stopped')

    def stop(self):
        self.isRunning = False
        self.file.close()

    def send(self,frame):
        self.frame = frame
        self.newFrame = True
        #print('Loaded image')


def main():
    video = video_client('localhost',8888)
    video.start()
    cap = cv2.VideoCapture(0)
    while 1:
        ret, frame = cap.read()
        print('Initialize sending')
        video.send(frame)
        cv2.imshow
        
    cap.release()
    


if __name__ == '__main__':
    main()
