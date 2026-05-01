#ifndef UTILS_HPP
# define UTILS_HPP



/**
 * @brief Retrieves the current monotonic time in milliseconds.
 *
 * @details Uses `clock_gettime` with `CLOCK_MONOTONIC` to obtain a
 *          time value that increases steadily and is not affected by
 *          system clock adjustments (e.g., NTP). This makes it suitable
 *          for measuring intervals and timeouts.
 *
 * @return The current time in milliseconds since an unspecified starting
 *         point (usually system boot) as a 64‑bit unsigned integer.
 */
uint64_t    get_time_ms(void);

/**
 * @brief Verifies if a client's deadline has passed.
 * @details Compares the client start time against the current time using the provided timeout.
 * @param start_time_ms Client start time (ms)
 * @param timeout_ms    Timeout value from config (ms)
 * @return true if elapsed time >= timeout, false otherwise.
 */
bool        verify_deadline_ms(const uint64_t &start_time_ms, const uint64_t timeout_ms);

#endif