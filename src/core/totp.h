/**
 * @file totp.h
 * @brief Time-based One-Time Password (TOTP) algorithm
 *
 * This implements RFC 6238 which extends HOTP to use time as the counter.
 * TOTP is widely used for two-factor authentication (2FA).
 *
 * Popular services using TOTP:
 * - Google Authenticator
 * - Microsoft Authenticator
 * - Authy
 * - 1Password
 *
 * @author TanmoyTheBoT
 * @date 2026-09-12
 */

#ifndef TOTP_H
#define TOTP_H

#include <string>
#include <vector>
#include <cstdint>

/**
 * @class TOTP
 * @brief Time-based One-Time Password implementation
 *
 * TOTP generates a unique code every time period (usually 30 seconds).
 * The code is calculated using HMAC-SHA1 with the current time as input.
 *
 * Algorithm Overview:
 *   Counter = floor(Unix_Time / Time_Period)
 *   TOTP = HOTP(Secret, Counter)
 *
 * Security Properties:
 * - Codes are valid only for a short time window
 * - Even if a code is intercepted, it becomes useless after expiration
 * - Replay attacks are prevented by the time-based counter
 */
class TOTP
{
public:
    /**
     * @brief Generate a TOTP code for the current time
     *
     * This is the primary function for generating OTP codes.
     * Call this every time you need a new code.
     *
     * Parameters:
     * - secret: Shared secret key between client and server
     *           Must be Base32-encoded (A-Z, 2-7)
     *           Example: "JBSWY3DPEHPK3PXP"
     *
     * - digits: Length of the generated code
     *           Common values: 6 (most services) or 8 (high security)
     *
     * - period: Time window in seconds
     *           Common values: 30 seconds (standard) or 60 seconds
     *
     * @param secret Base32-encoded secret key
     * @param digits Number of digits in the code (typically 6 or 8)
     * @param period Time step in seconds (typically 30)
     * @return Generated OTP code as string (e.g., "123456")
     *
     * @example
     *   // Generate a standard 6-digit code with 30-second validity
     *   std::string code = TOTP::generate("JBSWY3DPEHPK3PXP", 6, 30);
     *   std::cout << "Your code: " << code << std::endl;
     *   // Output: "Your code: 123456"
     */
    static std::string generate(
        const std::string& secret,
        int digits = 6,
        uint64_t period = 30
    );

    /**
     * @brief Verify a TOTP code with clock drift tolerance
     *
     * Compares the user-provided code against codes for multiple time windows
     * to account for clock synchronization issues between client and server.
     *
     * The 'window' parameter controls how much drift is allowed:
     * - window = 0: Only check current time (strict)
     * - window = 1: Check previous, current, and next period (±30 seconds)
     * - window = 2: Check ±2 periods (±60 seconds)
     *
     * Recommendation: Use window=1 for most applications
     *
     * @param secret Base32-encoded secret key
     * @param code User-provided OTP code to verify
     * @param digits Number of digits (must match generation)
     * @param period Time step (must match generation)
     * @param window Number of periods to check before and after
     * @return true if code is valid, false otherwise
     *
     * @example
     *   // Verify code with 1-period tolerance (±30 seconds)
     *   bool isValid = TOTP::verify(
     *       "JBSWY3DPEHPK3PXP",  // Secret
     *       "123456",             // User's code
     *       6,                    // 6 digits
     *       30,                   // 30 second period
     *       1                     // ±1 period tolerance
     *   );
     *
     *   if (isValid) {
     *       std::cout << "Login successful!" << std::endl;
     *   } else {
     *       std::cout << "Invalid code!" << std::endl;
     *   }
     */
    static bool verify(
        const std::string& secret,
        const std::string& code,
        int digits = 6,
        uint64_t period = 30,
        int window = 1
    );

private:
    // This class only contains static methods
    // No instantiation needed or allowed
    TOTP() = delete;
    ~TOTP() = delete;
    TOTP(const TOTP&) = delete;
    TOTP& operator=(const TOTP&) = delete;
};

#endif // TOTP_H
