import socket
mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
mysock.connect(('localhost', 8080))
# cmd = 'GET / HTTP/1.1\r\nHost: localhost\r\nHost: localhost\r\n\r\n'.encode()
cmd = 'GET / HTTP/1.1\r\nHost: localhost\r\n\r\n'.encode()
mysock.send(cmd)
while True:
    data = mysock.recv(466)
    if(len(data) < 1):
        break
    print(data.decode())
mysock.close()
