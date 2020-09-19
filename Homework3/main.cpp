/* @Author
Student Name: Baþar Demir
Student ID: 150180080
Date: 20/12/2019 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct base_node;
struct Base_child_list;
struct Mobile_host_list;
struct network;
struct stack;
struct stack_node;

struct network {
    base_node *head; //It holds root of the network tree

    stack *s; //It is for getting path to base stations

    void create();//It initializes tree

    void add_base_station(base_node *node, base_node *parent); //It adds base station to the network

    base_node *search_node(int id, base_node *base); //It searches base stations

    void construct_network(string file);//It constructs network system as in text file

    base_node *create_base_node(int id); //It creates new base station

    Mobile_host_list *create_mobile_host(int id); //It creates new mobile host

    void add_mobile_host(Mobile_host_list *mobile_head, Mobile_host_list *mobile, base_node *parent);

    void send_message(string file); //It traverses base stations and mobile hosts

    static bool send(string message, int id, base_node *base, stack *s);//It sends messages as in text file

    void close(base_node *base);//It deletes all tree

};

struct Base_child_list {
    base_node *child_base; //It holds pointers of child base stations
    Base_child_list *next; //It points next child
};

struct base_node {
    int base_id; //It

    void add_MH(Mobile_host_list *ptr);//It adds mobile host to base station

    Mobile_host_list *mobile_head; //It is head of child mobile hosts

    Base_child_list *base_child_head; //It is head of child base stations
};

struct Mobile_host_list {
    int id; //Id of mobile host
    Mobile_host_list *next; //Points other mobile host
};

struct stack_node {
    int data; //Data value of stack node
    stack_node *next;
};

struct stack {
    stack_node *top;

    void create();//Initializes stack

    void close();//Closes stack

    void push(int data);//It data to stack

    int pop();//Gets element from stack
};

typedef network Datastructure;

Datastructure Network;

void network::create() {
    head = new base_node; //Function creates root of the tree
    head->base_id = 0;    //and constructs central controller
    head->base_child_head = NULL;
    head->mobile_head = NULL;
}

void base_node::add_MH(Mobile_host_list *ptr) { //It adds mobile hosts to the base stations
    Mobile_host_list *traverse = mobile_head;

    if (traverse == NULL) { //If base station has not any mobile host
        mobile_head = ptr; //It adds first mobile host
        return;
    }
    while (traverse->next) { //It searches for end node of
        traverse = traverse->next;// the mobile host linked list
    }
    traverse->next = ptr; //It adds mobile host to the linked list
}

//It gets new base station and its parent node as a parameter
void network::add_base_station(base_node *node, base_node *parent) {
    Base_child_list *traverse = parent->base_child_head; //It starts to traverse children of base stations
    if (traverse == NULL) { //If there is not a base station child
        traverse = new Base_child_list();//It adds first child
        parent->base_child_head = traverse;
        traverse->next = NULL;
        traverse->child_base = node;
        return;         //Function terminates
    }

    while (traverse->next) { //It traverses end node of linked list
        traverse = traverse->next;
    }
    traverse->next = new Base_child_list(); //It creates child
    traverse = traverse->next; //It adds to end of the base child list
    traverse->next = NULL;
    traverse->child_base = node;
    return;
}

base_node *network::search_node(int id, base_node *base) {//It is a recursive function
    if (id == base->base_id) { //If base is proper base, function returns
        return base;
    }
    Base_child_list *traverse = base->base_child_head;
    while (traverse) { //It starts to traverse all base children of the current base
        base_node *found = search_node(id, traverse->child_base); //It calls itself from new base
        if (found) {
            return found; //If search_node function returns any pointer, function returns
        }
        traverse = traverse->next;//It traverse next child
    }
    return NULL;//If it cannot found proper id it returns NULL
}

bool network::send(string message, int id, base_node *base, stack *s) {
    cout << base->base_id << " "; //It prints base station which is traversed
    Mobile_host_list *ptr = base->mobile_head;
    while (ptr) { //It searches base station's mobile hosts
        if (ptr->id == id) {    //If it finds mobile host
            cout << endl;       //It prints message in proper form
            cout << "Message:" << message << " To:0 ";
            return true;
        }
        ptr = ptr->next;//It updates ptr
    }

    Base_child_list *traverse = base->base_child_head;

    while (traverse) { //It starts to traverse children of base station

        if (send(message, id, traverse->child_base, s)) { //It calls itself and if function returns true
            s->push(traverse->child_base->base_id); //If base station is on the path to mobile host, it adds to stack
            return true;                               //It terminates function
        }
        traverse = traverse->next; //It updates traverse
    }
    return false; //If nothing found, it returns false
}

base_node *network::create_base_node(int id) {
    base_node *ptr = new base_node(); //It initializes base station node
    ptr->base_id = id;                //with proper id
    ptr->base_child_head = NULL;
    ptr->base_child_head = NULL;
    return ptr; //It returns new base station node
}

void network::construct_network(string file) { //It takes filepath as an argument
    ifstream text;
    text.open(file.c_str()); //It opens file
    string type;//It keeps type of input(MH/BS)
    int id;//It keeps id
    int parent; //It keeps parent of input

    if (!text.is_open()) {//If file does not find, it gives warning
        cout << file << " not found" << endl;
        return;
    }

    while (!text.eof()) { //It starts to read all file
        text >> type;
        if(type.length()<=1 || type ==""){
            continue;
        }
        text >> id;     //It takes inputs
        text >> parent;
        if (type == "BS") { //If type is BS
            base_node *node = create_base_node(id); //It creates base station node
            base_node *parent_node = search_node(parent, head); //It finds parent node
            add_base_station(node, parent_node); //It adds new base station to the its parent node
        }
        if (type == "MH") { //If type is MH
            Mobile_host_list *mobile = create_mobile_host(id);//It creates mobile host
            base_node *parent_node = search_node(parent, head);//It finds parent node
            add_mobile_host(parent_node->mobile_head, mobile, parent_node);//It adds new mobile host to the parent node
        }
    }
    text.close();//It closes network file
}

void network::add_mobile_host(Mobile_host_list *mobile_head, Mobile_host_list *mobile, base_node *parent) {
    Mobile_host_list *traverse = mobile_head;
    if (traverse == NULL) { //If parent base station has not mobile host
        traverse = mobile;  //It adds first mobile host
        parent->mobile_head = mobile;
        return;
    }
    while (traverse->next) { //It starts to traverse mobile hosts
        traverse = traverse->next;
    }
    traverse->next = mobile; //It adds mobile host to end of the mobile host list
}

void create_message(string &message, int &target, string input) { //It operates input message
    char delimiter = '>'; //Input is separated by >

    int pos = input.find(delimiter); //It finds position of delimiter

    message = input.substr(0, pos); //Message is string which is located before delimiter
    istringstream ss(input.substr(pos + 1, input.length() - 1).c_str());//Target is located after delimiter
    ss >> target; //it helps to convert string to integer
}

void network::send_message(string file) {
    ifstream text;
    text.open(file.c_str()); //It opens Messages file

    string input;
    if (!text.is_open()) { //If file does not find, it gives warning
        cout << file << " not found" << endl;
        return;
    }
    while (!text.eof()) { //It starts to read all file
        getline(text, input); //It reads file line by line
        if(input.length()==0){
            continue;
        }
        string message; //It keeps message
        int target; //It keeps receiver mobile host id

        create_message(message, target, input); //Function call for getting proper message form

        cout << "Traversing:";
        stack *s;
        s = new stack(); //It creates new stack
        s->create();
        if (!send(message, target, head, s)) { //It calls send function
            cout << endl;   //If there is not a mobile host id which is wanted, it gives different output
            cout << "Can not be reached the mobile host mh_" << target << " at the moment" << endl;
        } else {
            while (s->top) {
                cout << s->pop() << " "; //It prints path from central station to base station of mobile host
            }
            cout << "mh_" << target << endl;
        }
        s->close();//It closes stack
    }
    text.close();//It closes messages file
}

void network::close(base_node *base) {
    base_node *base_ptr = base; //It keeps pointer of base station
    Base_child_list *traverse = base->base_child_head; //It points children of base

    while (traverse) { //If current base station has base station child
        if (traverse->child_base) {
            close(traverse->child_base); //It calls function from child of current base station
            traverse = traverse->next; //It traverses all children of current base station
        }
    }

    Mobile_host_list *ptr = base->mobile_head; //It points mobile host of base station

    while (ptr) { //It traverses mobile hosts
        Mobile_host_list *tail = ptr;
        ptr = ptr->next;
        delete tail; //It deletes mobile hosts
    }
    delete base_ptr; //It deletes base station
}

Mobile_host_list *network::create_mobile_host(int id) {
    Mobile_host_list *mobile = new Mobile_host_list(); //It creates new mobile host
    mobile->id = id; //It assigns its id
    mobile->next = NULL;
    return mobile; //returns new mobile host
}

void stack::create() {
    top = NULL;  //It creates new stack
}

void stack::push(int data) {
    stack_node *s = new stack_node(); //It creates new stack node
    s->data = data;     //It assigns data to stack node
    s->next = top;      //Adds node to beginning of the list
    top = s;
}

int stack::pop() {
    int temp = top->data;   //It takes top element of stack
    stack_node *ptr = top;
    top = top->next;        //It updates top element
    delete ptr;
    return temp;        //Returns value of the top element
}

void stack::close() {
    stack_node *ptr = top;
    while (ptr) {       //It deletes all data and node stored in stack
        stack_node *temp = ptr;
        ptr = ptr->next;
        delete temp;
    }
}

int main(int argc, char *argv[]) {
    string network_file = argv[1];
    string messages_file = argv[2];

    Network.create();//It initializes tree

    Network.construct_network(network_file);//It constructs network system as in text file

    Network.send_message(messages_file);//It sends messages as in text file

    Network.close(Network.head);//It deletes all tree

    return 0;
}
