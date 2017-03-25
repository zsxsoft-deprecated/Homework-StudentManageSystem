#pragma once
#include "third-party/sqlite/sqlite3.h"
extern sqlite3* db;
void database_constructor(void);
void database_destructor(void);