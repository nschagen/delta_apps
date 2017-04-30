#!/usr/bin/python2
import sys
import time
import socket
import select
import struct
import mmap
import subprocess


client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('localhost', 9999))

data = struct.pack('<hh', 1, 0)
data = data + 'a' * 32
client_socket.send(data)

time.sleep(0.1)

data = client_socket.recv(20)
if data is None:
  print("Socket closed!")
else:
  print("recieved {} bytes: {}".format(
      len(data), 
      ':'.join(x.encode('hex') for x in data)))
