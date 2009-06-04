# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = /Users/smarr/Projects/PhD/VMs/gtest-1.3.0

USER_DIR = $(SRC_DIR)
TEST_DIR = $(ROOT_DIR)/test

# Flags passed to the preprocessor.
CPPFLAGS += -I$(GTEST_DIR) -I$(GTEST_DIR)/include -I$(USER_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -g

TEST_SRC = $(wildcard $(UNITTEST_DIR)/*.cpp)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

INCLUDES += -I$(GTEST_DIR)/include

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

unittests : $(UT_BUILD_DIR)/unittest_runner
	$(UT_BUILD_DIR)/unittest_runner

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(UT_BUILD_DIR)/gtest-all.o : $(UT_BUILD_DIR) $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc \
	-o $(UT_BUILD_DIR)/gtest-all.o

$(UT_BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc \
	-o $(UT_BUILD_DIR)/gtest_main.o

$(UT_BUILD_DIR)/gtest.a : $(UT_BUILD_DIR)/gtest-all.o
	echo Target $@
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

$(UT_BUILD_DIR)/gtest_main.a : $(UT_BUILD_DIR)/gtest-all.o $(UT_BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

$(UT_BUILD_DIR)/unittest_runner : $(CSOM_OBJ) $(TEST_OBJ) $(UT_BUILD_DIR)/gtest.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(UT_BUILD_DIR):
	mkdir $(UT_BUILD_DIR)
