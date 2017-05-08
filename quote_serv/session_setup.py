#!/usr/bin/python2
import sys
import socket
import struct
import mmap

if len(sys.argv) < 3:
	print "Not enough arguments"
	exit(1)

fuzzed_input_file=sys.argv[1]
taint_file=sys.argv[2]

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('localhost', 9999))

taintfd=open(taint_file,"r+") # this mapping is for setting taint STOP/START tag.
taint_mm=mmap.mmap(taintfd.fileno(),0)

# Enable taint
taint_mm[0:1]='1'
taint_mm.flush()
taint_mm.seek(0)

with open(fuzzed_input_file, "r") as datafile:
	data = datafile.read()
	client_socket.send(data)

data = client_socket.recv(999)
print(data)
