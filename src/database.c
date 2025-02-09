#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../libs/hashmap/hashmap.h"
#include "../libs/vector/vector.h"
#include "../libs/database/database.h"

void db_new(table *tb, char *table_name, int col_count, column_schema *cols) {
    if (tb == NULL || table_name == NULL || col_count <= 0 || cols == NULL) {
        fprintf(stderr, "Invalid input to db_new()\n");
        return;
    }

    tb->name = strdup(table_name);
    if (tb->name == NULL) {
        fprintf(stderr, "Memory allocation failed for table name\n");
        return;
    }

    tb->columns = (column_schema*)malloc(sizeof(column_schema) * col_count);
    if (tb->columns == NULL) {
        fprintf(stderr, "Memory allocation failed for table columns schema\n");
        return;
    }

    tb->column_count = col_count;

    for (int i=0; i<col_count; i++) {
        tb->columns[i].name = strdup(cols[i].name);
        tb->columns[i].type = strdup(cols[i].type);
        tb->columns[i].size = cols[i].size;

        if (tb->columns[i].name == NULL || tb->columns[i].type == NULL) {
            fprintf(stderr, "Memory allocation failed for table columns name or table column type\n");
            for (int j=0; j<=i; j++) {
                free(tb->columns[i].name);
                free(tb->columns[i].type);
            }
            free(tb->columns);
            free(tb->name);
            set_null(tb);

            return;
        }
    }

    tb->rows = NULL;
    tb->row_count = 0;
}

