import cv2
import numpy as np
x_start = 80
x_finish = 207
y_start = 62
y_finish = 104
yellow = np.full(())

cap = cv2.VideoCapture("/dev/video2", cv2.CAP_V4L2)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    frame[x_start:x_finish, y_start,y_finis, :] = [255,125,125]
    cv2.imshow("MJPEG Stream", frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
