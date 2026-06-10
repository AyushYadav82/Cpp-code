#include<iostream>
#include<vector>
using namespace std;
int main(){
    vector<int> vec={1,2,3};
    // cout<<vec[0]<<endl;
    cout<<"size="<<vec.size()<<endl;
    vec.push_back(25);
    cout<<"after push back size="<<vec.size()<<endl;
    vec.pop_back();
     cout<<"after pop back size="<<vec.size()<<endl;
     cout<<vec.front()<<endl;
     cout<<vec.at(1)<<endl;
     cout<<vec.back()<<endl;
    for(int i: vec){
        cout<<i<<endl;
    }
    return 0;
}