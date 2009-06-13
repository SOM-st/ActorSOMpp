#include <gtest/gtest.h>
#include "../src/vmobjects/VMPointer.h"
#include "../src/vmobjects/VMObject.h"
#include <stdio.h>

TEST(VMPointer, Size) {
  EXPECT_EQ(4, sizeof(VMPointer<VMObject>));
}