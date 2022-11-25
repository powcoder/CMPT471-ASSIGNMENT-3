
all: server 
server: echoServer.c
	gcc -o server echoServer.c

client: tcpEchoClient.c
	gcc -o client tcpEchoClient.c

clean:
	rm server client