#include "otp_uri.h"

#include <sstream>


std::string OTPURI::createTOTP(
    const std::string& issuer,
    const std::string& account,
    const std::string& secret,
    int digits,
    int period
)
{
    std::stringstream uri;


    uri
    << "otpauth://totp/"
    << issuer
    << ":"
    << account

    << "?secret="
    << secret

    << "&issuer="
    << issuer

    << "&algorithm=SHA1"

    << "&digits="
    << digits

    << "&period="
    << period;


    return uri.str();
}