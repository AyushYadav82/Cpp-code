#include<iostream>
using namespace std;
int main(){
    int target,num;
int arr[50];
        cout<<"enter the no. of elements"<<endl;
        cin>>num;
        cout<<"enter the elements"<<endl;
        for(int i=0;i<num;i++){
            cin>>arr[i];
        }
        cout<<"Enter the target"<<endl;
        cin>>target;
        for(int i=0;i<num;i++){
            cout<<arr[i+1];
            

        }
        return 0;
    }