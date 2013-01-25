#****************************************************************************
#
# Makefile for huawei static code analyzer package
# Guo Jiuliang 
# epcc.sjtu.edu.cn
#
PLUGIN_SOURCE_FILES= tree_printer
# This is a GNU make (gmake) makefile
#****************************************************************************

ARCH:=$(shell uname -m)
BIT32:=i686
BIT64:=x86_64

EXE: PLUGIN_OBJECT_FILES plugin.so
	
	ls --color
GCCINSTALL_DIR =/usr/gcc-build
GCCDOWNLOAD_DIR =/root/Downloads/gcc-4.6.0
GMPINSTALL_DIR =/usr/local/gmp
TESTDIR = $(PWD)/test
GCC = $(GCCINSTALL_DIR)/bin/gcc
STDLIBPATH= $(GCCINSTALL_DIR)/lib
ifeq ($(ARCH),$(BIT32))
CC1PLUS = $(GCCINSTALL_DIR)/libexec/gcc/i686-pc-linux-gnu/4.6.0/cc1plus
else
CC1PLUS = $(GCCINSTALL_DIR)/libexec/gcc/x86_64-unknown-linux-gnu/4.6.0/cc1plus
endif
G++ = $(GCCINSTALL_DIR)/bin/g++

CC_FLAGS =
MAKE_MACROS = DUMMY
OBJECT_GCC_PLUGIN_SRC_FOLDER = gcc-plugin
CORE_SRC_FOLDER = core
UTIL_SRC_FOLDER = util
ANALYZER_SRC_FOLDER = analyzer

PLUGIN_SOURCE_FILES= $(OBJECT_GCC_PLUGIN_SRC_FOLDER)/plugin-api.cpp  $(OBJECT_GCC_PLUGIN_SRC_FOLDER)/gcc-genericize.c $(OBJECT_GCC_PLUGIN_SRC_FOLDER)/gcc-tree-walker.c $(OBJECT_GCC_PLUGIN_SRC_FOLDER)/ApplicationDelegate.cpp $(CORE_SRC_FOLDER)/tree/ASTTreeBuilder.cpp $(CORE_SRC_FOLDER)/tree/ASTTree.cpp  $(UTIL_SRC_FOLDER)/Logger.cpp  $(CORE_SRC_FOLDER)/SrcManager.cpp $(UTIL_SRC_FOLDER)/Util.cpp    $(CORE_SRC_FOLDER)/tree/ASTTreePrinter.cpp
 

PLUGIN_OBJECT_FILES= bin/plugin-api.o bin/gcc-genericize.o bin/ApplicationDelegate.o bin/gcc-tree-walker.o bin/ASTTreeBuilder.o bin/ASTTree.o  bin/Logger.o  bin/SrcManager.o  bin/Util.o  bin/ASTTreePrinter.o


OBJECT_GCC_PLUGIN_SRC_FOLDER = gcc-plugin
ifeq ($(ARCH), $(BIT64))
GCCPLUGINS_INCLUDE_FLAGS := -I$(GCCINSTALL_DIR)/lib/gcc/x86_64-unknown-linux-gnu/4.6.0/plugin/include/cp/ -I$(GCCINSTALL_DIR)/lib/gcc/x86_64-unknown-linux-gnu/4.6.0/plugin/include/   -I$(GCCDOWNLOAD_DIR)/gcc -I$(GMPINSTALL_DIR) -I$(PWD)/gcc-plugin/include  -I$(PWD)/core/include  -I$(PWD)/analyzer/include -I$(PWD)/util/include -I$(PWD)/gcc-plugin 
else
GCCPLUGINS_INCLUDE_FLAGS := -I$(GCCINSTALL_DIR)/lib/gcc/i686-pc-linux-gnu/4.6.0/plugin/include/cp/ -I$(GCCINSTALL_DIR)/lib/gcc/i686-pc-linux-gnu/4.6.0/plugin/include/   -I$(GCCDOWNLOAD_DIR)/gcc -I$(GMPINSTALL_DIR) -I$(PWD)/gcc-plugin/include  -I$(PWD)/core/include  -I$(PWD)/analyzer/include -I$(PWD)/util/include -I$(PWD)/gcc-plugin 
endif

CFLAGS+= -I$(GCCPLUGINS_DIR)/include $(GCCPLUGINS_INCLUDE_FLAGS) -fPIC -g 

PLUGIN_OBJECT_FILES:
	rm -f ./bin/*.o
	$(GCC) $(CC_FLAGS) -c -D$(MAKE_MACROS) $(CFLAGS) $(PLUGIN_SOURCE_FILES) 
	mv *.o ./bin/

plugin.so: $(PLUGIN_OBJECT_FILES) 
	$(GCC) $(CC_FLAGS) -L$(STDLIBPATH) -lstdc++ -shared ./bin/*.o -o plugin.so
install:
	cp plugin.so `gcc -print-file-name=plugin`/plugin.so

#project directory to recursively collect source code files under it
PROJ_DIR =
C_FILES = $(shell find $(PROJ_DIR) -regex ".*\.c")
CPP_FILES = $(shell find $(PROJ_DIR) -regex ".*\.cpp")
PROJ_SOURCE_FILES = $(CPP_FILES) $(C_FILES)

#run:
#	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH)
#	$(foreach file, $(PROJ_SOURCE_FILES), $(shell $(G++) -fsyntax-only  -iplugindir=/root/plugin-dev/ -D_GNU_SOURCE $(file) -c -dumpbase -mtune=generic -march=pentiumpro -fplugin=$(PWD)/plugin.so))
#INPUT_FILES =
INPUT_FILE =
ifeq ($(ARCH), $(BIT32))
MARCH = "-march=pentiumpro"
else
MARCH =
endif
run:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH)
	echo $(INPUT_FILE)
#	$(foreach file, $(INPUT_FILES), $(shell $(G++) -fsyntax-only  -iplugindir=/root/plugin-dev/ -D_GNU_SOURCE $(file) -c -dumpbase -mtune=generic -march=pentiumpro -fplugin=$(PWD)/plugin.so))
	$(shell $(G++) -fsyntax-only  -iplugindir=$(PWD) -D_GNU_SOURCE $(INPUT_FILE) -c -dumpbase -mtune=generic $(MARCH) -fplugin=$(PWD)/plugin.so)


		
	echo $(HTML_TARGET_DIR)

.PHONY: all
all:
	make
.PHONY: debug
debug:
	make MAKE_MACROS=PLUGIN_DEBUG=1
.PHONY: clean
clean:
	find . -name \*.o -exec rm {} \;
	find . -name \*.printer -exec rm {} \;
	rm plugin.so
