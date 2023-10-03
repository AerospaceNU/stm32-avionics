#include <gtest/gtest.h>

#include "cpp_circular_buffer.h"

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
