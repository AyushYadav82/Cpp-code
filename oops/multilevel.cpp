#include<iostream>
#include<string>
using namespace std;
class Person{
    public:
    string name;
    int age;
};
class student :public Person{
    public:
    int rollno;
     
};
class gradstu : public student{
    public:
    string researchArea;
};

int main(){
    gradstu s1;
    s1.name="ayush";
    s1.researchArea="physics";
    cout<<s1.name<<endl;
    cout<<s1.researchArea<<endl;

}