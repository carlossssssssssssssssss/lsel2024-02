#!/usr/bin/env python
"""
Node time client
"""
import socket
import sys
import argparse

import socket

host = 'localhost'
port = 50000
size = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
s.send('time'.encode())
print('El cliente pide la hora')
data = s.recv(size)
s.close()
print('Hora recibida:', data.decode())