void db_load_csv(table *tb, const char *filename) {
    if (tb == NULL || filename == NULL) {
        fprintf(stderr, "Invalid input to db_load_csv()\n");
        return;
    }

    FILE *file;
    char buffer[LIMIT_LINE_LEN];

    file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Failed to opned file %s\n", filename);
        return;
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "Failed to readline table name\n");
        free(tb);
        set_null(tb);

        fclose(file);
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // auto append null character
    tb->name = malloc(strlen(buffer) + 1);
    if (tb->name == NULL) {
        fprintf(stderr, "Memory allocation failed for table name\n");
        free(tb);
        set_null(tb);
        fclose(file);
        return;
    }
    strncpy(tb->name, buffer, strlen(buffer) + 1);

    str_vector *cols_title = (str_vector*)malloc(sizeof(str_vector)),
        *cols_type = (str_vector*)malloc(sizeof(str_vector)),
        *cols_size = (str_vector*)malloc(sizeof(str_vector));
    if (cols_title == NULL || cols_type == NULL || cols_size == NULL) {
        fprintf(stderr, "Memory allocation failed for cols_title, cols_type, cols_size\n");
        str_vec_free(cols_title);
        str_vec_free(cols_type);
        str_vec_free(cols_size);
        fclose(file);
        return;
    }

    init_str_vec(cols_title);
    init_str_vec(cols_type);
    init_str_vec(cols_size);

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "Failed to readline columns title\n");
        free(tb->name);
        free(tb);
        set_null(tb);

        str_vec_free(cols_title);
        str_vec_free(cols_type);
        str_vec_free(cols_size);
        fclose(file);
        return;
    }
    char *col_title = strtok(buffer, ",");
    while (col_title != NULL) {
        str_vec_push_back(cols_title, col_title);
        col_title = strtok(NULL, ",");
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "Failed to readline columns type\n");
        free(tb->name);
        free(tb);
        set_null(tb);

        str_vec_free(cols_title);
        str_vec_free(cols_type);
        str_vec_free(cols_size);
        fclose(file);
        return;
    }
    char *col_type = strtok(buffer, ",");
    while (col_type != NULL) {
        str_vec_push_back(cols_type, col_type);
        col_type = strtok(NULL, ",");
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "Failed to readline columns size\n");
        free(tb->name);
        free(tb);
        set_null(tb);

        str_vec_free(cols_title);
        str_vec_free(cols_type);
        str_vec_free(cols_size);
        fclose(file);
        return;
    }
    char *col_size = strtok(buffer, ",");
    while (col_size != NULL) {
        str_vec_push_back(cols_size, col_size);
        col_size = strtok(NULL, ",");
    }

    tb->columns = (column_schema*)malloc(sizeof(column_schema) * cols_title->size);
    if (tb->columns == NULL) {
        fprintf(stderr, "Memory allocation for column schema failed\n");
        free(tb->name);
        free(tb);
        set_null(tb);

        str_vec_free(cols_title);
        str_vec_free(cols_type);
        str_vec_free(cols_size);
        fclose(file);
        return;
    }

    tb->column_count = cols_title->size;

    for (int i=0; i<tb->column_count; i++) {
        cols_title->arr[i][strcspn(cols_title->arr[i], "\n")] = '\0';
        cols_type->arr[i][strcspn(cols_type->arr[i], "\n")] = '\0';

        tb->columns[i].name = strdup(cols_title->arr[i]);
        tb->columns[i].type = strdup(cols_type->arr[i]);
        tb->columns[i].size = atoi(cols_size->arr[i]);

        if (tb->columns[i].name == NULL || tb->columns[i].type == NULL) {
            fprintf(stderr, "Memory allocation failed for table columns name or table column type\n");
            for (int j=0; j<=i; j++) {
                free(tb->columns[i].name);
                free(tb->columns[i].type);
            }
            free(tb->columns);
            free(tb->name);
            free(tb);
            set_null(tb);
            
            str_vec_free(cols_title);
            str_vec_free(cols_type);
            str_vec_free(cols_size);
            fclose(file);
            return;
        }
    }
    str_vec_free(cols_title);
    str_vec_free(cols_type);
    str_vec_free(cols_size);

    str_vector *rows = (str_vector*)malloc(sizeof(str_vector));
    if (rows == NULL) {
        fprintf(stderr, "Memory allocation failed for rows\n");
        for (int i=0; i<tb->column_count; i++) {
            free(tb->columns[i].name);
            free(tb->columns[i].type);
        }
        free(tb->columns);
        free(tb->name);
        free(tb);
        set_null(tb);
        fclose(file);
        return;
    }
    
    int begin_row_pos = ftell(file);
    size_t row_count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        row_count++;
    }
    fseek(file, begin_row_pos, SEEK_SET);
    tb->row_count = row_count;

    tb->rows = (row*)malloc(sizeof(row) * tb->row_count);
    if (tb->rows == NULL) {
        fprintf(stderr, "Memory allocation failed for rows\n");
        for (int i=0; i<tb->column_count; i++) {
            free(tb->columns[i].name);
            free(tb->columns[i].type);
        }
        free(tb->columns);
        free(tb->name);
        free(tb);
        set_null(tb);
        fclose(file);
        return;
    }

    for (int i=0; i<tb->row_count; i++) {
        tb->rows[i].cells = (cell*)malloc(sizeof(cell) * tb->column_count);
        tb->rows[i].column_count = tb->column_count;

        if (tb->rows[i].cells == NULL || fgets(buffer, sizeof(buffer), file) == NULL) {
            fprintf(stderr, "Memory allocation failed for row or cannot readline row\n");
            for (int j=0; j<=i; j++) {
                free(tb->rows[j].cells);
            }
            free(tb->rows);

            for (int i=0; i<tb->column_count; i++) {
                free(tb->columns[i].name);
                free(tb->columns[i].type);
            }
            free(tb->columns);
            free(tb->name);
            free(tb);
            set_null(tb);
            fclose(file);
            return;
        }

        char *data = strtok(buffer, ",");
        for (int j=0; j<tb->column_count; j++) {
            data[strcspn(data, "\n")] = '\0';
            tb->rows[i].cells[j].data = strdup(data);
            tb->rows[i].cells[j].type = strdup(tb->columns[j].type);
            data = strtok(NULL, ",");
        }
    }

    fclose(file);
}