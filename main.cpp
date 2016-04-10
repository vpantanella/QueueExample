//
//  main.cpp
//  QueueHW4
//
//  Created by Tori Pantanella on 7/28/15.
//  Copyright (c) 2015 Tori Pantanella. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <iomanip>

using namespace std;

const int columns = 8;
const int spacing[] = {7, 7, 7, 6, 8, 9, 9, 10};
const char* title1[] = {"Minute", "Cust", "Status", "Entry", "Minutes", "Service", "Wait   ", "Time(min)"};
const char* title2[] = {"Number", "Number", "", "Min", "Waiting", "Time Rem", "Time Rem", "Complete"};

void printTitles(){
    for( int i = 0; i < columns; ++i){
        cout<< setw(spacing[i])<< title1[i];
    }
    cout<< endl;
    for( int i = 0; i < columns; ++i){
        cout<< setw(spacing[i])<< title2[i];
    }
    cout<< endl;
    
}

int generateRandom(int maxVal){
    return rand() % maxVal + 1;
}

struct CustomerRecord{
    int customerNumber;
    int startMin;
    int serviceTime;
    };

void printRecord (CustomerRecord rec){
    cout<< " Customer Number:" << rec.customerNumber;
    cout << " startmin " << rec.startMin;
    cout << " serviceTime " << rec.serviceTime<< endl;
}

class Node {
public:
    Node () {}
    Node* putOneNode (Node*, CustomerRecord);
    CustomerRecord getUserData ();
    Node* getNextPointer ();
    void putNextPointer (Node*);
    Node* getPriorPointer ();
    void putPriorPointer (Node*);
    void decrementServiceTime();
private:
    CustomerRecord rec;
    Node* nextPtr;
    Node* priorPtr;
};

Node* Node::getNextPointer (){
    return nextPtr;
}
void Node::putNextPointer (Node* x){
    this->nextPtr = x;
}
Node* Node::getPriorPointer (){
    return priorPtr;
}
void Node::putPriorPointer (Node* x){
    this->priorPtr = x;
}
Node* Node::putOneNode (Node* x, CustomerRecord y) {
    Node* temp = NULL;
    if ((temp = new Node()) != 0) {
        temp->nextPtr = x;
        temp->priorPtr = NULL;
        temp->rec = y;
        
    }
    return temp;
}
CustomerRecord Node::getUserData() {
    return rec;
}

void Node::decrementServiceTime(){
    rec.serviceTime -= 1;
}

void printNode(Node* nd){
    cout<< nd << " " << nd -> getPriorPointer() << " " << nd-> getNextPointer() << endl;
    printRecord(nd -> getUserData());
}

class DoubleLinkList {  // My Container class
public:
    DoubleLinkList ();
    bool putNode (CustomerRecord);  // Admit to list
    void getListForward ();   // like DumpQueue - but iterative
    Node* getListHead ();	  //
    Node*  deleteHead (); // Serve - delete one Node
    bool isEmpty();
    ~DoubleLinkList();
    
private:
    Node* FwdListPointer;
    Node* RevListPointer;
};

void printQueue(DoubleLinkList list){
    Node* currentNode = list.getListHead();
    if( currentNode == NULL)
       cout<< "list is empty"<< endl;
    while (currentNode!= NULL) {
        printNode(currentNode);
        currentNode = currentNode -> getPriorPointer();
    }
    
}

DoubleLinkList::DoubleLinkList ()
{FwdListPointer = RevListPointer = NULL;}

Node* DoubleLinkList:: getListHead(){
    return RevListPointer;
}


bool DoubleLinkList::putNode (CustomerRecord s) {
    bool x = false;
    Node* temp1;
    Node* tempfwd;
    Node A;
    temp1 = A.putOneNode (FwdListPointer, s);
    if (temp1 != NULL)
    {	if (FwdListPointer == NULL && RevListPointer == NULL)
        FwdListPointer = RevListPointer = temp1;
    else
    {tempfwd = FwdListPointer;
        tempfwd->putPriorPointer (temp1);
        FwdListPointer = temp1;
    }
        x =true;
    }
    return x;
}

bool DoubleLinkList:: isEmpty(){
    return FwdListPointer == NULL && RevListPointer == NULL;
}


Node* DoubleLinkList::deleteHead () {
    if (isEmpty()){
        cerr << "Error no nodes to remove" << endl;
        exit(1);
    }
    if (FwdListPointer == RevListPointer){
        Node* temp = FwdListPointer;
        FwdListPointer = NULL;
        RevListPointer = NULL;
        return temp;
    }
    Node* temp = RevListPointer;
    RevListPointer = RevListPointer-> getPriorPointer();
    temp -> putPriorPointer(NULL);
    RevListPointer -> putNextPointer(NULL);
    return temp;
}

DoubleLinkList::~DoubleLinkList(){
    Node* iterator = FwdListPointer;
    while( iterator != NULL){
        Node* nextPointer = iterator -> getNextPointer();
      //  delete iterator;
        iterator = nextPointer;
        
    }
}

