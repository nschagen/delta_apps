#!/usr/bin/python2
import sys
import socket
import select
import struct
import mmap

if len(sys.argv) < 3:
  print "Not enough arguments"
  sys.exit(1)

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

# Deliver payload
with open(fuzzed_input_file, "r") as datafile:
  data = datafile.read()
  client_socket.send(data)

client_socket.setblocking(0)
ready = select.select([client_socket], [], [], 10)
if ready[0]:
  # We expect a single character back
  data = client_socket.recv(20)
  print("Setup received response: ", data)
  sys.exit(0)
else:
  # Nothing was read, abort with error returncode such that fuzzer knows it
  # should not expect anything
  print "Setup timed out."
  sys.exit(1)
