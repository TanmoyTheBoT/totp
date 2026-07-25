#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>


class Account
{

public:

    int id;

    std::string issuer;

    std::string username;

    std::string secret;

    std::string algorithm;

    int digits;

    int period;


    Account()
    {
        id = 0;
        digits = 6;
        period = 30;
        algorithm = "SHA1";
    }


};


#endif