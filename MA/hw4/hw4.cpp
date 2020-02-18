#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>    //setpercision
using namespace std;
int main(){
    //stringstream ss;
    string tmp,date,stock;
    double ema_0[256] = {0},alpha,ema0_sum = 0;
    double price[1742];
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw4/00501101-1512.csv");
    ofstream ema("C:/Users/sean/Desktop/vs code MA/MA/hw4/ema.csv");
    ema_0[255] = 0;
    ema << "date,1";
    for (int n = 2; n <= 256;n++){
        ema  <<',' << n ;
    }
    ema << endl;
    for (int i = 0; i < 1742;i++){  //總資料數
        getline(ma,date,',');
        getline(ma, stock, ',');
        getline(ma, tmp, '\n');
        price[i] = stod(stock);     //該天價錢
        if(i <= 256 && i > 0){  
            for (int j = 0; j <= i-1;j++){
                ema0_sum += price[j];
            }
            ema_0[i-1] = ema0_sum / (double)i;    //算EMA0(1-256)
            ema0_sum = 0;
        }
        if(i >= 2){                       //第二天開始更新有值的ema_0
            for (int j = 0; j < i-1;j++){
                if(j == 256){             //只有256個
                    break;
                }
                alpha = 2/((double)j + 2);
                ema_0[j] = ema_0[j] + alpha * (price[i] - ema_0[j]);   
            }
        }
        
        if (i >= 255){
            ema << date << ',';
            for (int j = 0; j < 256; j++){         //輸出EMA 1-256
                ema << setprecision(10) << ema_0[j] ;
                ema << ',';
            }
            ema << endl;
        }
    }
    ema.close();
    system ("pause");
    return 0;
}    