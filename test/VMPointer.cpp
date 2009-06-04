#include <gtest/gtest.h>
#include "../src/vmobjects/VMPointer.h"

TEST(TestCase, Name) {
  EXPECT_EQ(1, 1);
  
  
}


TEST(ActorId, Create) {
  VMObject O1(0);
  VMPointer<VMObject> pO1(O1);
  
  
}