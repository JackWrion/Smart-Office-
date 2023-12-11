import face_recognition
import cv2
import numpy as np


#generative learning trilemma
    

# Get a reference to webcam #0 (the default one)
# video_capture = cv2.VideoCapture(0)

# # Load a sample picture and learn how to recognize it.
# obama_image = face_recognition.load_image_file("obama.jpg")
# obama_face_encoding = face_recognition.face_encodings(obama_image)[0]

# # Load a second sample picture and learn how to recognize it.
# biden_image = face_recognition.load_image_file("tin.png")
# biden_face_encoding = face_recognition.face_encodings(biden_image)[0]



    


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











# Initialize some variables
face_locations = []
face_encodings = []
#process_this_frame = True


def face_recognition_exe(rgb_small_frame):
    
    # Find all the faces and face encodings in the current frame of video
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
    
    
    
    # Display the resulting image
    
    return ret_names, rgb_small_frame
    # Hit 'q' on the keyboard to quit!
    # if cv2.waitKey(1) & 0xFF == ord('q'):
    #     break
# Release handle to the webcam
# video_capture.release()
# cv2.destroyAllWindows()