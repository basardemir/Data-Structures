/* @Author
Student Name: Başar Demir
Student ID : 150180080
Date: 15/11/2019 */

/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "task_management_tool.h"

using namespace std;


void WorkPlan::display(bool verbose, bool testing) {
    string inone = "***";
    if (head != NULL) {
        Task *pivot = new Task;
        Task *compeer = new Task;
        pivot = head;
        do {
            if (testing)
                inone += " ";
            else
                cout << pivot->day << ". DAY" << endl;
            compeer = pivot;
            while (compeer != NULL) {
                string PREV = compeer->previous != NULL ? compeer->previous->name : "NULL";
                string NEXT = compeer->next != NULL ? compeer->next->name : "NULL";
                string CONT = compeer->counterpart != NULL ? compeer->counterpart->name : "NULL";
                if (testing)
                    inone += compeer->name;
                else if (verbose)
                    cout << "\t" << setw(2) << compeer->time << ":00\t" << PREV << "\t<- " << compeer->name << "("
                         << compeer->priority << ")->\t" << NEXT << "\t |_" << CONT << endl;
                else
                    cout << "\t" << setw(2) << compeer->time << ":00\t" << compeer->name << "(" << compeer->priority
                         << ")" << endl;
                compeer = compeer->counterpart;
            }
            pivot = pivot->next;
        } while (pivot != head);
        if (testing) {
            cout << inone << endl;
            cout << "(checking cycled list:";
            if (checkCycledList())
                cout << " PASS)" << endl;
            else
                cout << " FAIL)" << endl;
        }
    } else
        cout << "There is no task yet!" << endl;
}

int WorkPlan::getUsableDay() {
    return usable_day;
}

int WorkPlan::getUsableTime() {
    return usable_time;
}


void WorkPlan::create() {
    head = NULL;            //it creates first task as a null
    day_num = 0;
}

void WorkPlan::close() {
    Task *ptr;
    Task *counter;
    while (head && day_num != 0) {
        ptr = head;                 //it travels all days and tasks of days
        head = head->next;          //and deletes them.
        counter = ptr->counterpart;
        while (counter) {
            Task *del = counter;
            counter = counter->counterpart;
            delete[] del->name;
            delete del;
        }
        delete[] ptr->name;
        delete ptr;
        day_num--;
    }
}

