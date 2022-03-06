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
    static const int buf_size = 10;
    // global object
    ArdBuffer<int, buf_size> buffer;
    TEST_ASSERT_EQUAL_INT(buf_size, buffer.GetRemaining());
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

    // done
    UNITY_END();  // stop unit testing
}

void loop()
{
    // never reached
}
