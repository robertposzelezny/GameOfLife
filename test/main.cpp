#include "Game.h"
#include <windows.h>
#include <sqlext.h>
#include <iostream>

int main() {

    SQLHENV env;
    SQLHDBC dbc;
    SQLRETURN ret;

    // Allocate environment
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    // Allocate connection
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    // Connection string
    SQLCHAR connStr[] =
        "DRIVER={SQL Server};SERVER=DESKTOP-TQN13PE;DATABASE=GameOfLifeDB;Trusted_Connection=yes;";

    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;

    ret = SQLDriverConnect(
        dbc,
        NULL,
        connStr,
        SQL_NTS,
        outstr,
        sizeof(outstr),
        &outstrlen,
        SQL_DRIVER_COMPLETE
    );

    if (SQL_SUCCEEDED(ret)) {
        std::cout << "Connected to database!" << std::endl;
    }
    else {
        std::cout << "Connection failed!" << std::endl;
    }

    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);

    return 0;

	//Game game(60);
	//game.runGame();
	//return 0;
}
