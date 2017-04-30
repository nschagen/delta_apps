#!/usr/bin/python2
import sys
import time
import struct
import socket
from socket import error as SocketError
import mmap

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('localhost', 9999))

data = struct.pack('<h', 100)
data = data + 'a' * 100

client_socket.send(data)

time.sleep(0.1)

data = None
try:
  data = client_socket.recv(1024)
except SocketError as e:
  print("Socket error!")
if data is None:
  print("Socket closed!")
else:
  print("recieved {} bytes: {}".format(
      len(data), 
      ':'.join(x.encode('hex') for x in data)))
