#CC=/usr/local/arm/4.3.2/bin/arm-linux-gcc
CC=arm-linux-gcc
OBJS=main.o
LIBS=-L. -llinux4418_bom_493 -lpthread -ljr1004  -lcrypto -lsm
#-lhardware -lpub -lapplog -ldl

1:shanghai-bom
shanghai-bom: $(OBJS)
	$(CC) -o shanghai-bom $(OBJS) $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o

clean:
	rm -f *.o
