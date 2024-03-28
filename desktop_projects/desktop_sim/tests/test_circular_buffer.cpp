#include <gtest/gtest.h>

#include "cpp_circular_buffer.h"

TEST(CircularBuffer, OverwriteCatch) {
  CircularBuffer<uint32_t, 5> cb;
  ASSERT_EQ(cb.count(), 0);
  uint32_t vals[10];
  for (int i = 5; i < 10; ++i) {
    vals[i] = 255;
  }
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(cb.enqueue(i), true);
  }
  // Buffer is:
  // [0, 1, 2, 3, 4, X]
  ASSERT_EQ(cb.peek(vals, 5), true);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vals[i], i);
  }
  for (int i = 5; i < 10; ++i) {
    ASSERT_EQ(vals[i], 255);
  }
  ASSERT_EQ(cb.dequeue(2), true);
  // Buffer is:
  // [X, X, 2, 3, 4, X]
  ASSERT_EQ(cb.enqueue(5), true);
  ASSERT_EQ(cb.enqueue(6), true);
  // Buffer is:
  // [6, X, 2, 3, 4, 5]
  ASSERT_EQ(cb.peek(vals, 5), true);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vals[i], i + 2);
  }
  // Validate that our other values were not overwritten
  for (int i = 5; i < 10; ++i) {
    ASSERT_EQ(vals[i], 255);
  }
}

TEST(CircularBuffer, GeneralTest) {
  CircularBuffer<uint32_t, 5> cb;
  ASSERT_EQ(cb.count(), 0);
  ASSERT_EQ(cb.full(), false);
  ASSERT_EQ(cb.dequeue(1), false);
  ASSERT_EQ(cb.enqueue(5), true);
  ASSERT_EQ(cb.count(), 1);
  ASSERT_EQ(cb.full(), false);
  uint32_t val;
  ASSERT_EQ(cb.peek(&val, 1), true);
  ASSERT_EQ(val, 5);
  ASSERT_EQ(cb.dequeue(1), true);
  ASSERT_EQ(cb.enqueue(0), true);
  ASSERT_EQ(cb.count(), 1);
  ASSERT_EQ(cb.full(), false);
  ASSERT_EQ(cb.peek(&val, 1), true);
  ASSERT_EQ(val, 0);

  ASSERT_EQ(cb.enqueue(1), true);
  ASSERT_EQ(cb.count(), 2);
  ASSERT_EQ(cb.enqueue(2), true);
  ASSERT_EQ(cb.count(), 3);
  ASSERT_EQ(cb.enqueue(3), true);
  ASSERT_EQ(cb.count(), 4);
  ASSERT_EQ(cb.enqueue(4), true);
  ASSERT_EQ(cb.count(), 5);
  ASSERT_EQ(cb.enqueue(5), false);
  ASSERT_EQ(cb.full(), true);
  uint32_t vals[5];
  ASSERT_EQ(cb.peek(vals, 5), true);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vals[i], i);
  }
  ASSERT_EQ(cb.dequeue(2), true);
  ASSERT_EQ(cb.count(), 3);
  ASSERT_EQ(cb.enqueue(5), true);
  ASSERT_EQ(cb.count(), 4);
  ASSERT_EQ(cb.enqueue(6), true);
  ASSERT_EQ(cb.count(), 5);

  ASSERT_EQ(cb.peek(vals, 5), true);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vals[i], i + 2);
  }
  cb.flush();
  ASSERT_EQ(cb.count(), 0);

  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(cb.enqueue(i), true);
  }
  ASSERT_EQ(cb.peek(vals, 5), true);
  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(vals[i], i);
  }

  cb.flush();
  ASSERT_EQ(cb.count(), 0);
}

