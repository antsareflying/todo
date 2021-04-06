#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"


sqlite3 *db;
int returncode;

int init(void);
int viewlist(void);
int insertrow(void);
int deleterow(void);
static int viewcallback(void *unused, int colcount, char **data, char **colname);

int main (void)
{
	//Welcome menu
	printf("Welcome to todo\n");
	printf("To view the list press 1\n");
	printf("To insert a new todo press 2\n");
	printf("To delete a todo press 3\n");
	printf("To quit the program press 4\n");

	//User's choice
	int choice;
	scanf("%d", &choice);
	getchar();
	printf("\n");
	init();
	
	switch (choice)
	{
		case 1:
			printf("\033[2J");
			viewlist();
			break;
			return 0;

		case 2: 
			printf("\033[2J");
			viewlist();
			insertrow();
			break;
			return 0;

		case 3:
			printf("\033[2J");
			viewlist();
			deleterow();			
			break;
			return 1;

		case 4:
			puts("Quitting...");
			break;
			return 1;

		default:
			puts("Invalid choice, try again.");
	}
	sqlite3_close(db);
	return 0;
}

int init(void)
{
	//Opens a database connection
	returncode = sqlite3_open("todo.db", &db);
	if (returncode != SQLITE_OK)
	{
		fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	//Makes a table in the db if not already present
	char *maketable = "create table if not exists todotable(todo text)";
	returncode = sqlite3_exec(db, maketable, NULL, NULL, NULL);
	if(returncode != SQLITE_OK)
	{
		fprintf(stderr, "Error making table: %s\n", sqlite3_errmsg(db));
		return 1;
	}
	return 0;
}

int viewlist(void)
{
	char *viewlist = "select rowid, * from todotable";
	returncode = sqlite3_exec(db, viewlist, viewcallback, NULL, NULL);
	if (returncode != SQLITE_OK)
	{
		fprintf(stderr, "Error reading from database: %s\n", sqlite3_errmsg(db));
		return 1;
	}
	printf("If nothing appeared that means there is no todos.\n");
	return 0;
}

int insertrow(void)
{
	char insertvalue[1024];
	char insertquery[1024];
	memset (insertvalue, '\0', sizeof(insertvalue));
	memset (insertquery, '\0', sizeof(insertquery));
	printf("Write your TODO here: ");
	if (fgets(insertvalue,1024, stdin) == NULL)
	{
		fprintf (stderr, "Error inputting value\n");
		return 1;
	}
	insertvalue[strcspn(insertvalue, "\n")] = 0;
	strcpy(insertquery, "insert into todotable values(\"");
	strcat(insertquery, insertvalue);
	strcat(insertquery, "\");\n");
	
	returncode = sqlite3_exec(db, insertquery, NULL, NULL, NULL);
	if (returncode != SQLITE_OK)
	{
		fprintf(stderr, "Error inserting into database: %s\n", sqlite3_errmsg(db));
		return 1;
	}
	printf("Inserted.\n");
	return 0;
}

int deleterow(void)
{
	char deletevalue[1024];
	char deletequery[1024];
	memset (deletevalue, '\0', sizeof(deletevalue));
	memset (deletequery, '\0', sizeof(deletequery));
	printf("Write the number of TODO you want to delete here: ");
	if (fgets(deletevalue, 1024, stdin) == NULL)
	{
		fprintf(stderr, "Error inputting value\n");
		return 1;
	}
	deletevalue[strcspn(deletevalue, "\n")] = 0;
	strcpy(deletequery, "delete from todotable where rowid = ");
	strcat(deletequery, deletevalue);
	strcat(deletequery,";\n");

	returncode = sqlite3_exec(db, deletequery, NULL, NULL, NULL);
	if (returncode != SQLITE_OK)
	{
		fprintf(stderr, "Error deleting from database: %s", sqlite3_errmsg(db));
		return 1;
	}
	printf("Deleted.\n");
	return 0;
}

static int viewcallback(void *unused, int colcount, char **data, char **colname)
{
	for (int i = 0; i < colcount; i++)
	{
		printf("%s\t", data[i]);
	}
	printf("\n");

	return 0;
}
