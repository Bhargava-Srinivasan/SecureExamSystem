#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "password"
#define DB_NAME "exam_system"

MYSQL *connect_db() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("MySQL initialization failed\n");
        return NULL;
    }
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        printf("Database connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

bool authenticate_user(const char *username, const char *password, char *role) {
    MYSQL *conn = connect_db();
    if (!conn) return false;

    char query[256];
    snprintf(query, sizeof(query), "SELECT role FROM users WHERE username='%s' AND password='%s'", username, password);
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        mysql_close(conn);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    strcpy(role, row[0]);
    
    mysql_free_result(result);
    mysql_close(conn);
    return true;
}

bool add_exam(const char *question, const char *answer) {
    MYSQL *conn = connect_db();
    if (!conn) return false;

    char query[512];
    snprintf(query, sizeof(query), "INSERT INTO exams (question, answer) VALUES ('%s', '%s')", question, answer);
    bool success = (mysql_query(conn, query) == 0);

    mysql_close(conn);
    return success;
}

bool submit_exam(const char *username, int exam_id, const char *answer) {
    MYSQL *conn = connect_db();
    if (!conn) return false;

    char query[512];
    snprintf(query, sizeof(query), "INSERT INTO exam_submissions (username, exam_id, answer) VALUES ('%s', %d, '%s')", username, exam_id, answer);
    bool success = (mysql_query(conn, query) == 0);

    mysql_close(conn);
    return success;
}

bool evaluate_exam(int exam_id, const char *correct_answer, int *score) {
    MYSQL *conn = connect_db();
    if (!conn) return false;

    char query[512];
    snprintf(query, sizeof(query), "SELECT answer FROM exam_submissions WHERE exam_id=%d", exam_id);
    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        mysql_close(conn);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    *score = (strcmp(row[0], correct_answer) == 0) ? 100 : 0;

    mysql_free_result(result);
    mysql_close(conn);
    return true;
}

