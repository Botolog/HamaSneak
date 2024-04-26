import socket
import keyboard
import pygame
from time import sleep as wait
from time import time as now


def tupToStr(A):
    return str(f"({A[0]},{A[1]},{A[2]},{A[3]},{A[4]})")


def prepToSend(A):
    return (A + ";").encode()


def check_keys():
    AX, AY = 0, 0
    if keyboard.is_pressed("w"):
        AY += 100
    if keyboard.is_pressed("a"):
        AX += 80
    if keyboard.is_pressed("s"):
        AY -= 100
    if keyboard.is_pressed("d"):
        AX -= 80
    return (AX, AY)



def check_controller():
    global pastBX, pastBY
    pygame.event.get()
    axes = joystick.get_numaxes()

    AX = int(joystick.get_axis(0) * -10) * 10
    AY = int(joystick.get_axis(1) * -10) * 10
    if joystick.get_button(8) > 0:
        BX = 180 - int(90 * joystick.get_axis(2) + 90)
        BY = 180 - int(-45 * joystick.get_axis(3) + 45)
    else:
        BX = pastBX
        BY = pastBY
    OP = min(joystick.get_button(9) + joystick.get_button(10), 1)
    OP += joystick.get_button(15)*2
    pastBX = BX
    pastBY = BY

    return (AX, AY, BX, BY, OP)


def d(a, b):
    delta = 0
    for i in range(4):
        delta += abs(a[i] - b[i])
    delta += abs(a[4] - b[4]) * 50
    return delta > 10

def GET_IP():
    return "http://" + socket.gethostbyname("E7470") + ":8080/"

pygame.init()
pygame.joystick.init()
clock = pygame.time.Clock()


pastBX = 0
pastBY = 0

pastKeyInput = (0, 0)
pastConInput = (0, 0, 0, 0, 0)

HOST = input("input the IP: ")  # Replace with ESP32's IP address
PORT = 80
lastSend = 0

# if True:
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(prepToSend(GET_IP()))
    if pygame.joystick.get_count() > 0:
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        axes = joystick.get_numaxes()
        while True:
            conInput = check_controller()
            if d(conInput, pastConInput) or (now() - lastSend>1):
                print(tupToStr(conInput))
                s.sendall(prepToSend(tupToStr(conInput)))
                lastSend = now()
                pastConInput = conInput
                wait(0.1)
    else:
        while True:
            keyInput = check_keys()
            if d(keyInput, pastKeyInput):
                s.sendall(prepToSend(tupToStr(keyInput)))
                pastKeyInput = keyInput
                wait(0.2)
