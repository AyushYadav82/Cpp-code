#include<iostream>
#include<string>
using namespace std;
class Teacher{
    //properties/attributes
    private:
    double salary; //data hiding

    public:
    string name;
    string dept;
    string subjects;
    

    //methods/member fn
    void changeDept(string newDept){
        dept=newDept;
    }
    //setter
    void setsalary(double s){
        salary=s;
    }
    //getter
    double getsalary(){
        return salary;
    }
};
int main(){
    //objects
    Teacher t1;  //constructor called
    t1.name="ayush";
    t1.subjects="maths";
    t1.dept="computer science";
    t1.setsalary(25000);

   cout<<t1.name<<endl;
   cout<<t1.getsalary()<<endl;
    return 0;
}