from video_stream import video_server
from gamepad import gamepad
import cv2


def main():
    print('video...')
    video = video_server.video_server('192.168.100.21',8888)
    control = gamepad.Controller('192.168.100.205',8889,0,30,-40,40)
    print('gamepad connected')
    video.start()
    control.start()
    try:
        control.join()
        video.join()
    except (KeyboardInterrupt, SystemExit):
        cv2.destroyAllWindows()
        control.close()
        video.close()


if __name__ == '__main__':
    main()