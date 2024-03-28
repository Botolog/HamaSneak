import socket
import keyboard
import pygame
from time import sleep as wait


def tupToStr(A):
    return str(f"({A[0]},{A[1]},{A[2]},{A[3]})")


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


def check_axes():
    pygame.event.get()
    axes = joystick.get_numaxes()

    AX = int(joystick.get_axis(0) * -10) * 10
    AY = int(joystick.get_axis(1) * -10) * 10
    BX = 180-int(90 * joystick.get_axis(2) + 90)
    BY = 180-int(-45 * joystick.get_axis(3) + 45)
    return (AX, AY, BX, BY)


def d(a, b):
    delta =0
    for i in range(4):
        delta += abs(a[i] - b[i])
    return delta > 10


pygame.init()
pygame.joystick.init()
clock = pygame.time.Clock()

pastKeyInput = (0, 0)
pastConInput = (0, 0, 0, 0)

HOST = input("input the IP: ")  # Replace with ESP32's IP address
PORT = 80

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    if pygame.joystick.get_count() > 0:
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        axes = joystick.get_numaxes()
        while True:
            conInput = check_axes()
            if d(conInput, pastConInput):
                s.sendall(prepToSend(tupToStr(conInput)))
                print(tupToStr(conInput))
                pastConInput = conInput
                wait(0.1)
    else:
        while True:
            keyInput = check_keys()
            if d(keyInput, pastKeyInput):
                s.sendall(prepToSend(tupToStr(keyInput)))
                pastKeyInput = keyInput
                wait(0.2)
