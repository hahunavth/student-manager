#ifndef STUDENT_DB_H_
#define STUDENT_DB_H_

typedef struct student
{
	int mssv;
	char *name;
	char *birth;
	char *address;
}student;

extern student* get_student_list();

extern int get_student_num ();

extern void insert_student(int mssv, char *name, char *birth, char *address);

#endif //STUDENT_DB_H_