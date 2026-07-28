#include<iostream>
#include<string>
using namespace std;
class student{
    public :
    string name;
    double cgpa;
    student(string name,double cgpa){
        this->name=name;
        this->cgpa=cgpa;
    }
    void getInfo(){
        cout<<"name : "<<name<<endl;
        cout<<"cgpa : "<<cgpa<<endl;
    }
};
int main(){
    student s1("ayush",8.5);
    s1.getInfo();
    return 0;
}