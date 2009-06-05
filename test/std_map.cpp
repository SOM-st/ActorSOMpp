/*
 *  std_map.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <map>
#include <string>

#include <gtest/gtest.h>

TEST(Understand, arrayAccess) {
  std::map<char, char*> map;
  
  EXPECT_EQ(NULL, map['g']);
  map['g'] = "foo";
  
  EXPECT_STREQ("foo", map['g']);
  EXPECT_STRNE("sfoo", map['g']);
  
  EXPECT_EQ(NULL, map['m']);
  EXPECT_EQ(2, map.size());
}

