mysocket.py:

A server socket that send the data in stdin to a client.


camerasocket.py:

Start a video recording from the camera and send the data to stdout.

To launch the sequence of camera, encryption, and socket:

python camerasocket.py | ./encryption | python mysocket.py