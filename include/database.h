
#ifndef DATABASE_H
#define DATABASE_H
#define LIMIT_TABLE_NAME 128

typedef struct _cell {
    void *data;
    char *type;
} cell;

typedef struct _row {
    cell *cells;
    int column_count;
} row;

typedef struct _column_schema {
    char *name;
    char *type;
    int size;
} column_schema;

typedef struct _table {
    char name[LIMIT_TABLE_NAME];

    column_schema *columns;
    int column_count;

    row *rows;
    int row_count;
} table;

void db_new(table *tb, char *table_name, int col_count, column_schema *column_schema);
void db_load_csv(table *tb, char *filename);

#endif