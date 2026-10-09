# Student Management System

A console-based student management system written in pure C, 
featuring singly linked list data structure and binary file persistence.

## Features

- Add / delete / list students (id, name, score)
- Binary file save & load (survives program restart)
- Menu-driven interactive interface
- Input validation with `%19s` width limit
- Full defensive guards: null checks on every operation

## Data Structure

Singly linked list with `malloc/free` dynamic memory management:
c
struct Student {
int id;
char name[20];
float score;
struct Student *next;
};


## Build & Run

bash
gcc main.c -o sms
./sms # Windows: sms.exe


## Screenshot


## What I Learned

- Pointer manipulation and dynamic memory (malloc/free pairing)
- File I/O in binary mode (fwrite/fread)
- Defensive programming: guard clauses on every public function
- Menu loop architecture with switch dispatch
