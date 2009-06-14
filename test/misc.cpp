/*
 *  misc.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 14/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <gtest/gtest.h>

#include <map>
#include <bitset>

#include "../src/memory/ObjectTable.h"

typedef std::bitset<NUMBER_OF_ACTORS> ActorSet, *pActorSet;

TEST(Understanding, bitest) {
  std::map<ObjectTable::Index, pActorSet> externalReferences;
  
  ObjectTable::Index idx;
  
  EXPECT_EQ(NULL, externalReferences[idx]);
  
  externalReferences[idx] = new ActorSet();
  
  EXPECT_FALSE((*externalReferences[idx])[1]);
  
  (*externalReferences[idx])[1] = true;
  
  EXPECT_EQ(true, (*externalReferences[idx])[1]);
}
