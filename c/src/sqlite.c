
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>

/* https://www.sqlite.org/cintro.html */


static int
callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	(void)NotUsed;
	int i;
	for( i = 0; i < argc; i++ ){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	putchar('\n');
	return 0;
}

int
main(int argc, char **argv)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	if( argc != 3 ){
		fprintf(stderr, "Usage: %s db sql-statement\n", argv[0]);
		return 1 ;
	}
	rc = sqlite3_open(argv[1], &db);
	if( rc ){
		fprintf(stderr, "%s: %s\n", argv[1], sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1 ;
	}
	rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 1;
	}
	sqlite3_close(db);
	return 0;
}
