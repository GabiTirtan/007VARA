import socket
import sys
import pickle
import struct
import cv2
import time

host = '192.168.1.6'
port = 8888

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print('socket created')

try:
    s.bind((host,port))
except socket.error as msg:
    print ('Bind failed, Error code:' + str(msg[0]) + ', message: ' + str(msg[1])) 
    sys.exit()

print ('Socket bind succesfull')

s.listen(2)
print ('Socket now listening')

con,addr = s.accept()
data = b''
size = struct.calcsize("L")#the size of an unsigned long which will be received through the network

last_time_stamp = time.time()
curr_time = time.time()


while 1:
    while len(data) < size:     #get bytes from the network until we received the length of the data
        data += con.recv(4096)
    packed_msg_size = data[:size] #save the length of the message
    data = data[size:]#clear the data variable
    msg_size = struct.unpack("L", packed_msg_size)[0] #unpack the size of the data
    while len(data) < msg_size:#read the data
        data += con.recv(4096)
    frame_data = data[:msg_size] #put into frame the number of bytes we think the message has
    data = data[msg_size:] #clear the part that is already stored in frame_data

    frame = pickle.loads(frame_data)#transform the dumped frame into the actual frame
    cv2.imshow("server",frame)


    curr_time = time.time()
    duration = curr_time - last_time_stamp
    last_time_stamp = curr_time
    fps = 1/duration
    print(fps)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break;
