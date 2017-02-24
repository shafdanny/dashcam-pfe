import socket
import time
import picamera
import sys
import datetime as dt

with picamera.PiCamera() as camera:
    camera.resolution = (640, 480)
    camera.framerate = 24
    camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    camera.annotate_background = True
    # Accept a single connection and make a file-like object out of it
    connection = sys.stdout
    try:
        camera.start_recording(connection, format='h264')
        while True:
            camera.annotate_text = dt.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            camera.wait_recording(1)
    finally:
        camera.stop_recording()
        connection.close()