void WorkPlan::add(Task *task) {
    char *static_name = static_cast<char *>(malloc(strlen(task->name) + 1));
    strcpy(static_name, task->name);
    if (day_num == 0) {
        Task *ptr = new Task();
        head = ptr;                //If there is not any day it creates first day and task
        ptr->next = ptr;
        ptr->previous = ptr;
        ptr->counterpart = NULL;
        ptr->name = static_name;
        ptr->day = task->day;
        ptr->time = task->time;
        ptr->priority = task->priority;
        day_num++;
        return;
    } else {
        Task *ptr = head;
        if (task->day == head->day) {     //If day of task that is wanted to add is first day
            if (task->time == head->time) {  //If it is first task
                if (task->priority <= head->priority) {
                    checkAvailableNextTimesFor(task);
                    int day = getUsableDay();           //It checks available times for task
                    task->day = day;                   //updates task
                    int time = getUsableTime();        //calls add function recursively
                    task->time = time;
                    add(task);
                    return;
                } else {
                    checkAvailableNextTimesFor(head);
                    Task *new_ptr = new Task();     //It creates new task for input
                    new_ptr->day = task->day;       //and initialize its values
                    new_ptr->time = task->time;
                    new_ptr->priority = task->priority;
                    new_ptr->name = static_name;
                    new_ptr->counterpart = head->counterpart;

                    new_ptr->next = head->next;         //adds new task to head position
                    new_ptr->previous = head->previous;
                    head->previous->next = new_ptr;
                    head->next->previous = new_ptr;
                    head->previous = NULL;
                    head->next = NULL;
                    head->counterpart = NULL;


                    int day = getUsableDay();
                    task->day = day;            //it updates task with head values
                    int time = getUsableTime();
                    task->time = time;


                    char *ptr_name = static_cast<char *>(malloc(strlen(ptr->name) + 1));
                    strcpy(ptr_name, head->name);
                    task->name = ptr_name;        //it updates task with head values
                    int pri = head->priority;
                    task->priority = pri;

                    head = new_ptr; //head updated

                    add(task); //add function called recursively
                    return;
                }
            }
            if (task->time > head->time) {      //If task is not first appointment of a head day
                Task *ptr = head;
                while (ptr->time < task->time) { //It searches for proper time
                    if (ptr->counterpart == NULL) { //If task time bigger than other appointments
                        Task *new_ptr = new Task();
                        new_ptr->day = task->day;
                        new_ptr->time = task->time;     //Task is added as a last appointment of a day
                        new_ptr->priority = task->priority;
                        new_ptr->name = static_name;
                        new_ptr->next = NULL;
                        new_ptr->previous = NULL;
                        ptr->counterpart = new_ptr;
                        new_ptr->counterpart = NULL;
                        return;
                    }
                    if (ptr->counterpart->time == task->time) { //If ptr finds same timed appointments
                        ptr = ptr->counterpart;
                        if (ptr->priority >= task->priority) {  //It checks priorities
                            checkAvailableNextTimesFor(task);
                            Task *new_task = new Task();
                            int day = getUsableDay();    //It checks available times for tas
                            int time = getUsableTime(); //updates task
                            new_task->day = day;
                            new_task->time = time;
                            new_task->priority = task->priority;
                            new_task->name = static_name;
                            new_task->next = NULL;
                            new_task->previous = NULL;
                            add(new_task);//calls add function recursively
                            return;
                        }
                        if (ptr->priority < task->priority) {
                            checkAvailableNextTimesFor(ptr);
                            int day = getUsableDay();
                            int time = getUsableTime();

                            char *ptr_name = static_cast<char *>(malloc(strlen(ptr->name) + 1));
                            strcpy(ptr_name, ptr->name);

                            Task *new_task = new Task(); //It creates new task for input
                            ptr->name = static_name;
                            task->name = ptr_name;      // It replace task's and ptr's values
                            task->day = day;            //So removing process is not used
                            task->time = time;
                            int temp = ptr->priority;
                            ptr->priority = task->priority;
                            task->priority = temp;

                            new_task->day = day;
                            new_task->time = time;      //It changes new_task values and add recursively
                            new_task->priority = ptr->priority;
                            new_task->name = ptr_name;
                            new_task->next = NULL;
                            new_task->previous = NULL;
                            add(new_task);
                            return;
                        }
                    }
                    if (ptr->counterpart->time > task->time) {
                        Task *new_ptr = new Task();
                        new_ptr->day = task->day;
                        new_ptr->time = task->time;         //If  task time bigger than all tasks of day
                        new_ptr->priority = task->priority; //it adds end of day
                        new_ptr->name = static_name;
                        new_ptr->next = NULL;
                        new_ptr->previous = NULL;
                        new_ptr->counterpart = ptr->counterpart;
                        ptr->counterpart = new_ptr;
                        return;
                    }
                    ptr = ptr->counterpart;
                }
            }
            if (task->time < head->time) {
                Task *new_ptr = new Task();
                new_ptr->day = task->day;
                new_ptr->time = task->time;         //If task time smaller than head time
                new_ptr->priority = task->priority; //task will be new head and head will be under counterpart
                new_ptr->name = static_name;

                new_ptr->counterpart = head;
                new_ptr->next = head->next;
                new_ptr->previous = head->previous;
                head->previous->next = new_ptr;
                head->next->previous = new_ptr;
                head->previous = NULL;
                head->next = NULL;
                head = new_ptr;
                return;
            }

        }
        if (task->day < head->day) {
            Task *new_ptr = new Task();
            new_ptr->day = task->day;       //If task day smaller tan all task's days
            new_ptr->time = task->time;        //task added to head's previous
            new_ptr->priority = task->priority;//It creates new task for input
            new_ptr->name = static_name;//and initialize its values
            new_ptr->next = head;
            new_ptr->previous = head->previous;
            head->previous->next = new_ptr;
            head->previous = new_ptr;
            head = new_ptr;
            day_num++;
            return;
        } else { //task_day > head_day
            if (head->next == head) {
                Task *new_ptr = new Task();//It creates new task for input
                new_ptr->day = task->day;
                new_ptr->time = task->time;
                new_ptr->priority = task->priority;
                new_ptr->name = static_name;       //If there is only one day it adds as a last element
                head->next = new_ptr;
                head->previous = new_ptr;
                new_ptr->next = head;
                new_ptr->previous = head;
                new_ptr->counterpart = NULL;
                day_num++;
                return;
            }
            while (ptr->next->day <= task->day && ptr->next != head) {  //It started to traverse all day to find
                ptr = ptr->next;                                        //proper day
                if (ptr->day == task->day) {
                    if (ptr->time == task->time) { //It finds day
                        if (ptr->priority >= task->priority) {
                            checkAvailableNextTimesFor(task);
                            Task *new_task = new Task();//It creates new task for input
                            int day = getUsableDay();   //IT is looking for new usable day
                            int time = getUsableTime();
                            new_task->day = day;
                            new_task->time = time;
                            new_task->priority = task->priority;
                            new_task->name = static_name;
                            new_task->next = NULL;
                            new_task->previous = NULL;
                            add(new_task);
                            return;
                        } else {
                            checkAvailableNextTimesFor(ptr);
                            int day = getUsableDay();
                            int time = getUsableTime();

                            char *ptr_name = static_cast<char *>(malloc(strlen(ptr->name) + 1));
                            strcpy(ptr_name, ptr->name);

                            Task *new_task = new Task(); //It creates new task for input

                            ptr->name = static_name;    // It replace task's and ptr's values
                            task->name = ptr_name;      //So removing process is not used
                            task->day = day;
                            task->time = time;
                            int temp = ptr->priority;
                            ptr->priority = task->priority;
                            task->priority = temp;


                            new_task->day = day;
                            new_task->time = time;
                            new_task->priority = ptr->priority;
                            new_task->name = ptr_name;
                            new_task->next = NULL;
                            new_task->previous = NULL;

                            add(new_task);          //It calls add function recursively
                            return;
                        }
                    } else {
                        if (task->time > ptr->time) {
                            while (ptr->time <= task->time) {
                                if (ptr->time == task->time) {      //If it finds same appointment it compares priority
                                    if (ptr->priority >= task->priority) {
                                        checkAvailableNextTimesFor(task);
                                        Task *new_task = new Task();    //It looks for new time for task
                                        int day = getUsableDay();//It creates new task for input
                                        int time = getUsableTime();
                                        new_task->day = day;
                                        new_task->time = time;
                                        new_task->priority = task->priority;
                                        new_task->name = static_name;
                                        new_task->next = NULL;
                                        new_task->previous = NULL;
                                        add(new_task);
                                        return;
                                    }
                                    if (ptr->priority < task->priority) {
                                        checkAvailableNextTimesFor(ptr);
                                        int day = getUsableDay();
                                        int time = getUsableTime();

                                        char *ptr_name = static_cast<char *>(malloc(strlen(ptr->name) + 1));
                                        strcpy(ptr_name, ptr->name);

                                        Task *new_task = new Task();//It creates new task for input

                                        ptr->name = static_name;
                                        task->name = ptr_name;      //It changes ptr properties with task properties
                                        task->day = day;
                                        task->time = time;
                                        int temp = ptr->priority;
                                        ptr->priority = task->priority;
                                        task->priority = temp;


                                        new_task->day = day;
                                        new_task->time = time;      //It looks new time for ptr
                                        new_task->priority = ptr->priority;
                                        new_task->name = ptr_name;
                                        new_task->next = NULL;
                                        new_task->previous = NULL;

                                        add(new_task);
                                        return;
                                    }
                                }
                                if (ptr->counterpart == NULL && ptr->time < task->time) {
                                    Task *new_ptr = new Task();//It creates new task for input
                                    new_ptr->day = task->day;
                                    new_ptr->time = task->time;     //It adds task to the end of day
                                    new_ptr->priority = task->priority;
                                    new_ptr->name = static_name;
                                    new_ptr->next = NULL;
                                    new_ptr->previous = NULL;
                                    ptr->counterpart = new_ptr;
                                    new_ptr->counterpart = NULL;
                                    return;
                                }

                                if (ptr->counterpart->time > task->time) {
                                    Task *new_ptr = new Task();//It creates new task for input
                                    new_ptr->day = task->day;       //It adds between to appointments
                                    new_ptr->time = task->time;
                                    new_ptr->priority = task->priority;
                                    new_ptr->name = static_name;
                                    new_ptr->next = NULL;
                                    new_ptr->previous = NULL;
                                    new_ptr->counterpart = ptr->counterpart;
                                    ptr->counterpart = new_ptr;
                                    return;
                                }
                                ptr = ptr->counterpart;
                            }
                        }
                        if (task->time < ptr->time) {
                            Task *new_ptr = new Task();//It creates new task for input
                            new_ptr->day = task->day;
                            new_ptr->time = task->time;     //It adds beginning of the day
                            new_ptr->priority = task->priority;
                            new_ptr->name = static_name;

                            new_ptr->counterpart = ptr;
                            new_ptr->next = ptr->next;
                            new_ptr->previous = ptr->previous;
                            ptr->previous->next = new_ptr;
                            ptr->next->previous = new_ptr;
                            ptr->previous = NULL;
                            ptr->next = NULL;
                            return;
                        }
                    }
                }
            }
            if (ptr->next == head && ptr->day < task->day) {
                Task *new_ptr = new Task();//It creates new task for input
                new_ptr->day = task->day;
                new_ptr->time = task->time; //It creates new day to end of agenda
                new_ptr->priority = task->priority;
                new_ptr->name = static_name;
                day_num++;
                new_ptr->previous = ptr;
                new_ptr->next = head;
                head->previous = new_ptr;
                ptr->next = new_ptr;
                return;
            }
            if (ptr->next->day > task->day && ptr->day < task->day) {
                Task *new_ptr = new Task();//It creates new task for input
                new_ptr->day = task->day;
                new_ptr->time = task->time;  //It adds between two days
                new_ptr->priority = task->priority;
                new_ptr->name = static_name;
                day_num++;
                new_ptr->next = ptr->next;
                ptr->next->previous = new_ptr;
                new_ptr->previous = ptr;
                ptr->next = new_ptr;
                new_ptr->counterpart = NULL;
                return;
            }
        }
    }
}


