
##need to install picamera[array] or any other libraries that give an error
from picamera.array import PiRGBArray
from picamera import PiCamera
import socket
import time
import cv2
import pickle
import struct

#create a default socket
s = socket.socket()
#connect with the server, make sure the server is runnig on 
#the same host and port
s.connect(('localhost',8888))

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))

# allow the camera to warmup
time.sleep(0.1)
 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
    image = frame.array
	# show the frame
	# clear the stream in preparation for the next frame
    rawCapture.truncate(0)
    #transform the frame in bytes that can be sent over network
    data = pickle.dumps(image)
    #send the length of the data as an unsigned long type
    #then the actual data
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




