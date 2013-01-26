CC = g++
CFLAGS = -g
CURRENT_DIR = $(shell pwd)
TARGET = $(CURRENT_DIR)/binary
SOURCE1 = $(CURRENT_DIR)/analyzer
SOURCE2 = $(CURRENT_DIR)/core
SOURCE3 = $(CURRENT_DIR)/core/tree
SOURCE4 = $(CURRENT_DIR)/util
SOURCE5 = $(CURRENT_DIR)/core/analyzer
main:
	$(CC) $(CFLAGS) -c *.cpp; mv *.o $(TARGET); cd $(SOURCE1); $(CC) $(CFLAGS) -c *.cpp; mv *.o $(TARGET); cd $(SOURCE2); $(CC) $(CFLAGS) -c *.cpp; mv *.o $(TARGET); cd $(SOURCE3); $(CC) $(CFLAGS) -c *.cpp; mv *.o $(TARGET); cd $(SOURCE4); $(CC) $(CFLAGS) -c *.cpp; mv *.o $(TARGET); cd $(SOURCE5); $(CC) $(CFLAGS) -c *.cpp; mv *.o $(TARGET); cd $(TARGET); $(CC) $(CFLAGS) -o $@ *.o -lmysqlclient -ldl -lpthread -L/usr/lib/mysql -L/usr/local/mysql/lib/mysql

