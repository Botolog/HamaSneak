import socket
import keyboard
from time import sleep as wait


def check_keys():
    PRESSED = ""
    if keyboard.is_pressed("w"):
        PRESSED += "F"
    if keyboard.is_pressed("a"):
        PRESSED += "L"
    if keyboard.is_pressed("s"):
        PRESSED += "B"
    if keyboard.is_pressed("d"):
        PRESSED += "R"
    if PRESSED == "":
        PRESSED = "N"
    if keyboard.is_pressed("q"):
        PRESSED = "/"
    return PRESSED

    # time.sleep(1)  # Wait for 1 second


HOST = input("input the IP: ")  # Replace with ESP32's IP address
PORT = 80

pastInput = ""

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:
        userInput = check_keys() + str(";")
        if userInput != pastInput:
            s.sendall(userInput.encode())
            pastInput = userInput
            wait(0.2)
