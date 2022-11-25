https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
import sys
print (sys.version)
from socket import *
serverHost = "127.0.0.1"
serverPort = 50007
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(( serverHost, serverPort))
print ( 'The server is ready to receive ' )
while True:
        sentence, clientAddress = serverSocket.recvfrom(1024)
        print ("Message received: " , sentence )
        capitalizedSentence = sentence.decode().upper()
        serverSocket.sendto(capitalizedSentence.encode(), clientAddress)
