CC := gcc
OBJS := poc.o asm.s
TARGET := meltdown

all : $(TARGET)
	
$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean : 
	rm -rf $(TARGET) $(TARGET).o
