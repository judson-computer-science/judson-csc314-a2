CC = gcc
CFLAGS = -Wall -g
EXE = encoder

$(EXE): *.c *.h
	$(CC) $(CFLAGS) -o $(EXE) main.c -lm

clean:
	rm $(EXE); rm *.wav
