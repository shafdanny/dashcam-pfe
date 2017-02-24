#!/bin/bash

python video-streaming.py | ./encryption | python pi-server-socket.py