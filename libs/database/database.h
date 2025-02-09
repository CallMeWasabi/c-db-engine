
#ifndef DATABASE_H
#define DATABASE_H
#define LIMIT_LINE_LEN 8192 // 8 kb
#define set_null(tb) \
    tb->name = NULL; \
    tb->columns = NULL; \
    tb->rows = NULL; \
    tb->column_count = tb->row_count = 0; \
    tb = NULL;

typedef struct _cell {
    void *data;
    char *type;
} cell;

typedef struct _row {
    cell *cells;
    size_t column_count;
} row;

typedef struct _column_schema {
    char *name;
    char *type;
    size_t size;
} column_schema;

typedef struct _table {
    char *name;

    column_schema *columns;
    size_t column_count;

    row *rows;
    size_t row_count;
} table;

void db_new(table *tb, char *table_name, int col_count, column_schema *column_schema);
void db_load_csv(table *tb, const char *filename);
void db_close(table *tb);

#endif