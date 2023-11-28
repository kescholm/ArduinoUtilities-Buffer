#include <Arduino.h>
#include <unity.h>

#include "ArdBuffer.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

// Tests
// -----

static void test_create_buffer()
{
    static const size_t buf_size = 10;
    // global object
    ArdBuffer<int, buf_size> buffer;
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetSize());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());

}

static void test_reset_buffer()
{
    static const size_t buf_size = 3;
    // global object
    ArdBuffer<int, buf_size> buffer;
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetSize());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());

    eArdBufferStatus status = buffer.Push(1);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    status = buffer.Push(2);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    status = buffer.Push(3);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(0, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetFilled());

    buffer.Reset();
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetSize());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());
}

static void test_fill_buffer()
{
    static const size_t buf_size = 3;
    // global object
    ArdBuffer<int, buf_size> buffer;
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetSize());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());

    eArdBufferStatus status = buffer.Push(1);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(2, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(1, buffer.GetFilled());

    status = buffer.Push(2);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(1, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(2, buffer.GetFilled());

    status = buffer.Push(3);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(0, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetFilled());

    status = buffer.Push(4);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferFull, status);
    TEST_ASSERT_EQUAL_INT(0, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetFilled());

    int popped_value = -1;
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(1, popped_value);
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(2, popped_value);
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(3, popped_value);
}

static void test_force_fill_buffer()
{
    static const size_t buf_size = 3;
    // global object
    ArdBuffer<int, buf_size> buffer;
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetSize());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());

    eArdBufferStatus status = buffer.ForcePush(1);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(2, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(1, buffer.GetFilled());

    status = buffer.ForcePush(2);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(1, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(2, buffer.GetFilled());

    status = buffer.ForcePush(3);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(0, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetFilled());

    status = buffer.ForcePush(4);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferOverwrite, status);
    TEST_ASSERT_EQUAL_INT(0, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetFilled());

    int popped_value = -1;
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(2, popped_value);
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(3, popped_value);
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(4, popped_value);
}

static void test_empty_buffer()
{
    static const size_t buf_size = 3;
    // global object
    ArdBuffer<int, buf_size> buffer;
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetSize());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());

    eArdBufferStatus status = buffer.Push(1);
    status = buffer.Push(2);
    status = buffer.Push(3);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(0, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetFilled());

    int popped_value = -1;
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(1, popped_value);
    TEST_ASSERT_EQUAL_INT(1, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(2, buffer.GetFilled());

    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(2, popped_value);
    TEST_ASSERT_EQUAL_INT(2, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(1, buffer.GetFilled());

    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferSuccess, status);
    TEST_ASSERT_EQUAL_INT(3, popped_value);
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());

    popped_value = -1;
    status = buffer.Pop(popped_value);
    TEST_ASSERT_EQUAL(eArdBufferStatus::kArdBufferEmpty, status);
    TEST_ASSERT_EQUAL_INT(-1, popped_value);
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
    TEST_ASSERT_EQUAL_INT(0, buffer.GetFilled());
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();  // IMPORTANT LINE!

    // ArdBuffer
    // ------

    RUN_TEST(test_create_buffer);
    RUN_TEST(test_reset_buffer);
    RUN_TEST(test_fill_buffer);
    RUN_TEST(test_force_fill_buffer);
    RUN_TEST(test_empty_buffer);

    // done
    UNITY_END();  // stop unit testing
}

void loop()
{
    // never reached
}
