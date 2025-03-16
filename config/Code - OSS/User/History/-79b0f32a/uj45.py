import cv2

x_start = 100
x_finish = 150
y_start = 100   
y_finish = 120
cap = cv2.VideoCapture("/dev/video2", cv2.CAP_V4L2)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    cv2.imshow("MJPEG Stream", frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
