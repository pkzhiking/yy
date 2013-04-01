CC = g++
CFLAGS = -g -ggdb3
SOURCE_FILES = main.cpp Util.cpp
main: $(SOURCE_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lmysqlclient
clean:
	rm main

