import cv2
import io
import time
import sys
import struct
import numpy as np


file = open('imagine.jpeg','wb')
cap = cv2.VideoCapture(0)
ret,frame = cap.read()
cv2.imshow('prima',frame)
ret,ceva = cv2.imencode('\.jpeg',frame)
print(sys.getsizeof(ceva))
size = struct.pack('i',sys.getsizeof(ceva))
file.write(size)
file.write(ceva)
file.close()
file = open('imagine.jpeg','rb')
size = file.read(struct.calcsize('i'))
(size,) = struct.unpack('i',size)
print(size)
altceva  = file.read(size)
captura = cv2.imdecode(np.frombuffer(altceva, dtype=np.uint8), cv2.IMREAD_COLOR)

cv2.imshow('a doua',captura)
cv2.waitKey(0)
time.sleep(3)
cv2.release()
cv2.destroyAllWindos()
