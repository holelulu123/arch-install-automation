import cv2

x_finish = 
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
