CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = 

SOURCES = src/main.cpp src/SenseMain.cpp src/network.cpp src/neuron.cpp
HEADERS = src/SenseMain.h src/network.h src/neuron.h

EXECUTABLE = sensebot

all: $(EXECUTABLE) test

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

test: test.cpp $(HEADERS)
	$(CC) $(CFLAGS) test.cpp src/SenseMain.cpp src/network.cpp src/neuron.cpp -o test

clean:
	rm -f $(EXECUTABLE) test

run: $(EXECUTABLE)
	./$(EXECUTABLE)

run_test: test
	./test
