#!/usr/bin/python2
import socket
import struct

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('localhost', 9999))

#data = struct.pack('>ccH', 'e', 'a', 132)
data = struct.pack('<ccH', 'e', 'a', 102)
data = data + 'hello world!' + (90 * 'a');
client_socket.send(data)
data = client_socket.recv(999)
print(data)
