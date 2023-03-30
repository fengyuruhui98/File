#CC=/usr/local/arm/4.3.2/bin/arm-linux-gcc
CC=arm-linux-gcc
OBJS=main.o
LIBS=-L. -llinux2440 -lpthread -ljr1004_493   -lcrypto -lsm
#-lhardware -lpub -lapplog -ldl

1:shanghai
shanghai: $(OBJS)
	$(CC) -o shanghai $(OBJS) $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o

clean:
	rm -f *.o shanghai
