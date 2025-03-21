import cv2
import time

def main():
    # Initialize video capture object with the first webcam
    cap = cv2.VideoCapture(0)

    # Check if the webcam is opened correctly
    if not cap.isOpened():
        print("Cannot open camera")
        return

    # Set video frame size and frame rate (adjust as necessary)
    frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    frame_rate = 30  # Frames per second

    # Define the codec and create VideoWriter object for MP4
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter('output.mp4', fourcc, frame_rate, (frame_width, frame_height))

    # Add a delay for camera to initialize properly
    time.sleep(2)

    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()

        # If frame is read correctly ret is True
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break

        # Write the frame into the file
        out.write(frame)

        # Display the resulting frame
        #cv2.imshow('Webcam Feed', frame)

        # Exit on pressing 'q'
        if cv2.waitKey(1) == ord('q'):
            break

    # When everything done, release the capture and writer objects
    cap.release()
    out.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

