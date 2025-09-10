#ifndef DATABASE_H
#define DATABASE_H

#include <stdbool.h>

bool authenticate_user(const char *username, const char *password, char *role);
bool add_exam(const char *question, const char *answer);
bool submit_exam(const char *username, int exam_id, const char *answer);
bool evaluate_exam(int exam_id, const char *correct_answer, int *score);
void fetch_exam(int exam_id, char *question);

#endif

