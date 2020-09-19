/* @Author
Student Name: Başar Demir
Student ID: 150180080
Date: 11/10/2019  */

using namespace std;
#include <iostream>   
#include <stdlib.h>
#include <string.h>
#include <fstream>


struct node {
	int size;
	int quant;
	node* next;
};

struct stock {
	node* head;
	int list_count;  //It counts nodes which are linked to list
	void create();	//Initialize linked list
	void add_stock(int);
	void sell(int);
	void current_stock(); //Prints all stocks to terminal
	void clear();	//Deletes linked list
};

stock LinkedList;

void stock::create() {
	head = NULL;		//Creates the head of linked list.
	list_count = 0;		//Counts node number of nodes
}

void stock::add_stock(int shoe_num) {		//Shoe_num means size of shoe which is given in input
	if (list_count == 0) {
		node* node_ptr = new node();		// If there is not any node, it creates node for head pointer
		head = node_ptr;
		node_ptr->size = shoe_num;
		node_ptr->quant = 1;
		node_ptr->next = NULL;
		list_count++;
		return;
	}
	if ((head->size) == shoe_num) {			// If head node's size equal to shoe size directly increase
		head->quant++;						// number of shoes 
		return;
	}
	if (head->size > shoe_num) {			// If input is smaller than the head, it creates new node
		node* node_ptr = new node();		// and makes new node head of linked list	
		node_ptr->quant = 1;
		node_ptr->size = shoe_num;
		node_ptr->next = head;
		head = node_ptr;
		list_count++;
		return;
	}
	else {
		node* ptr = head;
		if (list_count == 1) {
			if (head->size < shoe_num) {			//If there is only head node and input is bigger than head
				node* node_ptr = new node();	//it creates a node and link it to head's next
				head->next = node_ptr;
				node_ptr->size = shoe_num;
				node_ptr->quant = 1;
				node_ptr->next = NULL;
				list_count++;
				return;
			}
			if (head->size == shoe_num) {	    //If head's size equals to input size,
				head->quant++;					//it increase the quantity of head node
				return;
			}
		}
		while (ptr->next && (ptr->next->size <= shoe_num)) {	// It starts to traverse in linked list until
			ptr = ptr->next;									// it finds a node which is bigger than or equal to input size
			if (ptr->next == NULL) {						//It shows that ptr came to last node of linked list
				if (ptr->size < shoe_num) {
					node* node_ptr = new node();	//If input is bigger than all elements of linked list
					node_ptr->size = shoe_num;		//it creates a new node and links it to the last node
					node_ptr->quant = 1;
					node_ptr->next = NULL;
					ptr->next = node_ptr;
					list_count++;
					return;
				}
				if (ptr->size == shoe_num) {	//If last node's size equal to input
					ptr->quant++;				// increases quantity of last node
					return;
				}
			}
		}
		if (ptr->size == shoe_num) {	//If pointer points a node which has size equal to input 
			ptr->quant++;
			return;
		}
		else {							 //Now pointer points biggest element which is smaller than input
			node* node_ptr = new node();
			node_ptr->size = shoe_num;	  //It adds new node to end of the 
			node_ptr->quant = 1;		 //node which is pointed by pointer
			node_ptr->next = ptr->next;
			ptr->next = node_ptr;
			list_count++;
			return;
		}
	}
}

void stock::sell(int shoe_num) {
	if (list_count == 0) {
		cout << "NO_STOCK" << endl;	//If there is not any node it prints directly no stock
		return;
	}
	if (list_count == 1 && head->quant > 1) {  //It checks first node
		head->quant--;
		return;
	}
	if (list_count == 1 && head->quant == 1) {
		head = NULL;							//It deletes first node if its quantity equals to 1
		list_count = 0;
		return;
	}
	if (list_count > 1) {
		node* ptr = head;
		node* temp = ptr;
		if (head->size == shoe_num) { // It checks first node
			if (head->quant > 1) {
				head->quant--;
				return;
			}
			if (head->quant == 1) { 	//deletes head node
				node* temp = head;
				head = temp->next;
				delete temp;
				return;
			}
		}
		if (head->size > shoe_num) {	//If head's size bigger than iput size
			cout << "NO STOCK" << endl;	//it does not look other nodes
			return;
		}
		while (ptr->next && (ptr->next->size <= shoe_num)) {
			temp = ptr;											//It searches for proper shoe size
			ptr = ptr->next;
			if (ptr->next == NULL) {			//It means ptr come to last element and ther is
				if (ptr->size < shoe_num) {		//no iput sized shoe			
					cout << "NO STOCK" << endl;
					return;
				}
				if (ptr->size == shoe_num) { //It found shoe in last nodes
					if (ptr->quant > 1) {
						ptr->quant--;
						return;
					}
					if (ptr->quant == 1) {
						delete ptr;				//It deletes last node if its quant equals to one
						temp->next = NULL;
						list_count--;
						return;
					}
				}
			}
		}
		if (ptr->size != shoe_num) {
			cout << "NO STOCK" << endl;	//Shows bigger shoe size
			return;
		}
		else {
			if (ptr->quant == 1) {		//Deletes node
				temp->next = ptr->next;
				delete ptr;
				list_count--;
				return;
			}
			else {
				ptr->quant--;
				return;
			}
		}
	}
}

void stock::current_stock() {
	if (list_count == 0) {	//If there is not any node 
		return;				//it returns nothing
	}
	node* ptr = head;
	cout << ptr->size << ":" << ptr->quant << endl;
	while (ptr->next) {									//It prints all stock
		ptr = ptr->next;
		cout << ptr->size << ":" << ptr->quant << endl;
	}
}

void stock::clear() {
	node* ptr = head;
	while (head) {			//It deletes every node which is created
		ptr = head;
		head = head->next;
		delete ptr;
	}
	list_count = 0;
}

void read_from_file(FILE* fptr) {
	while (!feof(fptr)) {						//It takes inputs from file and assign them to read
		int read;
		int num = fscanf(fptr, "%d", &read);
		if (read == 0) {					//If it is zero, it prints all stock
			LinkedList.current_stock();
		}
		if (read < 0) {						//If it is negative, it tries to sell
			LinkedList.sell(-read);
		}
		if (read > 0) {						//If it positive, it adds new stock
			LinkedList.add_stock(read);
		}
	}
}

int main(int argc, char* argv[]) {
	char* file_name = argv[1];

	FILE* file_ptr;

	if (!(file_ptr = fopen(file_name, "r"))) {
		cout << "File cannot be found" << endl;
		return EXIT_FAILURE;
	}

	LinkedList.create();

	read_from_file(file_ptr);

	LinkedList.clear();

	fclose(file_ptr);

	return EXIT_SUCCESS;
}