# Makefile for C++ programs
# v1.4-TUT
# Made by Maxime Chretien

CXX=g++
CXXFLAGS=-Wall -O -std=c++17
LDFLAGS=
DEPFLAGS=
TARGET=main
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
DEPS=$(OBJS:.o=.d)

.PHONY: all clean mrproper

all: $(TARGET)

exe: $(TARGET)
	./$(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEPFLAGS)

clean:
	rm -f *.o core *.d

mrproper: clean
	rm -f $(TARGET)

-include $(DEPS)
