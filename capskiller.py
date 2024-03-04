import ctypes
from time import sleep as wait 

def turn_capslock(dll):
    VK_CAPITAL = 0X14
    if dll.GetKeyState(VK_CAPITAL):
        dll.keybd_event(VK_CAPITAL, 0X3a, 0X1, 0)
        dll.keybd_event(VK_CAPITAL, 0X3a, 0X3, 0)
    return dll.GetKeyState(VK_CAPITAL)

dll = ctypes.WinDLL('User32.dll')
while True:
    try:
        turn_capslock(dll)
    except:
        print("fuck")
    wait(0.1)