CXX = g++

OFLAGS = -O2
STD = -std=c++17
WFLAGS = -Wall
CXXFLAGS = $(OFLAGS) -pthread $(STD)

TARGET = Projekt

all: $(TARGET)

compile: $(TARGET).o ObjectFactory.o OneElevatorManager.o IPhysicalCabin.o DefaultCabin.o \
		DefaultPerson.o DefaultPeopleGenerator.o DefaultFloor.o FloorQueue.o

$(TARGET): $(TARGET).o ObjectFactory.o OneElevatorManager.o IPhysicalCabin.o DefaultCabin.o \
		DefaultPerson.o DefaultPeopleGenerator.o DefaultFloor.o FloorQueue.o

$(TARGET).o: $(TARGET).cpp ObjectFactory.h IElevatorManager.h

ObjectFactory.o: ObjectFactory.cpp ObjectFactory.h DefaultCabin.h DefaultFloor.h DefaultPeopleGenerator.h DefaultPerson.h OneElevatorManager.h

OneElevatorManager.o: OneElevatorManager.cpp OneElevatorManager.h 

IPhysicalCabin.o: IPhysicalCabin.cpp IPhysicalCabin.h ObjectFactory.h

DefaultCabin.o: DefaultCabin.cpp DefaultCabin.h ObjectFactory.h

DefaultPerson.o: DefaultPerson.cpp DefaultPerson.h ObjectFactory.h

DefaultPeopleGenerator.o: DefaultPeopleGenerator.cpp DefaultPeopleGenerator.h ObjectFactory.h

DefaultFloor.o: DefaultFloor.cpp DefaultFloor.h

FloorQueue.o: FloorQueue.cpp FloorQueue.h

clean:
	rm -rf $(TARGET) *.o *.out
