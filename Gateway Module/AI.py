import face_recognition
import cv2
import numpy as np

# This is a demo of running face recognition on live video from your webcam. It's a little more complicated than the
# other example, but it includes some basic performance tweaks to make things run a lot faster:
#   1. Process each video frame at 1/4 resolution (though still display it at full resolution)
#   2. Only detect faces in every other frame of video.

# PLEASE NOTE: This example requires OpenCV (the `cv2` library) to be installed only to read from your webcam.
# OpenCV is *not* required to use the face_recognition library. It's only required if you want to run this
# specific demo. If you have trouble installing it, try any of the other demos that don't require it instead.

# Get a reference to webcam #0 (the default one)
video_capture = cv2.VideoCapture(0)

# Create arrays of known face encodings and their names
known_face_encodings = []
known_face_names = []
def enroll_face_encode(raw_frame, name):
    known_face_encodings.append(  face_recognition.face_encodings(   face_recognition.load_image_file(raw_frame)       )   [0]    )
    known_face_names.append( name  )


enroll_face_encode("Tin1.jpg","Tin1")
enroll_face_encode("Tin2.jpg","Tin2")
enroll_face_encode("Tin3.jpg","Tin3")
enroll_face_encode("Thi1.jpg","Thi1")
enroll_face_encode("Thi2.jpg","Thi2")
enroll_face_encode("Thi3.jpg","Thi3")
enroll_face_encode("Thang1.jpg","Thang1")
enroll_face_encode("Thang2.jpg","Thang2")
enroll_face_encode("Thang3.jpg","Thang3")
enroll_face_encode("BeNhu.jpg","BeNhu")





# Initialize some variables
face_locations = []
face_encodings = []
face_names = []
process_this_frame = True

while True:
    # Grab a single frame of video
    ret, frame = video_capture.read()


# Resize frame of video to 1/4 size for faster face recognition processing
    small_frame = cv2.resize(frame, (0, 0), fx=0.2, fy=0.2)

    # Convert the image from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
    rgb_small_frame = np.ascontiguousarray(small_frame[:, :, ::-1])


    face_locations = face_recognition.face_locations(img=rgb_small_frame, model="hog")
    face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)
    face_names = []
    ret_names = []
    
    for face_encoding in face_encodings:
        # See if the face is a match for the known face(s)
        name = "Unknown"
        face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
        best_match_index = np.argmin(face_distances)
        if face_distances[best_match_index] < 0.5:
            name = known_face_names[best_match_index] 
            ret_names.append(name)
            name = name + "\n" + str(int((1 - face_distances[best_match_index]) *100)) + "%"
        
        face_names.append(name)


    # Display the results
    for (top, right, bottom, left), name in zip(face_locations, face_names):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        
        # Draw a box around the face
        cv2.rectangle(rgb_small_frame, (left, top), (right, bottom), (0, 0, 255), 2)
        # Draw a label with a name below the face
        cv2.rectangle(rgb_small_frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
        font = cv2.FONT_HERSHEY_DUPLEX
        cv2.putText(rgb_small_frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)
    


    # Display the results
    for (top, right, bottom, left), name in zip(face_locations, face_names):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        top *= 5
        right *= 5
        bottom *= 5
        left *= 5

        # Draw a box around the face
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        # Draw a label with a name below the face
        cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
        font = cv2.FONT_HERSHEY_DUPLEX
        cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

    # Display the resulting image
    cv2.imshow('Video', frame)

    # Hit 'q' on the keyboard to quit!
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release handle to the webcam
video_capture.release()
cv2.destroyAllWindows()