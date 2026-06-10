#include<iostream>
using namespace std;
int linearsearch(int arr[],int size,int target){ 
    for(int i=0;i<size;i++){
        if(arr[i]==target){
            return i;
        }
    }
    return -1;
}
    int main(){
    int arr[]={5,3,4,2,7};
    int size=5;
    int target=4;
    cout<<linearsearch(arr,size,target)<<endl;
    return 0;
    }