Task *WorkPlan::getTask(int day, int time) {
    Task *ptr = head;
    while (ptr->day <= day) { //It start to travel all days
        Task *day_ptr = ptr;
        if (ptr->day == day) { //It found
            while (ptr->time <= time) {//It start to travel all counterparts
                if (ptr->time == time) {
                    return ptr;     //When it finds it returns its pointer
                }
                if (ptr->counterpart == NULL) {
                    return NULL;
                }
                ptr = ptr->counterpart;
            }
        }
        if (day_ptr->next == head) {
            return NULL;
        }
        ptr = ptr->next;
    }
    return NULL;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed) {
    Task *ptr = head;
    while (ptr->next != head) {//It start to travel all days
        if (delayed->day == ptr->day) { //If it found
            Task *day_ptr = ptr;
            while (ptr->counterpart != NULL) {//It start to travel all times
                if (ptr->time >= delayed->time && ptr->counterpart->time - ptr->time >= 2) {
                    int time = ptr->time + 1;
                    int day = delayed->day;     //If it finds proper time after delayed time
                    usable_time = time;         //It returns its time
                    usable_day = day;
                    return;
                }
                ptr = ptr->counterpart;
            }
            if (ptr->counterpart == NULL) {
                ptr = day_ptr;      //If it comes to end of day,it returns pointer of beginning of the day
            }

        }
        if (ptr->day > delayed->day) { //If it did not find proper time in delayed day
            Task *day_ptr = ptr;
            while (ptr->counterpart != NULL) { //It looks for proper time
                if (ptr->time > 8 && ptr->next != NULL) { //It returns 8 if there is not
                    usable_time = 8;
                    int day = ptr->day;
                    usable_day = day;
                    return;
                }
                if (ptr->counterpart->time - ptr->time >= 2) {
                    int time = ptr->time + 1; //It returns a time middle of day
                    int day = ptr->day;
                    usable_time = time;
                    usable_day = day;
                    return;
                }
                ptr = ptr->counterpart;
            }
            if (ptr->counterpart == NULL) {
                if (ptr->time > 8 && ptr->next != NULL) {
                    usable_time = 8;
                    int day = ptr->day;     //If returns 8 for one appointment allocated days
                    usable_day = day;
                    return;
                }
                ptr = day_ptr;
            }

        }
        ptr = ptr->next;
    }
    if (ptr->next == head) { // It starts to travel other days
        while (ptr->counterpart != NULL) { //It starts to travel hours of day
            if (ptr->time > 8 && ptr->next != NULL && ptr->day != delayed->day) {
                usable_time = 8;            //  If 8 is not allocated it returns it
                int day = ptr->day;
                usable_day = day;
                return;
            }
            if (ptr->counterpart->time - ptr->time >= 2) {
                int time = ptr->time + 1;
                int day = ptr->day;         //It checks proper time between two appointment
                usable_time = time;
                usable_day = day;
                return;
            }
            ptr = ptr->counterpart;
        }
        if (ptr->counterpart == NULL && day_num == 1) {
            int day = ptr->day + 1; //If there is no other day
            int time = 8;           //It returns new day
            usable_time = time;
            usable_day = day;
            return;
        }
        if (ptr->counterpart == NULL && day_num != 1) { //If there is no usable time
            Task *new_ptr = head;                       //It will try to allocate nearest time
            Task *dayptr = new_ptr;
            while (new_ptr->day <= delayed->day) {
                dayptr = new_ptr;
                if (new_ptr->day == delayed->day) { //It finds day of task which will be delayed
                    if (new_ptr->counterpart == NULL && new_ptr->time < 16) {
                        int time = new_ptr->time++;
                        time++;                     //For days that have one task
                        int day = new_ptr->day;
                        usable_time = time;
                        usable_day = day;
                        return;
                    }
                    if (new_ptr->counterpart == NULL && new_ptr->time >= 16) { //If there is not any usable time
                        while (new_ptr->next != head) {                        //it searches other days
                            if (new_ptr->counterpart == NULL && new_ptr->time < 16) {
                                int time = new_ptr->time;
                                time++;
                                int day = new_ptr->day;         //For days that have one task
                                usable_time = time;
                                usable_day = day;
                                return;
                            }
                            Task *day_ptr = new_ptr;
                            while (new_ptr->counterpart) {
                                new_ptr = new_ptr->counterpart;     //It travels to last task of day
                            }
                            if (new_ptr->time < 16) {
                                int day = new_ptr->day;
                                int time = new_ptr->time + 1; //If it is usable returns value
                                usable_time = time;
                                usable_day = day;
                                return;
                            } else {
                                new_ptr = day_ptr->next;
                            }

                        }
                    }
                    while (new_ptr->counterpart) {
                        new_ptr = new_ptr->counterpart; //It travels to last task of delayed's day
                    }
                    if (new_ptr->time < 16) {
                        int time = new_ptr->time;
                        time++;
                        int day = new_ptr->day;         //If it is usable returns value
                        usable_time = time;
                        usable_day = day;
                        return;
                    } else {                                //If there is not any usable time
                        while (dayptr->next != head) {      //it searches other days
                            Task *temp = dayptr;
                            if (dayptr->counterpart == NULL && dayptr->time < 16 && dayptr->day > delayed->day) {
                                int day = dayptr->day;
                                int time = dayptr->time;
                                time++;                 //For days that have one task
                                usable_time = time;
                                usable_day = day;
                                return;
                            }
                            if (temp->counterpart != NULL) {
                                while (temp->counterpart) { //It travels to last task of day
                                    temp = temp->counterpart;
                                }
                                if (temp->time < 16) {
                                    int day = temp->day;
                                    int time = temp->time;  //If it is usable returns value
                                    time++;
                                    usable_time = time;
                                    usable_day = day;
                                    return;
                                }
                            }
                            if (dayptr->next->day - dayptr->day > 1) {
                                int day = dayptr->day;
                                day++;      //It searches for one or more free day and returns its day value
                                int time = 8;
                                usable_time = time;
                                usable_day = day;
                                return;
                            }
                            dayptr = dayptr->next;
                        }
                        if (dayptr->next == head) { //If it traveled all days instead of last day
                            if (dayptr->time < 16 && dayptr->counterpart == NULL) {
                                int day = dayptr->day;
                                int time = dayptr->time;  //If it is usable returns value
                                time++;
                                usable_time = time;
                                usable_day = day;
                                return;
                            }
                            while (dayptr->counterpart) {
                                dayptr = dayptr->counterpart; //Goes to last appointment of a day
                            }
                            if (dayptr->time < 16) {
                                int day = dayptr->day;
                                int time = dayptr->time;    //If it is usable returns value
                                time++;
                                usable_time = time;
                                usable_day = day;
                                return;
                            } else {
                                int day = dayptr->day;
                                day++;              //If it is not usable returns day which is following day of pointer
                                int time = 8;
                                usable_time = time;
                                usable_day = day;
                                return;
                            }
                        }

                    }
                }
                new_ptr = new_ptr->next;
            }
        }
    }
}


