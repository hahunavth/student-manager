#include "sqlite3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student-db.h"

#define DB_NAME "../db/test.db"
#define TABLE_NAME "students"

student studentList[1000000];
int studentNum = 0;

static int callback(void *NotUsed, int argc, char **argv,
          char **azColName) {
  NotUsed = 0;
  if(studentNum >= 1000000)
    return 1;
  char *name = calloc(100, sizeof(char));
  char *birth = calloc(100, sizeof(char));
  char *address = calloc(100, sizeof(char));
  strcpy(name, argv[1]);
  strcpy(birth, argv[2]);
  strcpy(address, argv[3]);

  student *newStudent = calloc(1, sizeof(student));
  studentList[studentNum].mssv = atoi(argv[0]);
  (studentList+studentNum)->name = name;
  (studentList+studentNum)->birth = birth;
  (studentList+studentNum)->address = address;
  studentNum++;

  return 0;
}

void printStudent(student *studentList,int studentNum) {
  for(int i = 0; i < studentNum; i++) {
    printf("%d %s %s %s\n", (studentList+i)->mssv, (studentList+i)->name, 
      (studentList+i)->birth, (studentList+i)->address);
  }
}

extern student* get_student_list() {
  sqlite3 *db;
  char *err_msg = 0;
  int rc = sqlite3_open(DB_NAME, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n",
        sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }
  char sql[100];
  strcpy(sql, "SELECT * FROM ");
  strcat(sql, TABLE_NAME);
  rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
  if (rc != SQLITE_OK ) {
    fprintf(stderr, "Failed to select data\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }
  sqlite3_close(db);

  printStudent(studentList, studentNum);

  return studentList;
}

extern int get_student_num () {
  return studentNum;
}
