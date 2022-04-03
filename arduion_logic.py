import serial
import atexit
import time

arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=1)

def send_msg(cmd):
    cmd += '\n'
    arduino.write(cmd.encode())
    arduino.flush()
    arduino.flushInput()

def receive_msg():
    while arduino.inWaiting() == 0: pass
    if arduino.inWaiting() > 0:
        answer = arduino.readline()
    return answer

def exit_handler():
    arduino.close()

atexit.register(exit_handler)