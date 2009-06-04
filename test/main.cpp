#include <gtest/gtest.h>
#include "../src/actors/actors.h"

int main(int argc, char** argv) {
  actors_init();
  
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}