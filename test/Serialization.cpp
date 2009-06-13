/*
 *  Serialization.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 13/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <gtest/gtest.h>

#include "../src/vmobjects/VMBigInteger.h"

TEST(VMBigInteger, Serialization) {
  char buffer[20];
  
  VMBigInteger a(0xABCDEFAB);
  a.Serialize(&buffer);
  
  EXPECT_EQ(sizeof(ImmutableTypes) + sizeof(int64_t), a.GetSerializedSize());
  
  
  ImmutableTypes* typeTag = (ImmutableTypes*)&buffer;
  EXPECT_EQ(BigIntegerTag, *typeTag);
  
  int64_t* value = (int64_t*)((intptr_t)buffer + sizeof(ImmutableTypes));
  EXPECT_EQ(0xABCDEFAB, *value);
  
  
  int64_t deserialize = 0;
  void* newBuffer = VMBigInteger::Deserialize((void*)value, deserialize);
  
  EXPECT_EQ(0xABCDEFAB, deserialize);
  
  EXPECT_EQ((intptr_t)value + sizeof(int64_t), (intptr_t)newBuffer);
}
