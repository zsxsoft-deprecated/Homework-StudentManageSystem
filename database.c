#include "stdafx.h"
#include "third-party\sqlite\sqlite3.h"

sqlite3 *db;
void database_constructor() {
	int rc = sqlite3_open_v2("./data/data.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (rc != SQLITE_OK) {
		printf("SQLite����%d���޷���ʼ������", rc);
		gets();
		sqlite3_close_v2(db);
		exit(rc);
	}
}

void database_destructor() {
	sqlite3_close_v2(db);
	sqlite3_shutdown();
}