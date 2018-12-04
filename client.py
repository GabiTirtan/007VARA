
##need to install picamera[array] or any other libraries that give an error
from picamera.array import PiRGBArray
from picamera import PiCamera
import socket
import time
import cv2
import pickle
import struct

#create a default socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#connect with the server, make sure the server is runnig on 
#the same host and port
s.connect(('192.168.1.6',8888))     #asta e adresa mea locala pe reteaua wifi de la sedu, se poate schimba

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
stop_sign_cascade = cv2.CascadeClassifier('stop_sign.xml')  #load the classifier used for stop signs

# allow the camera to warmup
time.sleep(0.1)
last_time_stamp = time.time()
curr_time = time.time()
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
    image = frame.array
	# show the frame
	# clear the stream in preparation for the next frame
    rawCapture.truncate(0)
    #transform the frame in bytes that can be sent over network
    #data = pickle.dumps(image)
    gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    stops_gray = stop_sign_cascade.detectMultiScale(gray,1.3,5)
    for(x,y,w,h) in stops_gray:
        cv2.rectangle(gray,(x,y),(x+w,y+h),(0,255,0),2)
    data = pickle.dumps(gray)
    #send the length of the data as an unsigned long type
    #then the actual data
    curr_time = time.time()
    duration = curr_time - last_time_stamp
    last_time_stamp = curr_time
    fps = 1/duration
    print(fps)
    s.sendall(struct.pack("L",len(data)) + data)



#import socket
#import cv2
#import pickle
#import struct


#cap = cv2.VideoCapture(0)
#s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#s.connect(('localhost',8888))

#while True:
#      ret,frame = cap.read()
#       data = pickle.dumps(frame)
#       s.send(struct.pack("L",len(data))+data)




