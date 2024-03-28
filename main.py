import cv2
from urllib.request import urlopen
import numpy as np

url = r"http://10.0.0.12"

while True:
    img_resp = urlopen(url)
    imgnp = np.asarray(bytearray(img_resp.read()), dtype="uint8")
    img = cv2.imdecode(imgnp, -1)
    cv2.imshow("Camera", img)
    if cv2.waitKey(1) == 113:
        break