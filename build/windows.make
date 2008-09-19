#!/usr/bin/env make -f

#
#  Based on Makefile by Tobias Pape
#
#
# Copyright (c) 2007 Michael Haupt, Tobias Pape
# Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
# http://www.hpi.uni-potsdam.de/swa/
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

CC			=g++
CFLAGS		=-Wall $(DBG_FLAGS) $(INCLUDES)
LDFLAGS		=$(LIBRARIES)

INSTALL		=install

CSOM_LIBS	=
CORE_LIBS	=-lm

CSOM_NAME	=CPPSOM
CORE_NAME	=SOMCore

############ global stuff -- overridden by ../Makefile

ROOT_DIR	?= $(PWD)/..
SRC_DIR		?= $(ROOT_DIR)/src
BUILD_DIR   ?= $(ROOT_DIR)/build
DEST_DIR	?= $(ROOT_DIR)/build.out

ST_DIR		?= $(ROOT_DIR)/Smalltalk
EX_DIR		?= $(ROOT_DIR)/Examples
TEST_DIR	?= $(ROOT_DIR)/TestSuite

############# "component" directories


COMPILER_DIR 	= $(SRC_DIR)/compiler
INTERPRETER_DIR = $(SRC_DIR)/interpreter
MEMORY_DIR 		= $(SRC_DIR)/memory
MISC_DIR 		= $(SRC_DIR)/misc
VM_DIR 			= $(SRC_DIR)/vm
VMOBJECTS_DIR 	= $(SRC_DIR)/vmobjects

COMPILER_SRC	= $(wildcard $(COMPILER_DIR)/*.cpp)
COMPILER_OBJ	= $(COMPILER_SRC:.cpp=.o)
INTERPRETER_SRC	= $(wildcard $(INTERPRETER_DIR)/*.cpp)
INTERPRETER_OBJ	= $(INTERPRETER_SRC:.cpp=.o)
MEMORY_SRC		= $(wildcard $(MEMORY_DIR)/*.cpp)
MEMORY_OBJ		= $(MEMORY_SRC:.cpp=.o)
MISC_SRC		= $(wildcard $(MISC_DIR)/*.cpp)
MISC_OBJ		= $(MISC_SRC:.cpp=.o)
VM_SRC			= $(wildcard $(VM_DIR)/*.cpp)
VM_OBJ			= $(VM_SRC:.cpp=.o)
VMOBJECTS_SRC	= $(wildcard $(VMOBJECTS_DIR)/*.cpp)
VMOBJECTS_OBJ	= $(VMOBJECTS_SRC:.cpp=.o)

MAIN_SRC		= $(wildcard $(SRC_DIR)/*.cpp)
#$(SRC_DIR)/Main.cpp
MAIN_OBJ		= $(MAIN_SRC:.cpp=.o)
#$(SRC_DIR)/main.o

############# primitives location etc.

PRIMITIVES_DIR	= $(SRC_DIR)/primitives
PRIMITIVES_SRC	= $(wildcard $(PRIMITIVES_DIR)/*.cpp)
PRIMITIVES_OBJ	= $(PRIMITIVES_SRC:.cpp=.o)

############# include path

INCLUDES		=-I$(SRC_DIR)
LIBRARIES		=-L$(ROOT_DIR)

##############
############## Collections.

CSOM_OBJ		=  $(MEMORY_OBJ) $(MISC_OBJ) $(VMOBJECTS_OBJ) \
				$(COMPILER_OBJ) $(INTERPRETER_OBJ) $(VM_OBJ) $(PRIMITIVES_OBJ) $(MAIN_OBJ) 

OBJECTS			= $(CSOM_OBJ) 

SOURCES			=  $(COMPILER_SRC) $(INTERPRETER_SRC) $(MEMORY_SRC) \
				$(MISC_SRC) $(VM_SRC) $(VMOBJECTS_SRC)  \
				$(PRIMITIVES_SRC) $(MAIN_SRC)

############# Things to clean

CLEAN			= $(OBJECTS) \
				$(DIST_DIR) $(DEST_DIR) CORE
############# Tools

#OSTOOL			= $(BUILD_DIR)/ostool.exe

#
#
#
#  metarules
#

.SUFFIXES: .pic.o

.PHONY: clean clobber test

all: $(CSOM_NAME).exe 
#\
#	$(CSOM_NAME).dll \
#	CORE


debug : DBG_FLAGS=-DDEBUG -g
debug: all

profiling : DBG_FLAGS=-g -pg
profiling : LDFLAGS+=-pg
profiling: all


.c.pic.o:
	$(CC) $(CFLAGS) -g -c $< -o $*.pic.o


clean:
	rm -Rf $(CLEAN)



#
#
#
# product rules
#

$(CSOM_NAME).exe: $(CSOM_OBJ)
	@echo Linking $(CSOM_NAME) loader
	$(CC) $(LDFLAGS) \
		-o $(CSOM_NAME).exe $(CSOM_OBJ)

#$(CSOM_NAME).dll: $(CSOM_OBJ)
#	@echo Linking $(CSOM_NAME) Dynamic Library
#	$(CC) $(LDFLAGS) \
#		-o $(CSOM_NAME).dll 
#	@echo CSOM done.
#
#CORE: $(CSOM_NAME).dll $(PRIMITIVES_OBJ)
#	@echo Linking SOMCore lib
#	$(CC) $(LDFLAGS) \
#		-o $(CORE_NAME) \
#		$(PRIMITIVES_OBJ) \
#		$(CORE_LIBS) -l$(CSOM_NAME)
#	mv $(CORE_NAME) $(ST_DIR)
#	@touch CORE
#	@echo SOMCore done.

install: all
	@echo installing CSOM into build
	$(INSTALL) -d $(DEST_DIR)
	$(INSTALL) $(CSOM_NAME) $(DEST_DIR)
	@echo CSOM.
	cp -Rpf $(ST_DIR) $(EX_DIR) $(TEST_DIR)  $(DEST_DIR)
	@echo Library.
	@echo done.

#
# test: run the standard test suite
#
test: install
	@(cd $(DEST_DIR); \
	./$(CSOM_NAME).exe -cp Smalltalk TestSuite\\TestHarness.som;)

#
# bench: run the benchmarks
#
bench: install
	@(cd $(DEST_DIR); \
	./$(CSOM_NAME).exe -cp Smalltalk Examples\\Benchmarks\\All.som;)