void WorkPlan::delayAllTasksOfDay(int day) {
    Task *ptr = head;
    if (day_num == 1 && day == head->day) { //If there is only one day
        int day = ptr->day;                 //It updates all task's days to day+1
        day++;
        int time = 8;
        ptr->time = 8;
        ptr->day = day;
        while (ptr->counterpart != NULL) {
            ptr = ptr->counterpart;
            ptr->day = day;
            time++;
            ptr->time = time;
        }
        return;
    }
    while (ptr->day <= day) { //It starts to travel all days to find day that is needed
        if (ptr->day == day) {
            if (ptr->next == head) {
                int day = ptr->day;   //It updates all task's days to day+1
                day++;                //Because it is last day
                int time = 8;
                ptr->day = day;
                ptr->time = time;
                while (ptr->counterpart != NULL) {
                    ptr = ptr->counterpart;
                    ptr->day = day;
                    time++;
                    ptr->time = time;
                }
                return;
            }
            Task *day_ptr = ptr;
            while (ptr) {
                day_ptr = ptr;
                checkAvailableNextTimesFor(ptr); //It try to find available time
                if (getUsableDay() == day_ptr->day) { //If it is in the same day
                    Task *new_ptr = new Task();    //It builds a virtual appointment at 17 o'clock
                    new_ptr->day = getUsableDay();
                    new_ptr->time = 17;
                    new_ptr->priority = ptr->priority;
                    new_ptr->name = new char[strlen(ptr->name)];
                    new_ptr->name = ptr->name;
                    new_ptr->next = NULL;
                    new_ptr->previous = NULL;
                    add(new_ptr);
                    checkAvailableNextTimesFor(new_ptr); //Checks available times for virtual appointment
                    remove(new_ptr);                     //Available day must be in another day
                    //It removes virtual appointment
                    Task *nptr = new Task();
                    nptr->day = getUsableDay();     //It creates new task and updates its values with usable times
                    nptr->time = getUsableTime();
                    nptr->priority = ptr->priority;
                    nptr->name = new char[strlen(ptr->name)];
                    nptr->name = ptr->name;
                    nptr->next = NULL;
                    nptr->previous = NULL;
                    add(nptr);
                    ptr = ptr->counterpart;
                    remove(day_ptr);

                } else {//If it is not in the same day
                    Task *new_ptr = new Task();
                    new_ptr->day = getUsableDay();
                    new_ptr->time = getUsableTime();    //It directly adds to proper time
                    new_ptr->priority = ptr->priority;
                    new_ptr->name = new char[strlen(ptr->name)];
                    new_ptr->name = ptr->name;
                    new_ptr->next = NULL;
                    new_ptr->previous = NULL;
                    add(new_ptr);
                    ptr = ptr->counterpart;
                    remove(day_ptr);
                }
            }
            return;
        }
        ptr = ptr->next;
    }
}

