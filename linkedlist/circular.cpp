#include<iostream>
using namespace std;

class Node{
    public:
    int data;
    Node* next;
    Node(int val){
        data=val;
        next=NULL;
    }
};

class CircularList{
    Node* head;
    Node* tail;

    public:
    CircularList(){
        head=tail=NULL;
    }
    void insertatHead(int val){
        Node* newNode=new Node(val);

        if(tail==NULL){
            head=tail=newNode;
            tail->next=head;
        }else{
            newNode->next=head;
            head=newNode;
            tail->next=head;
        }
    }
    void insertatTail(int val){
        Node* newNode=new Node(val);

        if(tail==NULL){
            head=tail=newNode;
            tail->next=head;
        }else{
            newNode->next=head;
            tail->next=newNode;
            tail=newNode;
        }
    }
    void deleteAthead(){
        if(head==NULL) return ;
        else if(head==tail){ //single
            delete head;
            head=tail=NULL;
        }else{ //2 or more
            Node* temp=head;
            head=head->next;
            tail->next=head;

            temp->next=NULL;
            delete temp;
        }
    } 
    void deleteAtTail(){
        if(head==NULL) return ;
        else if(head==tail){ //single
            delete head;
            head=tail=NULL;
        }else{
            Node* temp=tail; //2 or more
            Node* prev=head;
            while(prev->next!=tail){
                prev=prev->next;
            }
            tail=prev;
            tail->next=head;
            temp->next=NULL;
            delete temp;
        }
    }
    void print(){
        if(head==NULL) return ;

        cout<<head->data<<"->";
        Node* temp=head->next;

        while(temp!= head){
            cout<<temp->data<<"->";
            temp=temp->next;
        }
        cout<<temp->data<<endl;
    }
};

int main(){
     CircularList cll;
    //  cll.insertatHead(1);
    //  cll.insertatHead(2);
    //  cll.insertatHead(3);
    cll.insertatTail(1);
    cll.insertatTail(2);
    cll.insertatTail(3);
    // cll.deleteAthead();
    cll.deleteAtTail();
     cll.print();
     return 0;
}