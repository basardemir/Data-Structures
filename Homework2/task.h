/* @Author
Student Name: Başar Demir
Student ID : 150180080
Date: 15/11/2019 */

/*
PLEASE DO NOT CHANGE THIS FILE
*/

#define NAME_LENGTH 2

struct Task{
    char *name;
    int day;
    int time;
    int priority;

    Task *previous;
    Task *next;
    Task *counterpart;
};