void WorkPlan::remove(Task *target) {
    Task *ptr = head;
    if (day_num == 1 && head->counterpart == NULL) {
        delete head;    //It deletes head and create new one
        create();
        return;
    }
    while (ptr->day <= target->day) {//It starts to travel all days
        if (ptr->day == target->day) {
            Task *tail = ptr;
            while (ptr->time <= target->time) {//It starts to travel all times of day
                if (ptr->time == target->time) {
                    if (tail == ptr) { //If it is first appointment of a day
                        if (ptr->counterpart == NULL) {
                            if (ptr == head) {
                                head = ptr->next;//It updates value of head
                            }
                            ptr->next->previous = ptr->previous; //It connects other tasks
                            ptr->previous->next = ptr->next;
                            delete ptr; //Deletes ptr
                            day_num--;
                            return;
                        } else {
                            if (ptr == head) {
                                head = ptr->counterpart; //It updates value of head
                            }
                            ptr->next->previous = ptr->counterpart;
                            ptr->previous->next = ptr->counterpart; //It connects other tasks
                            ptr->counterpart->next = ptr->next;
                            ptr->counterpart->previous = ptr->previous;
                            delete ptr;//Deletes ptr
                            return;
                        }
                    } else {
                        tail->counterpart = ptr->counterpart;
                        delete ptr;//Deletes ptr
                        return;
                    }
                }
                tail = ptr;
                ptr = ptr->counterpart;
            }
        }
        ptr = ptr->next;
    }
}

bool WorkPlan::checkCycledList() {
    Task *pivot = new Task();
    pivot = head;
    int patient = 100;
    bool r = false;
    while (pivot != NULL && patient > 0) {
        patient--;
        pivot = pivot->previous;
        if (pivot == head) {
            r = true;
            break;
        }
    }
    cout << "(" << 100 - patient << ")";
    patient = 100;
    bool l = false;
    while (pivot != NULL && patient > 0) {
        patient--;
        pivot = pivot->next;
        if (pivot == head) {
            l = true;
            break;
        }
    }
    return r & l;
}
