app:server.o
	g++ $< -o app
Client:client.o
	g++ -g -Wall -I/usr/local/include -o Client client.cpp -lpthread

client.o:client.cpp
	g++ -c $< -o client.o

server.o:server.cpp
	g++ -c $< -o server.o

.PHONY:clean
clean:
	rm *.o
	rm  Client
