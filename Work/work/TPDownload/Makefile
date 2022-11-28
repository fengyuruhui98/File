#CC=arm-linux-gcc-V.4.9.3

CC=arm-linux-gcc
OBJS=main.o

1:TPdownload
TPdownload: $(OBJS)
	$(CC) -o TPdownload $(OBJS)
	
main.o: main.c
	$(CC) -c main.c -o main.o
	
clean:
	rm -f *.o TPdownload