CC=arm-linux-gcc
#CC=gcc
OBJS=main.o

1:shanghai
shanghai: $(OBJS)
	$(CC) -o shanghai $(OBJS)
	
main.o: main.c
	$(CC) -c main.c -o main.o
	
clean:
	rm -f *.o shanghai