#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
using namespace std;
int main(){
    srand(10);
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw3/00501101-1512.csv");
    ofstream sma("C:/Users/sean/Desktop/vs code MA/MA/hw7/sma.csv");
    ofstream trad("C:/Users/sean/Desktop/vs code MA/MA/hw7/prof.csv");
    int init_m = 1000000,stock_n,remain,a = 0,prof = 1000000,count=0,skip = 0;
    double ma_sum = 0,man;
    string date, stock, tmp,m_tmp,sma_tmp;
    double price[1742];
    stringstream ss;
    for (int i = 0; i < 1742;i++){
        getline(ma,date,',');
        getline(ma, stock, ',');
        getline(ma, tmp, '\n');
        
        price[i] = stod(stock);
        if(i == 483){
            a = 1;
            ss << date;
            getline(ss, tmp, '/');
            getline(ss, tmp, '/');
            ss.clear();
            m_tmp = tmp;
        }
        if(i > 483){
            ss << date;
            getline(ss, tmp, '/');
            getline(ss, tmp, '/');
            ss.clear();
            if(m_tmp != tmp){
                if (skip == 1){
                    break;
                }
                m_tmp = tmp;
                skip = 1;
            }
        }
        if (a == 1){
            for (int j = 1;j <= 256;j++){      //開始算每天的MA1-256
                for (int k = 0; k < j;k++){    //MA(j)
                    ma_sum += price[i - k];
                }
                man = ma_sum / j;
                sma << man << setprecision(8);
                sma << ',';
                ma_sum = 0;
            }
            sma << endl;
            count++;
        }
    }
    sma.close();
    ifstream sma_in("C:/Users/sean/Desktop/vs code MA/MA/hw7/sma.csv");
    ofstream pop("C:/Users/sean/Desktop/vs code MA/MA/hw7/pop.csv");
    double ** sma_file;
    double beta[32],r;
    int par[32], index = 7, sum_count = 0,stre[4],b = 0;
    sma_file = new double * [count];
    for (int i = 0; i < count;i++){
        sma_file[i] = new double[256];
        for (int j = 0; j < 256;j++){
            if(j == 255){
                getline(sma_in, sma_tmp, '\n');
                sma_file[i][j] = stod(sma_tmp);
            }else{
                getline(sma_in, sma_tmp, ',');
                sma_file[i][j] = stod(sma_tmp);
            }
            
        }
    }
    double * m_price;
    m_price = new double[count];
    for (int i = 0; i < count;i++){
        m_price[i] = price[483 + i];
    }
    for (int i = 0; i < 32; i++){
        beta[i] = 0.5;
    }
    for (int i = 0; i < 10000;i++){
        for (int j = 0; j < 32;j++){     // b1 b2 s1 s2
            r = rand() / (double)(RAND_MAX + 1);
            if(r > beta[j]){
                par[j] = 1;
                sum_count += pow(2, index);
            }else if(r <= beta[j]){
                par[j] = 0;
            }
            index--;
            if(j%8 == 7){
                stre[j / 8] = sum_count;
                //pop << sum_count+1 <<endl;
                sum_count = 0;
                index = 7;
            }
        }
        for (int j = 1; j < count;j++){   //第二天到最後一天找買賣時機
            if(sma_file[j][stre[0]] > sma_file[j][stre[1]] && sma_file[j-1][stre[0]] <= sma_file[j-1][stre[1]] && b == 0){
                stock_n = int(init_m / m_price[j]);
                remain = int(init_m - (stock_n * m_price[j]));
                b = 1;
                cout << i << "--b--" << j << endl;
            }else if(sma_file[j][stre[2]] < sma_file[j][stre[3]] && sma_file[j-1][stre[2]] >= sma_file[j-1][stre[3]] && b == 1){
                prof = int(stock_n * m_price[j]) + remain;
                //init_m = prof; //賺的錢可以多買?
                b = 0;
                cout << i << "--s--" << j << endl;
            }
            if(j == count-1 && b == 1){
                prof = int(stock_n * m_price[j]) + remain;
                //init_m = prof; //賺的錢可以多買?
                b = 0;
                cout << i << "//--s--//" << j << endl;
            }
        }
        trad << i+1 << "," << prof << "," << (double(prof) - double(init_m)) / double(init_m) * 100.0 << endl;
        pop << stre[0] << "," << stre[1] << "," << stre[2] << "," << stre[3] << endl;
        prof = 1000000;
    }
        system("pause");
} 