https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
from socket import *
serverName = 'localhost'
serverPort = 50007
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = input('Input lowercase sentence:')
clientSocket.sendto( message.encode(), (serverName, serverPort) )
print ('message sent')
modifiedMessage, severAddress = clientSocket.recvfrom(2048)
print (modifiedMessage.decode())
clientSocket.close()
