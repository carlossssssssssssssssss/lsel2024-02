#!/usr/bin/env python

import socket
import select
from datetime import datetime
from datetime import date
import sys

"""
Timedate server
"""
host = 'localhost'
port = 50000
backlog = 5
size = 1024
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((host,port))
server.listen(backlog)
input=[server]
print('Servidor abierto')
while 1:
    inputready,ouputready,exceptready = select.select(input,[],[])

    for s in inputready:
        if s == server:
            #socket del servidor:
            client, address = server.accept()
            print('Cliente conectado')
            input.append(client)
        else:
            # sockets de clientes:
            data = s.recv(size)
            if data.decode()=="date":
                print('Lectura:',data.decode())
                fecha=date.today().strftime("%d/%m/%Y")
                s.send(fecha.encode())
                print('Envio de la fecha al cliente:',fecha)
            elif data.decode()=="time":
                print('Lectura:',data.decode())
                hora = datetime.now().strftime("%H:%M:%S")
                s.send(hora.encode())
                print('Envio de la hora al cliente:',hora)
            else:
                s.close()
                input.remove(s)

