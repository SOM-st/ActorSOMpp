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

CC         =/opt/local/bin/g++
CFLAGS		=-Wextra -Wno-endif-labels $(DBG_FLAGS) $(INCLUDES)
#CC          =ag++
#CFLAGS		=-Wextra -Wno-write-strings -Wno-endif-labels $(DBG_FLAGS) $(INCLUDES) -D__STDC_LIMIT_MACROS --c_compiler /opt/local/bin/g++ --keep_woven
LDFLAGS		=$(LIBRARIES)

SHAREDFLAGS	=-fPIC -mmacosx-version-min=10.4 -undefined dynamic_lookup \
                -dynamiclib -Wl,-single_module -Wl,-Y,1455

INSTALL		=install

CSOM_LIBS	=
CORE_LIBS	=-lm

CSOM_NAME	=SOM++
CORE_NAME	=SOMCore
PRIMITIVESCORE_NAME  =PrimitiveCore
SHARED_EXTENSION    =dylib

              
############# Tools

#
#
#
#  metarules
#

.SUFFIXES: .pic.o .fpic.o

.PHONY: clean clobber test

all: $(CSOM_NAME)\
	$(CSOM_NAME).$(SHARED_EXTENSION) \
	$(PRIMITIVESCORE_NAME).$(SHARED_EXTENSION) \
	CORE


debug : DBG_FLAGS=-DDEBUG -g3
debug: all

release : DBG_FLAGS=-O3
release: all

profiling : DBG_FLAGS=-g -pg
profiling : LDFLAGS+=-pg
profiling: all

# pull in dependency info for *existing* .o files
-include $(CSOM_OBJ:.o=.d)

.cpp.pic.o:
	$(CC) $(CFLAGS) -fPIC -c $< -o $*.pic.o
	@$(CC) -MM $(CFLAGS) $< > $*.pic.d
	@mv -f $*.pic.d $*.pic.d.tmp
	@sed -e 's|.*:|$*.pic.o:|' < $*.pic.d.tmp > $*.pic.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.pic.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.pic.d
	@rm -f $*.pic.d.tmp


.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $*.o
	@$(CC) -MM $(CFLAGS) $< > $*.d
	@$(CC) -MM $(CFLAGS) $< > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	rm -Rf $(CLEAN) $(CSOM_OBJ:.o=.d)
	-rm -Rf $(UT_BUILD_DIR)



#
#
#
# product rules
#

$(CSOM_NAME): $(CSOM_NAME).$(SHARED_EXTENSION) $(MAIN_OBJ)
	@echo Linking $(CSOM_NAME) loader
	$(CC) $(LDFLAGS) \
		-o $(CSOM_NAME) $(MAIN_OBJ) $(CSOM_NAME).$(SHARED_EXTENSION) -ldl
	@echo loader done.

$(CSOM_NAME).$(SHARED_EXTENSION): $(CSOM_OBJ)
	@echo Linking $(CSOM_NAME) Dynamic Library
	$(CC) $(LDFLAGS) $(SHAREDFLAGS) \
		-o $(CSOM_NAME).$(SHARED_EXTENSION) $(CSOM_OBJ) $(CSOM_LIBS)
	@echo CSOM done.

$(PRIMITIVESCORE_NAME).$(SHARED_EXTENSION): $(CSOM_NAME) $(PRIMITIVESCORE_OBJ)
	@echo Linking PrimitivesCore lib
	$(CC) $(LDFLAGS) $(SHAREDFLAGS) \
		-o $(PRIMITIVESCORE_NAME).$(SHARED_EXTENSION) \
		$(PRIMITIVESCORE_OBJ)
	@touch $(PRIMITIVESCORE_NAME).$(SHARED_EXTENSION)
	@echo PrimitivesCore done.

CORE: $(CSOM_NAME) $(PRIMITIVESCORE_OBJ) $(PRIMITIVES_OBJ)
	@echo Linking SOMCore lib
	$(CC) $(LDFLAGS)  \
		$(SHAREDFLAGS) -o $(CORE_NAME).csp \
		$(PRIMITIVES_OBJ) \
		$(PRIMITIVESCORE_OBJ) \
		$(CORE_LIBS)
	mv $(CORE_NAME).csp $(ST_DIR)
	@touch CORE
	@echo SOMCore done.

install: all
	@echo installing CSOM into build
	$(INSTALL) -d $(DEST_DIR)
	$(INSTALL) $(CSOM_NAME) $(DEST_DIR)
	@echo CSOM.
	cp -Rpf $(ST_DIR) $(EX_DIR) $(TEST_DIR)  $(DEST_DIR)
	@echo Library.
	@echo done.

#
# console: start the console
#
console: all
	./$(CSOM_NAME) -cp ./Smalltalk

#
# test: run the standard test suite
#
test: all
	./$(CSOM_NAME) -cp ./Smalltalk ./TestSuite/TestHarness.som

#
# bench: run the benchmarks
#
bench: all
	./$(CSOM_NAME) -cp ./Smalltalk ./Examples/Benchmarks/All.som
