CC = g++

CFLAGS = -O2 -Wall -std=c++17

TARGET = Projekt

all: $(TARGET)


$(TARGET): $(TARGET).o ObjectFactory.o OneElevatorManager.o IPhysicalCabin.o DefaultCabin.o \
		DefaultPerson.o DefaultPeopleGenerator.o DefaultFloor.o FloorQueue.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o ObjectFactory.o OneElevatorManager.o IPhysicalCabin.o \
	DefaultCabin.o DefaultPerson.o DefaultPeopleGenerator.o DefaultFloor.o FloorQueue.o

$(TARGET).o: $(TARGET).cpp ObjectFactory.h IElevatorManager.h
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).cpp ObjectFactory.h IElevatorManager.h

ObjectFactory.o: ObjectFactory.cpp ObjectFactory.h DefaultCabin.h DefaultFloor.h DefaultPeopleGenerator.h DefaultPerson.h OneElevatorManager.h
	$(CC) $(CFLAGS) -o ObjectFactory.o ObjectFactory.cpp ObjectFactory.h DefaultCabin.h DefaultFloor.h DefaultPeopleGenerator.h DefaultPerson.h OneElevatorManager.h

OneElevatorManager.o: OneElevatorManager.cpp OneElevatorManager.h 
	$(CC) $(CFLAGS) -o OneElevatorManager.o OneElevatorManager.cpp OneElevatorManager.h 

IPhysicalCabin.o: IPhysicalCabin.cpp IPhysicalCabin.h ObjectFactory.h
	$(CC) $(CFLAGS) -o IPhysicalCabin.o IPhysicalCabin.cpp IPhysicalCabin.h ObjectFactory.h

DefaultCabin.o: DefaultCabin.cpp DefaultCabin.h ObjectFactory.h
	$(CC) $(CFLAGS) -o DefaultCabin.o DefaultCabin.cpp DefaultCabin.h ObjectFactory.h

DefaultPerson.o: DefaultPerson.cpp DefaultPerson.h ObjectFactory.h
	$(CC) $(CFLAGS) -o DefaultPerson.o DefaultPerson.cpp DefaultPerson.h ObjectFactory.h

DefaultPeopleGenerator.o: DefaultPeopleGenerator.cpp DefaultPeopleGenerator.h ObjectFactory.h
	$(CC) $(CFLAGS) -o DefaultPeopleGenerator.o DefaultPeopleGenerator.cpp DefaultPeopleGenerator.h ObjectFactory.h

DefaultFloor.o: DefaultFloor.cpp DefaultFloor.h
	$(CC) $(CFLAGS) -o DefaultFloor.o DefaultFloor.cpp DefaultFloor.h

FloorQueue.o: FloorQueue.cpp FloorQueue.h
	$(CC) $(CFLAGS) -o FloorQueue.o FloorQueue.cpp FloorQueue.h

clean:
	rm -rf $(TARGET) *.o



