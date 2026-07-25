#include "totp.h"

#include "base32.h"
#include "hotp.h"

#include <ctime>


uint64_t TOTP::getCurrentCounter(uint64_t period)
{
    std::time_t currentTime = std::time(nullptr);

    return static_cast<uint64_t>(currentTime) / period;
}



std::string TOTP::generate(
    const std::string& secret,
    int digits,
    uint64_t period)
{
    /*
        Convert Base32 secret
        into binary key
    */
    std::vector<uint8_t> key =
        base32::decode(secret);



    /*
        RFC 6238

        Counter =
        Unix Time / Time Step
    */
    uint64_t counter =
        getCurrentCounter(period);



    return HOTP::generate(
        key,
        counter,
        digits
    );
}



bool TOTP::verify(
    const std::string& secret,
    const std::string& code,
    int digits,
    uint64_t period,
    int window)
{
    std::vector<uint8_t> key =
        base32::decode(secret);



    uint64_t counter =
        getCurrentCounter(period);



    /*
        Check:

        previous time step
        current time step
        future time step
    */

    for(int i = -window; i <= window; i++)
    {
        uint64_t testCounter =
            counter + i;



        std::string generated =
            HOTP::generate(
                key,
                testCounter,
                digits
            );


        if(generated == code)
        {
            return true;
        }
    }


    return false;
}