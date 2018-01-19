CC := gcc
OBJS := meltdown.o main.o
TARGET := meltdown

all : $(TARGET)
	
$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)


meltdown.o : header.h meltdown.c

main.o : header.h main.c

clean : 
	rm -rf $(TARGET) main.o meltdown.o
