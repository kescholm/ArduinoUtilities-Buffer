
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

#pragma once

#include <cstdlib>
#include <cstdint>

/**
 * @defgroup   ArdBuffer Circular buffer
 * @brief      Circular buffer
 *
 * @{
 *
 */

enum class eArdBufferStatus
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
    kArdBufferEmpty,
    /**
     * @brief Adding to buffer overwrote existing data
     */
    kArdBufferOverwrite
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
     * @brief Add value to buffer if there is space available.
     *
     * @param push_value New value to add to buffer.
     * @return Status of operation.
     */
    eArdBufferStatus Push(BufferType push_value);

    /**
     * @brief Add value to buffer and overwrite oldest value if buffer is full.
     *
     * @param push_value New value to add to buffer.
     * @return Status of operation.
     */
    eArdBufferStatus ForcePush(BufferType push_value);

    /**
     * @brief Get oldest value from buffer.
     *
     * @param popped_value Oldest value in buffer.
     * @return Status of operation.
     */
    eArdBufferStatus Pop(BufferType &popped_value);

    /**
     * @brief Reset buffer to empty state.
     *
     */
    void Reset();

    /**
     * @brief Get number of filled buffer positions.
     *
     * @return Number of filled buffer positions.
     */
    size_t GetFilled() const
    {
        size_t filled_size = 0;
        if (m_full)
        {
            filled_size = m_size;
        }
        else
        {
            filled_size = (m_tail > m_head ? (m_size - m_tail) + m_head : m_head - m_tail);
        }
        return filled_size;
    }

    /**
     * @brief Get number of remaining buffer positions.
     *
     * @return Number of remaining buffer positions.
     */
    size_t GetRemaining() const
    {
        size_t remaining_size = 0;
        if (m_full)
        {
            remaining_size = 0;
        }
        else
        {
            remaining_size = (m_tail > m_head ? m_tail - m_head : m_size - (m_head - m_tail));
        }
        return remaining_size;
    }

    /**
     * @brief Get total buffer size.
     *
     * @return Total buffer size.
     */
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
    eArdBufferStatus status = eArdBufferStatus::kArdBufferSuccess;
    const size_t remaining_size = GetRemaining();
    if (remaining_size == 0)
    {
        status = eArdBufferStatus::kArdBufferFull;
    }
    else
    {
        // push to head of queue
        m_buffer[m_head] = push_value;
        m_head = (m_head + 1) % m_size;
        m_full = (m_head == m_tail);
    }
    return status;
}

template<typename BufferType, size_t BufferSize>
inline eArdBufferStatus ArdBuffer<BufferType, BufferSize>::ForcePush(BufferType push_value)
{
    eArdBufferStatus status = eArdBufferStatus::kArdBufferSuccess;
    const size_t remaining_size = GetRemaining();
    if (remaining_size == 0)
    {
        m_buffer[m_head] = push_value;
        m_head = (m_head + 1) % m_size;
        m_tail = m_head;
        status = eArdBufferStatus::kArdBufferOverwrite;
    }
    else
    {
        // push to head of queue
        m_buffer[m_head] = push_value;
        m_head = (m_head + 1) % m_size;
        m_full = (m_head == m_tail);
    }
    return status;
}

template<typename BufferType, size_t BufferSize>
inline eArdBufferStatus ArdBuffer<BufferType, BufferSize>::Pop(BufferType &popped_value)
{
    eArdBufferStatus status = eArdBufferStatus::kArdBufferSuccess;
    const size_t filled_size = GetFilled();
    if (filled_size == 0)
    {
        status = eArdBufferStatus::kArdBufferEmpty;
    }
    else
    {
        // consume at tail of queue
        popped_value = m_buffer[m_tail];
        // advance
        m_tail = (m_tail + 1) % m_size;
        m_full = false;
    }
    return status;
}

template<typename BufferType, size_t BufferSize>
inline void ArdBuffer<BufferType, BufferSize>::Reset()
{
    m_head = 0;
    m_tail = 0;
    m_full = false;
}
