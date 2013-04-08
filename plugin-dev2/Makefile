CC = g++
CFLAGS = -g
CURRENT_DIR = $(shell pwd)
SOURCE0 = $(CURRENT_DIR)
SOURCE_EXTEND_CUR := $(wildcard $(SOURCE0)/*.cpp)
OBJ_CUR := $(SOURCE_EXTEND_CUR:%.cpp=%.o)
TARGET = $(CURRENT_DIR)/binary

SOURCE1 = $(CURRENT_DIR)/analyzer
SOURCE1_EXTEND_CUR := $(wildcard $(SOURCE1)/*.cpp)
OBJ1 := $(SOURCE1_EXTEND_CUR:%.cpp=%.o)
SOURCE2 = $(CURRENT_DIR)/core
SOURCE2_EXTEND_CUR := $(wildcard $(SOURCE2)/*.cpp)
OBJ2 := $(SOURCE2_EXTEND_CUR:%.cpp=%.o)
SOURCE3 = $(CURRENT_DIR)/core/tree
SOURCE3_EXTEND_CUR := $(wildcard $(SOURCE3)/*.cpp)
OBJ3 := $(SOURCE3_EXTEND_CUR:%.cpp=%.o)
SOURCE4 = $(CURRENT_DIR)/util
SOURCE4_EXTEND_CUR := $(wildcard $(SOURCE4)/*.cpp)
OBJ4 := $(SOURCE4_EXTEND_CUR:%.cpp=%.o)
SOURCE5 = $(CURRENT_DIR)/core/analyzer
SOURCE5_EXTEND_CUR := $(wildcard $(SOURCE5)/*.cpp)
OBJ5 := $(SOURCE5_EXTEND_CUR:%.cpp=%.o)


OBJS := $(CURRENT_DIR)/$(OBJ_CUR) $(SOURCE1)/$(OBJ1) $(SOURCE2)/$(OBJ2) $(SOURCE3)/$(OBJ3) $(SOURCE4)/$(OBJ4) $(SOURCE5)/$(OBJ5)
MAKEDEPEND = g++ -MM

all: main
	mv main $(TARGET)
main: $(OBJ_CUR) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)
	$(CC) -o $@ $^ -lmysqlclient -ldl -lpthread -L/usr/lib/mysql -L/usr/local/mysql/lib/mysql

$(OBJ_CUR): %.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
$(OBJ1): %.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
$(OBJ2): %.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
$(OBJ3): %.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
$(OBJ4): %.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
$(OBJ5): %.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
clean:
	rm $(CURRENT_DIR)/*.o; rm $(SOURCE1)/*.o $(SOURCE2)/*.o $(SOURCE3)/*.o $(SOURCE4)/*.o $(SOURCE5)/*.o
#depend:
#	echo $(OBJ_CUR)
#	rm .depend
#	$(MAKE) .depend
#main:
#	$(CC) $(CFLAGS) -c *.cpp; cd $(SOURCE1); $(CC) $(CFLAGS) -c *.cpp; cd $(SOURCE2); $(CC) $(CFLAGS) -c *.cpp; cd $(SOURCE3); $(CC) $(CFLAGS) -c *.cpp; cd $(SOURCE4); $(CC) $(CFLAGS) -c *.cpp; cd $(SOURCE5); $(CC) $(CFLAGS) -c *.cpp; cd $(TARGET); $(CC) $(CFLAGS) -o $@ $(CURRENT_DIR)/*.o $(SOURCE1)/*.o $(SOURCE2)/*.o $(SOURCE3)/*.o $(SOURCE4)/*.o $(SOURCE5)/*.o -lmysqlclient -ldl -lpthread -L/usr/lib/mysql -L/usr/local/mysql/lib/mysql

#clean:
#	rm $(TARGET)/* -rf
