#include<iostream>
#include<vector>
using namespace std;
int issorted(vector<int> arr,int n){
    if(n==0 || n==1){
        return true;
    }
    
    return arr[n-1] >=arr[n-2] && issorted(arr,n-1);
}
int main(){
    vector<int> arr={1,2,3};
    cout<<issorted(arr,arr.size());
    return 0;
}