class CheckoutLine {
public:
    CheckoutLine(int tt, int mi, int st); //tt= totalTime, mi= MaxInterval, st= MaxServiceTime
    bool customerEnter(); //customer enters line, indicates true when a new customer needs to be added
    void addCustomer(); // adds a customer to the end of the line
    void clock(); //keeps track of time
    bool serviceComplete(); //only for head
    void removeHead();
    char currentStatus(Node*);
    bool simulationOver();
    void print();
    
    
private:
    int totalTime;
    int MaxInterval;
    int MaxServiceTime;
    DoubleLinkList Queue;
    int currentMin;
    int priorCustomerNum;
    int customerEntryTime;
    void generateNextCustomerEntryTime();

};

CheckoutLine::CheckoutLine(int tt, int mi, int st){
    totalTime = tt;
    MaxInterval = mi;
    MaxServiceTime = st;
    currentMin = 0;
    priorCustomerNum = 0;
    generateNextCustomerEntryTime();
    
}

void CheckoutLine:: generateNextCustomerEntryTime(){
    customerEntryTime = currentMin + generateRandom(MaxInterval);
}


bool CheckoutLine::customerEnter(){
    if (currentMin == customerEntryTime){
    return true ;
    }
    else return false;
}

void CheckoutLine:: addCustomer(){
    int serviceTime = generateRandom(MaxServiceTime);
    CustomerRecord rec1 = { priorCustomerNum += 1, currentMin, serviceTime};
    Queue.putNode(rec1);
    generateNextCustomerEntryTime();
}
//increment min
void CheckoutLine::clock(){
    currentMin += 1;
    if (!Queue.isEmpty()){
        Node* firstCustomer = Queue.getListHead();
        firstCustomer -> decrementServiceTime();
        
    }
}

bool CheckoutLine:: serviceComplete(){
    if (!Queue.isEmpty()){
    Node* firstCustomer = Queue.getListHead();
    CustomerRecord record1 = firstCustomer -> getUserData();
    bool serviceComplete = record1.serviceTime == 0;
    return serviceComplete;
    }
    else return false;
}

void CheckoutLine::removeHead(){
    Queue.deleteHead();
}

bool CheckoutLine:: simulationOver(){
    return (totalTime <= currentMin);
}

char CheckoutLine::currentStatus(Node* customer){
    if( customer == NULL){
        return 'N';
    }
    CustomerRecord rec1 = customer -> getUserData();
    if(rec1.startMin == currentMin){
        return 'E';
        }
    Node* listHead = Queue.getListHead();
    if( listHead == customer){
        return  'S';
    }
    return 'W';
    }

void CheckoutLine::print(){
 
    Node* customer = Queue.getListHead();
    int priorwaittimes = 0;
    while ( customer != NULL){
        int col = 0;
        cout << setw(spacing[col++]) << currentMin;
        CustomerRecord rec = customer -> getUserData();
        cout << setw(spacing[col++]) << rec.customerNumber;
        cout << setw(spacing[col++]) << currentStatus(customer);
        cout << setw(spacing[col++]) << rec.startMin;
        cout << setw(spacing[col++]) << currentMin - rec.startMin;
        cout << setw(spacing[col++]) << rec.serviceTime;
        cout << setw(spacing[col++]) << priorwaittimes;
        cout << setw(spacing[col++]) << priorwaittimes + rec.serviceTime;
        priorwaittimes += rec.serviceTime;
        cout<< endl;
        customer = customer -> getPriorPointer();
        
    }
    
    
}


int main(){
    srand (time(NULL));
    cout << " Enter the Simulation time, max Entry interval, max service time interval" << endl;
    
    if (cin.eof() || !cin.good())
    {cout << "Invalid input. "
		      << "Program terminating." << endl;
        exit (1);}
    
    int simtime, entrytime, servicetime;
        cin >> simtime >> entrytime >> servicetime;
    CheckoutLine line(simtime, entrytime, servicetime);
    printTitles();
    while(!line.simulationOver()){
        line.clock();
        if( line.customerEnter()){
            line.addCustomer();
        }
        line.print();
        if(line.serviceComplete()){
            line.removeHead();
        }
    }
    
    
    
    /*CustomerRecord rec1 = {1, 1, 1};
    DoubleLinkList Double;
    Double.putNode(rec1);
    printQueue(Double);
    cout << endl;
    Node* head = Double.deleteHead();
    printQueue(Double);
    printRecord(head->getUserData());
    cout<< "head" << head << endl;
    delete head;
    rec1 = {2, 2, 2};
    Double.putNode(rec1);
    printQueue(Double);
    rec1 = {3, 3, 3};
    Double.putNode(rec1);
    rec1 = { 4, 4, 4};
    Double.putNode(rec1);
    printQueue(Double);*/
    
    
    
    
    
}

