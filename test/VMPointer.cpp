#include <gtest/gtest.h>
#include "../src/vmobjects/VMPointer.h"
#include "../src/vmobjects/VMObject.h"
#include <stdio.h>

TEST(TestCase, Name) {
  EXPECT_EQ(1, 1);
  
  
}


TEST(ActorId, Create) {
  //VMObject O1(0);
  //VMPointer<VMObject> pO1(O1);
  
  printf("Size: %d", sizeof(VMPointer<VMObject>));
  
  EXPECT_EQ(8, sizeof(VMPointer<VMObject>));
  
}