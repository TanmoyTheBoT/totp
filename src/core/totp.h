#ifndef TOTP_H
#define TOTP_H

#include <string>
#include <vector>
#include <cstdint>


class TOTP
{
public:

    /*
        Generate TOTP code.

        secret:
            Base32 encoded secret

        digits:
            OTP length
            Default: 6

        period:
            Time step in seconds
            Default: 30
    */
    static std::string generate(
        const std::string& secret,
        int digits = 6,
        uint64_t period = 30
    );


    /*
        Verify a TOTP code.

        Allows clock drift.

        window:
            Number of time steps to check
            Default: 1

            1 means:
            previous 30 sec
            current 30 sec
            next 30 sec
    */
    static bool verify(
        const std::string& secret,
        const std::string& code,
        int digits = 6,
        uint64_t period = 30,
        int window = 1
    );


private:

    /*
        Current Unix time counter

        counter = time / period
    */
    static uint64_t getCurrentCounter(
        uint64_t period
    );
};


#endif // TOTP_H