#ifndef OTP_URI_H
#define OTP_URI_H

#include <string>


class OTPURI
{
public:

    static std::string createTOTP(
        const std::string& issuer,
        const std::string& account,
        const std::string& secret,
        int digits = 6,
        int period = 30
    );

};


#endif