#include <fstream>
#include <iostream>
#include "ListBuffer.h"

ListBuffer::ListBuffer() {
    head = new node();
    head -> next = NULL;
}

ListBuffer::~ListBuffer(){
    node *p = head;
    node *tmp;
    while(p->next!=NULL) {
        tmp = p;
        p = p -> next;
        delete tmp;
    }
    delete p;
}

void ListBuffer::insertLine(int line_idx, const string &text){ 
    node *p = head;
    node *prev = head;
    while(p != NULL && p->line < line_idx){
        prev = p;
        p =  p -> next;
    }
    if(p != NULL && line_idx == p -> line) {
        p -> statement = text;
        return;
    } 
    prev -> next = new node(line_idx, text, p);
}

void ListBuffer::showLines() const {
    node *p = head -> next;
    while(p != NULL){
        cout << p -> line << '\t' << p -> statement << '\n';
        p = p -> next;
    }
}

void ListBuffer::deleteLine(int line_idx) {
    node *p = head -> next;
    node *prev = head;
    while( p != NULL) {
        if( p-> line == line_idx) {
            prev -> next = p -> next;
            delete p;
            return;
        }
        prev = p;
        p = p-> next;
    }
}

void ListBuffer::writeToFile(const string &filename) const {
    ofstream fileOutputStream(filename);
    node *p = head;
    while(p->next != NULL) {
        p = p -> next;
        fileOutputStream << p->line << '\t' << p->statement << '\n';
    }
    int bytes = fileOutputStream.tellp();
    cout << "saved to " << filename << ", " << bytes << " bytes written" << endl;
    fileOutputStream.close();
}