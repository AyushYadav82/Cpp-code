#include<iostream>
#include<string>
using namespace std;
class Teacher{
    public:
    string name;
    int age;
};
class student {
    public:
    int rollno;
     
};
class TA : public student,public Teacher{

};

int main(){
    TA t1;
    t1.name="Tony stark";
    t1.rollno=20;
    cout<<t1.name<<endl;
    cout<<t1.rollno<<endl;
    return 0;
}