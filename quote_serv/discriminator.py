#!/usr/bin/python2
import socket
import struct

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('localhost', 9999))

# It will truncate the response to 98 chars, because it anticipates that the
# last character (at 98) contains a character that needs to be escaped.
client_socket.send("a" * 99)
data = client_socket.recv(999)
print(data)