TEST(CircularBuffer, StressTest) {
  uint32_t vals[2000];
  CircularBuffer<uint32_t, 1787> cb;
  for (int i = 0; i < 1000; ++i) {
    for (int j = 0; j < i; ++j) {
      ASSERT_EQ(cb.enqueue(j), true);
    }
    ASSERT_EQ(cb.count(), i);
    ASSERT_EQ(cb.peek(vals, i), true);
    for (int j = 0; j < i; ++j) {
      ASSERT_EQ(vals[j], j);
    }
    ASSERT_EQ(cb.dequeue(i), true);
    ASSERT_EQ(cb.count(), 0);
  }
}
TEST(CircularBuffer, AverageTest){
  CircularBuffer<uint32_t,5> cb;
  for (int i = 1; i < 6; ++i){
    ASSERT_EQ(cb.enqueue(i),true);
  }
  ASSERT_EQ(cb.get_avg(),3);
  ASSERT_EQ(cb.dequeue(1),true);
  ASSERT_EQ(cb.enqueue(6),true);
  ASSERT_EQ(cb.get_avg(),4);
  ASSERT_EQ(cb.dequeue(1),true);
  ASSERT_EQ(cb.enqueue(7),true);
  ASSERT_EQ(cb.get_avg(), 5);
  ASSERT_EQ(cb.dequeue(2),true);
  ASSERT_EQ(cb.enqueue(23),true);
  ASSERT_EQ(cb.enqueue(9),true);
  ASSERT_EQ(cb.get_avg(),10);
}

TEST(CircularBuffer, MinimumTest){
  CircularBuffer<int32_t,10> cb;
  for (int i = 1; i < 6; ++i){
    ASSERT_EQ(cb.enqueue(i),true);
  }
  ASSERT_EQ(cb.get_min(),1);

  ASSERT_EQ(cb.dequeue(2),true);
  ASSERT_EQ(cb.get_min(),3);

  ASSERT_EQ(cb.enqueue(-5),true);
  ASSERT_EQ(cb.get_min(),-5);

  for (int i = -10; i < -5; i ++){
    ASSERT_EQ(cb.enqueue(i),true);
  }
  ASSERT_EQ(cb.get_min(),-10);
}

TEST(CircularBuffer, MaximumTest){
  CircularBuffer<int32_t,10> cb;
  for (int i = 0; i < 10; ++i){
    if (i % 2 ==0){
      ASSERT_EQ(cb.enqueue(i),true);
    }
    else{
      ASSERT_EQ(cb.enqueue(- 1 * i),true);
    }
  }
  ASSERT_EQ(cb.get_max(),8);
  ASSERT_EQ(cb.dequeue(9),true);
  ASSERT_EQ(cb.enqueue(10),true);
  ASSERT_EQ(cb.get_max(),10);
  ASSERT_EQ(cb.enqueue(2),true);
  ASSERT_EQ(cb.enqueue(20),true);
  ASSERT_EQ(cb.enqueue(-1),true);
  ASSERT_EQ(cb.get_max(),20);
}

TEST(CircularBuffer, MedianTestAlreadySorted){
    CircularBuffer<int32_t,10> cb;
    for (int i = 0; i < 10; ++i){
    
      ASSERT_EQ(cb.enqueue(i),true);
    }
  ASSERT_EQ(cb.get_med(),4.5);
}

TEST(CircularBuffer, MedianTest){
  CircularBuffer<int32_t,10> cb;
  int arr [] = {4,5,2,6,9,1,3};
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i ++){
    ASSERT_EQ(cb.enqueue(arr[i]),true);
  }

  ASSERT_EQ(cb.get_med(),4);
  ASSERT_EQ(cb.enqueue(10),true);
  ASSERT_EQ(cb.get_med(),4.5);

  ASSERT_EQ(cb.enqueue(1),true);
  ASSERT_EQ(cb.get_med(),4);

  ASSERT_EQ(cb.dequeue(1),true);
  ASSERT_EQ(cb.get_med(),4);

  ASSERT_EQ(cb.dequeue(1),true);
  ASSERT_EQ(cb.get_med(),3);

  // 2 6 9 1 3 10 1


  ASSERT_EQ(cb.enqueue(8),true);
  ASSERT_EQ(cb.enqueue(5),true);
  ASSERT_EQ(cb.enqueue(3),true);

  // 2 6 9 1 3 10 1 8 5 3

  ASSERT_EQ(cb.get_med(),4);

  ASSERT_EQ(cb.dequeue(2),true);

  //  9 1 3 10 1 8 5 3
  ASSERT_EQ(cb.get_med(),4);
}
TEST(CircularBuffer,SumTest){
  CircularBuffer<int32_t,10> cb;
  for (int i = 0; i < 5; i ++){
    ASSERT_EQ(cb.enqueue(i),true);
  }
  // 0 1 2 3 4
  ASSERT_EQ(cb.get_sum(),10);
  ASSERT_EQ(cb.dequeue(2),true);
  // 2 3 4
  ASSERT_EQ(cb.get_sum(),9);
}