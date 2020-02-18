#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>    //setpercision
using namespace std;
int main(){
    stringstream ss;
    string tmp,date,stock;
    double a = 0,ma_sum = 0,wma_sum = 0,wma_under = 0; //a決定開始輸出 MA
    double price[1742];
    double man;
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw3/00501101-1512.csv");
    ofstream sma("C:/Users/sean/Desktop/vs code MA/MA/hw3/sma.csv");
    ofstream wma("C:/Users/sean/Desktop/vs code MA/MA/hw3/wmaNEW.csv");
    //getline(ma, tmp, '\n');
    // for (int i = 0; i < 256;i++){
    //     wma << i+1;
    //     i == 255 ? wma << endl : wma << ',';
    // }
    sma << "date";
    //wma << "date";
    for (int n = 0; n < 256;n++){
        sma  << n << "," ;
        //wma  << n << "," ;
    }
    sma << endl;
    //wma << endl; 
    for (int i = 0; i < 1742;i++){
        getline(ma,date,',');
        getline(ma, stock, ',');
        getline(ma, tmp, '\n');
        // ss << stock;
        // ss >> price[i];
        // ss.clear();

        // istringstream buffer(stock);
        // price[i];
        // buffer >> price[i];
        // buffer.clear();
        price[i] = stod(stock);
        if(i == 255){
            a = 1;
        }
        if (a == 1){
            
            for (int j = 1;j <= 256;j++){      //開始算每天的MA1-256
                for (int k = 0; k < j;k++){    //MA(j)
                    ma_sum += price[i - k];
                }
                man = ma_sum / j;
                sma << man << setprecision(10);
                sma << ',';
                ma_sum = 0;
            }
            sma << endl;
            
            for (int j = 1;j <= 256;j++){      //開始算每天的WMA1-256
                for (int k = j; k > 0;k--){    //WMA(j) k從j~1
                    wma_sum += price[i - j + k]*(k);
                    wma_under += (k);
                }
                man = wma_sum / wma_under;
                wma << man << setprecision(8) << ',';
                wma_sum = 0;
                wma_under = 0;
            }
            wma << endl;
        }
    }
    sma.close();
    wma.close();
    system("pause");
}