
/**
 * @file       ArdBuffer.h
 * @author     Kyle Escholm
 * @brief      Circular buffer for Arduino
 *
 * @details
 *
 * See group @ref ArdBuffer
 *
 */

#ifndef ARD_BUFFER_H
#define ARD_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup   ArdBuffer Circular buffer
 * @brief      Circular buffer
 *
 * @{
 *
 */

enum eArdBufferStatus
{
    /**
     * @brief Successful operation
     */
    kArdBufferSuccess = 0,
    /**
     * @brief Operation failed, buffer is full
     */
    kArdBufferFull,
    /**
     * @brief Operation failed, buffer is empty
     */
    kArdBufferEmpty
};

/**
 * @brief Circular Buffer
 *
 */
template<typename BufferType, size_t BufferSize>
class ArdBuffer
{
   public:
    ArdBuffer() : m_buffer(), m_tail(0), m_head(0), m_size(BufferSize), m_full(false) {}

    /**
     * @brief Add
     *
     * @param push_value
     * @return eArdBufferStatus
     */
    eArdBufferStatus Push(BufferType push_value);

    eArdBufferStatus Pop(BufferType &popped_value);

    void Reset();

    size_t GetFilled() const
    {
        if (m_full)
        {
            return m_size;
        }
        else
        {
            return (m_tail > m_head ? (m_size - m_tail) + m_head : m_head - m_tail);
        }
    }

    size_t GetRemaining() const
    {
        if (m_full)
        {
            return 0;
        }
        else
        {
            return (m_tail > m_head ? m_tail - m_head : m_size - (m_head - m_tail));
        }
    }

    size_t GetSize() const
    {
        return m_size;
    }

   private:
    /**
     * @brief Circular buffer of commands for stepper
     */
    BufferType m_buffer[BufferSize];
    /**
     * @brief Index of oldest filled buffer position
     *
     * Buffer is empty if @c m_tail is equal to @c m_head
     */
    size_t m_tail;
    /**
     * @brief Index of latest open buffer position
     *
     * Buffer is empty if @c m_tail is equal to @c m_head
     */
    size_t m_head;
    /**
     * @brief Total buffer size
     */
    size_t m_size;
    /**
     * @brief `true` if buffer is full, `false` otherwise
     */
    bool m_full;
};

// ===============================
// ArdBuffer Implementation
// ===============================

template<typename BufferType, size_t BufferSize>
inline eArdBufferStatus ArdBuffer<BufferType, BufferSize>::Push(BufferType push_value)
{
    const size_t remaining_size = GetRemaining();
    if (remaining_size == 0)
    {
        return kArdBufferFull;
    }
    else
    {
        m_buffer[m_head] = push_value;
        m_head = (m_head + 1) % m_size;
        m_full = (m_head == m_tail);
        return kArdBufferSuccess;
    }
}

template<typename BufferType, size_t BufferSize>
inline eArdBufferStatus ArdBuffer<BufferType, BufferSize>::Pop(BufferType &popped_value)
{
    const size_t filled_size = GetFilled();
    if (filled_size == 0)
    {
        return kArdBufferEmpty;
    }
    else
    {
        // consume at tail of queue
        popped_value = m_buffer[m_tail];
        // advance
        m_tail = (m_tail + 1) % m_size;
        m_full = false;
        return kArdBufferSuccess;
    }
}

template<typename BufferType, size_t BufferSize>
inline void ArdBuffer<BufferType, BufferSize>::Reset()
{
    m_head = 0;
    m_tail = 0;
    m_full = false;
}

#endif
