import cv2
import time
from ultralytics import YOLO
import paho.mqtt.client as mqtt
from mqttConfig import MQTT_USERNAME, MQTT_PASSWORD

def main():

    # mqtt_config
    mqtt_broker = "mqtt.cetools.org"
    mqtt_port = 1884
    topic_laptop = "student/CASA0016/ucfntl0/Laptop"
    topic_phone = "student/CASA0016/ucfntl0/Cellphone"
    topic_total_Screen_Usage = "student/CASA0016/ucfntl0/Total"

    # Connect to MQTT Broker
    client = mqtt.Client()
    # Set Username and Passwords
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.connect(mqtt_broker, mqtt_port, 60)
    client.loop_start()

    # Load the YOLOv8 model
    model = YOLO('yolov8n.pt')

    # # Open the video file
    # video_path = "http://192.168.1.177:81/stream"
    # # video_path = "http://10.129.119.87:81/stream" 
    # cap = cv2.VideoCapture(video_path)

    # Initialize the timers and flags
    start_time_computer = None
    start_time_phone = None
    computer_in_frame = False
    phone_in_frame = False
    total_duration_computer = 0
    total_duration_phone = 0
    total_duration_screen = 0
    last_update_time = time.time()
    update_interval = 2

    while True:
        try:
            # Open the video file
            video_path = "http://10.129.119.87:81/stream"
            cap = cv2.VideoCapture(video_path)

            while cap.isOpened():
                success, frame = cap.read()
                if success:
                    results = model(frame)

                    # Reset the flags
                    computer_in_frame = False
                    phone_in_frame = False

                    for result in results:
                        if result.boxes:
                            # Check if there is any screen
                            for box in result.boxes:
                                cls_index = int(box.cls)
                                label = result.names[cls_index]

                                if label == 'laptop':
                                    computer_in_frame = True
                                    if start_time_computer is None:
                                        start_time_computer = time.time()
                                elif label == 'cell phone':
                                    phone_in_frame = True
                                    if start_time_phone is None:
                                        start_time_phone = time.time()

                    # Update screen usage and publish the time
                    current_time = time.time()
                    if current_time - last_update_time >= update_interval:
                        if computer_in_frame:
                            total_duration_computer += int(current_time - start_time_computer)  # Make the time an integer
                            start_time_computer = current_time  # Reset the timer
                        if phone_in_frame:
                            total_duration_phone += int(current_time - start_time_phone)  # Make the time an integer
                            start_time_phone = current_time  # Reset the timer

                        # # Update total screen usage
                        # if computer_in_frame or phone_in_frame:
                        #     total_duration_screen += int(current_time - last_update_time)  # Make the time an integer

                        client.publish(topic_laptop, f"Total computer usage duration: {total_duration_computer} seconds")
                        client.publish(topic_phone, f"Total phone usage duration: {total_duration_phone} seconds")
                        # client.publish(topic_total_Screen_Usage, f"Total screen usage duration: {total_duration_screen} seconds")

                        last_update_time = current_time

                    if cv2.waitKey(1) & 0xFF == ord("q"):
                        break
                else:
                    break

            cap.release()
            cv2.destroyAllWindows()

        except Exception as e:
            print(f"An error occurred: {e}")
            time.sleep(5)  # Retry after 5 seconds

if __name__ == "__main__":
    main()
