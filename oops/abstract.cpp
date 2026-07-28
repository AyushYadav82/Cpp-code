#include<iostream>
#include<string>
using namespace std;
class Shape{ //abstract class 
    virtual void draw()=0; //pure virtual fn
};
class circle : public Shape{
    public :
    void draw(){
        cout<<"drawing a circle"<<endl;
    }
};
int main(){
    circle c1;
    c1.draw();
    return 0;
}
