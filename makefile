CXX = g++
CXXFLAGS = -Wall -g -std=c++11 

all: logging_server logging_client

logging_server: logging_server.o
	$(CXX) $(CXXFLAGS) -o  logging_server logging_server.o -lbsd 

logging_client: logging_client.o
	$(CXX) $(CXXFLAGS) -o logging_client logging_client.o
