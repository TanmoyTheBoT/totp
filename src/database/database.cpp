#include "database.h"

#include "sqlite3.h"
#include <iostream>


Database::Database()
{
    db = nullptr;
}


Database::~Database()
{
    close();
}



bool Database::open(
    const std::string& filename
)
{

    sqlite3* database;


    int result =
        sqlite3_open(
            filename.c_str(),
            &database
        );


    if(result != SQLITE_OK)
    {
        std::cout
            << "Database open failed\n";

        return false;
    }


    db = database;


    return true;
}



void Database::close()
{

    if(db)
    {
        sqlite3_close(
            static_cast<sqlite3*>(db)
        );

        db = nullptr;
    }

}




bool Database::createTables()
{

    const char* sql = R"(

    CREATE TABLE IF NOT EXISTS accounts
    (

        id INTEGER PRIMARY KEY AUTOINCREMENT,

        issuer TEXT NOT NULL,

        username TEXT NOT NULL,

        secret TEXT NOT NULL,

        algorithm TEXT DEFAULT 'SHA1',

        digits INTEGER DEFAULT 6,

        period INTEGER DEFAULT 30,

        notes TEXT DEFAULT '',

        favorite INTEGER DEFAULT 0

    );

    )";



    char* error = nullptr;


    int result =
        sqlite3_exec(
            static_cast<sqlite3*>(db),
            sql,
            nullptr,
            nullptr,
            &error
        );



    if(result != SQLITE_OK)
    {

        std::cout
            << "Table creation failed: "
            << error
            << std::endl;


        sqlite3_free(error);


        return false;
    }


    return true;

}





bool Database::addAccount(
    const Account& account
)
{

    std::string sql = R"(

    INSERT INTO accounts
    (
        issuer,
        username,
        secret,
        algorithm,
        digits,
        period,
        notes,
        favorite,
        iconPath
    )

    VALUES(?,?,?,?,?,?,?,?,?);

    )";



    sqlite3_stmt* stmt;



    sqlite3_prepare_v2(

        static_cast<sqlite3*>(db),

        sql.c_str(),

        -1,

        &stmt,

        nullptr

    );



    sqlite3_bind_text(
        stmt,
        1,
        account.issuer.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        stmt,
        2,
        account.username.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        stmt,
        3,
        account.secret.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        stmt,
        4,
        account.algorithm.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_int(
        stmt,
        5,
        account.digits
    );


    sqlite3_bind_int(
        stmt,
        6,
        account.period
    );


    sqlite3_bind_text(
        stmt,
        7,
        account.notes.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_int(
        stmt,
        8,
        account.favorite ? 1 : 0
    );

    sqlite3_bind_text(
        stmt,
        9,
        account.iconPath.c_str(),
        -1,
        SQLITE_TRANSIENT
    );



    bool success =
        sqlite3_step(stmt)
        ==
        SQLITE_DONE;



    sqlite3_finalize(stmt);



    return success;

}


bool Database::updateAccount(
    const Account& account
)
{

    std::string sql = R"(

    UPDATE accounts

    SET

        issuer=?,

        username=?,

        secret=?,

        algorithm=?,

        digits=?,

        period=?,

        notes=?,

        favorite=?,

        iconPath=?

    WHERE id=?;

    )";


    sqlite3_stmt* stmt;


    sqlite3_prepare_v2(

        static_cast<sqlite3*>(db),

        sql.c_str(),

        -1,

        &stmt,

        nullptr

    );


    sqlite3_bind_text(
        stmt,
        1,
        account.issuer.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        stmt,
        2,
        account.username.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        stmt,
        3,
        account.secret.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        stmt,
        4,
        account.algorithm.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_int(
        stmt,
        5,
        account.digits
    );


    sqlite3_bind_int(
        stmt,
        6,
        account.period
    );


    sqlite3_bind_text(
        stmt,
        7,
        account.notes.c_str(),
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_int(
        stmt,
        8,
        account.favorite ? 1 : 0
    );

    sqlite3_bind_text(
        stmt,
        9,
        account.iconPath.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        10,
        account.id
    );


    bool success =
        sqlite3_step(stmt)
        ==
        SQLITE_DONE;


    sqlite3_finalize(stmt);


    return success;

}


std::vector<Account> Database::getAccounts()
{

    std::vector<Account> accounts;


    const char* sql =
        "SELECT * FROM accounts;";



    sqlite3_stmt* stmt;



    sqlite3_prepare_v2(

        static_cast<sqlite3*>(db),

        sql,

        -1,

        &stmt,

        nullptr

    );



    while(
        sqlite3_step(stmt)
        ==
        SQLITE_ROW
    )
    {

        Account acc;



        acc.id =
            sqlite3_column_int(
                stmt,
                0
            );



        acc.issuer =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    stmt,
                    1
                )
            );



        acc.username =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    stmt,
                    2
                )
            );



        acc.secret =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    stmt,
                    3
                )
            );



        acc.algorithm =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    stmt,
                    4
                )
            );



        acc.digits =
            sqlite3_column_int(
                stmt,
                5
            );


        acc.period =
            sqlite3_column_int(
                stmt,
                6
            );


        // ponytail: check column exists before reading (handles old DBs)
        if (sqlite3_column_count(stmt) > 7) {
            const unsigned char* notesText = sqlite3_column_text(stmt, 7);
            acc.notes = notesText ? reinterpret_cast<const char*>(notesText) : "";
        }


        if (sqlite3_column_count(stmt) > 8) {
            acc.favorite = sqlite3_column_int(stmt, 8) == 1;
        }

        if (sqlite3_column_count(stmt) > 9) {
            const unsigned char* iconText = sqlite3_column_text(stmt, 9);
            acc.iconPath = iconText ? reinterpret_cast<const char*>(iconText) : "";
        }


        accounts.push_back(acc);

    }



    sqlite3_finalize(stmt);



    return accounts;

}






bool Database::deleteAccount(
    int id
)
{

    std::string sql =
        "DELETE FROM accounts WHERE id=?;";



    sqlite3_stmt* stmt;



    sqlite3_prepare_v2(

        static_cast<sqlite3*>(db),

        sql.c_str(),

        -1,

        &stmt,

        nullptr

    );



    sqlite3_bind_int(
        stmt,
        1,
        id
    );



    bool success =
        sqlite3_step(stmt)
        ==
        SQLITE_DONE;



    sqlite3_finalize(stmt);



    return success;

}