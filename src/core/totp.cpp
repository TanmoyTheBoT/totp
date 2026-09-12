/**
 * @file totp.cpp
 * @brief Time-based One-Time Password (TOTP) implementation
 *
 * Implements RFC 6238 - TOTP: Time-Based One-Time Password Algorithm
 * https://tools.ietf.org/html/rfc6238
 *
 * TOTP is an extension of HOTP that uses the current time as the counter.
 * The algorithm generates a unique code every time period (usually 30 seconds).
 */

#include "totp.h"
#include "base32.h"
#include "hotp.h"

#include <ctime>

namespace {
    /**
     * @brief Get the current time-based counter value
     *
     * The counter is calculated as: floor(Unix_Time / Time_Step)
     *
     * Example:
     *   If current time is 1609459200 (2021-01-01 00:00:00)
     *   and period is 30 seconds:
     *   counter = 1609459200 / 30 = 53648640
     *
     * @param period Time step in seconds (usually 30)
     * @return Current counter value
     */
    uint64_t getCurrentCounter(uint64_t period) {
        // Get current Unix timestamp (seconds since 1970-01-01 00:00:00 UTC)
        std::time_t currentTime = std::time(nullptr);

        // Divide by time period to get counter
        // This ensures the counter only changes every 'period' seconds
        return static_cast<uint64_t>(currentTime) / period;
    }
}

/**
 * @brief Generate a TOTP code for the current time
 *
 * This is the main function users call to generate OTP codes.
 *
 * Process:
 *   1. Decode Base32 secret into binary key
 *   2. Calculate current time-based counter
 *   3. Generate HOTP code using counter
 *
 * @param secret Base32-encoded secret key (shared between client and server)
 * @param digits Number of digits in the OTP code (6 or 8)
 * @param period Time step in seconds (30 or 60 seconds)
 * @return Generated OTP code as string
 *
 * @example
 *   std::string code = TOTP::generate("JBSWY3DPEHPK3PXP", 6, 30);
 *   // Returns something like "123456"
 */
std::string TOTP::generate(
    const std::string& secret,
    int digits,
    uint64_t period)
{
    // Step 1: Convert Base32 secret string into binary key
    // Base32 encoding is used because it's case-insensitive and URL-safe
    // Example: "JBSWY3DPEHPK3PXP" -> [0x48, 0x65, 0x6C, 0x6C, 0x6F, ...]
    std::vector<uint8_t> key = base32::decode(secret);

    // Step 2: Calculate current time-based counter
    // RFC 6238: T = (Current Unix time - T0) / X
    // where T0 = 0 (epoch) and X = time step (period)
    uint64_t counter = getCurrentCounter(period);

    // Step 3: Generate HOTP code using the counter
    // HOTP uses HMAC-SHA1 to generate a secure code from the key and counter
    return HOTP::generate(key, counter, digits);
}

/**
 * @brief Verify a TOTP code with clock drift tolerance
 *
 * Because client and server clocks may not be perfectly synchronized,
 * we check multiple time windows to account for clock drift.
 *
 * Process:
 *   1. Calculate current counter
 *   2. Check codes for (current - window) to (current + window)
 *   3. Return true if any match
 *
 * @param secret Base32-encoded secret key
 * @param code User-provided OTP code to verify
 * @param digits Number of digits (6 or 8)
 * @param period Time step in seconds
 * @param window Number of time steps to check (usually 1)
 *               window=1 means check: previous, current, and next time step
 * @return true if code is valid, false otherwise
 *
 * @example
 *   bool valid = TOTP::verify("JBSWY3DPEHPK3PXP", "123456", 6, 30, 1);
 *   // Checks codes for times: t-30s, t, t+30s
 */
bool TOTP::verify(
    const std::string& secret,
    const std::string& code,
    int digits,
    uint64_t period,
    int window)
{
    // Decode the secret key from Base32
    std::vector<uint8_t> key = base32::decode(secret);

    // Get current time-based counter
    uint64_t counter = getCurrentCounter(period);

    // Check multiple time windows to handle clock drift
    //
    // Example with window=1:
    //   i=-1: Check previous 30 seconds (t-30)
    //   i=0:  Check current 30 seconds (t)
    //   i=1:  Check next 30 seconds (t+30)
    //
    // This allows for up to 30 seconds of clock drift
    for (int i = -window; i <= window; i++) {
        // Calculate counter for this time window
        uint64_t testCounter = counter + i;

        // Generate TOTP code for this counter
        std::string generatedCode = HOTP::generate(key, testCounter, digits);

        // Compare with user-provided code
        if (generatedCode == code) {
            return true;  // Valid code found
        }
    }

    // No matching code found in any time window
    return false;
}
