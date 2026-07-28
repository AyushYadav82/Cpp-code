#include<iostream>
#include<string>
using namespace std;
class student{
    public :
    string name;
    double * cgpaPtr;
    student(string name,double cgpa){
        this->name=name;
        cgpaPtr=new double;
        *cgpaPtr=cgpa;
    }
    //destructor
    ~student(){
        cout<<"destructor"<<endl;
        delete cgpaPtr; //it deletes the dynamilcally allocated memory
    }
    void getInfo(){
        cout<<"name : "<<name<<endl;
        cout<<"cgpa : "<<*cgpaPtr<<endl;
    }
};
int main(){
    student s1("ayush",8.5);
    s1.getInfo();
    return 0;
}