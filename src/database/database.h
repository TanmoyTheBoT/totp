#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

#include "account.h"


class Database
{

private:

    void* db;   // SQLite database pointer


public:

    Database();

    ~Database();


    /*
        Open / create database
    */
    bool open(
        const std::string& filename
    );


    /*
        Close database
    */
    void close();



    /*
        Create tables
    */
    bool createTables();



    /*
        Add new TOTP account
    */
    bool addAccount(
        const Account& account
    );

        // NEW
    bool updateAccount(const Account& account);

    // NEW
    bool deleteAccount(int id);



    /*
        Get all accounts
    */
    std::vector<Account> getAccounts();



    /*
        Delete account
    */
    // bool deleteAccount(int id);


};

#endif