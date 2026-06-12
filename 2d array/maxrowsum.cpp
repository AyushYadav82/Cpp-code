#include<iostream>
#include <climits>
using namespace std;
int maxsum(int mat[][3],int rows,int cols){
    int maxrowsum=INT_MIN;
         for(int i=0;i<rows;i++){
             int rowsum=0;            //it is here because it willbe set to 0 after every row
        for(int j=0;j<cols;j++){
           rowsum +=mat[i][j];
        }
          maxrowsum=max(maxrowsum,rowsum);    //call the fn
    }
  return maxrowsum;
    }
int main(){
    int matrix[3][3]={{1,2,3},{4,5,6},{7,8,9}};
    int rows=3;
    int cols=3;
    cout<<maxsum(matrix,rows,cols)<<endl;
    return 0;
}