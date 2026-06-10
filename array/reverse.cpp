#include<iostream>
using namespace std;
void reverse(int arr[],int size){
    int start=0,end=size-1;
    while(start<end){
        swap(arr[start],arr[end]);
        start++;
        end--;
    }
}
int main(){
    int arr[]={5,3,7,6,2};
    int size=5;
    reverse(arr,size);
    for(int i=0;i<size;i++){
        cout<<arr[i]<<" ";
    }
    return 0;
}