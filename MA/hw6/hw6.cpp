#include <iostream>
#include <stdlib.h>
#include<fstream>
#include<math.h>
using namespace std;
int main(){
    srand(10);
    ofstream pop("C:/Users/sean/Desktop/vs code MA/MA/hw6/pop_1W.csv");
    double beta[32],r; // 0-7 8-15 16-23 24-31
    for (int i = 0; i < 32;i++){
        beta[i] = 0.5;
    }
    int par[32], index = 7, count = 0;
    for (int i = 0; i < 10000;i++){
        pop << "pop" << "," << i+1 << endl;
        for (int j = 0; j < 32;j++){
            r = rand() / (double)(RAND_MAX + 1);
            if(r > beta[j]){
                par[j] = 1;
                count += pow(2, index);
            }else if(r <= beta[j]){
                par[j] = 0;
            }
            index--;
            pop << par[j] << ",";
            if(j%8 == 7){
                pop << count+1 <<endl;
                count = 0;
                index = 7;
            }
        }
    }
    system("pause");
    return 0;
}