#include<iostream>
#include<vector>
#include<stack>
using namespace std;
int main(){
    stack<int> s;
    s.push(10);
    s.push(20);
    while(!s.empty()){
        cout<<s.top()<<" ";
        s.pop();
    }
    return 0;
}