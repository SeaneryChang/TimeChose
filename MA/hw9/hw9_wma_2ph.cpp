#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
using namespace std;
int main(){
    srand(10);
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw3/00501101-1512.csv");
    ofstream wma("C:/Users/sean/Desktop/vs code MA/MA/hw9/wma.csv");
    //ofstream trad("C:/Users/sean/Desktop/vs code MA/MA/hw8/prof.csv");
    ofstream gb("C:/Users/sean/Desktop/vs code MA/MA/hw9/gbest.csv");
    int init_m = 1000000,stock_n,remain,a = 0,prof = 1000000,count=0,skip = 0,best_tmp,rec = 0;
    double ma_sum = 0,man,wma_sum,wma_under;
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
            count++;
        }
    }
    wma.close();
    ifstream wma_in("C:/Users/sean/Desktop/vs code MA/MA/hw9/wma.csv");
    double ** wma_file;
    double beta[32],r,best_prof = 0,worst_porf = 10000000,delta = 0.002;
    int par[32], index = 7, sum_count = 0,update = 0,stre[4],b = 0,beta_gbest[32],beta_worst[32],best_bs[4],worst_bs[4];
    wma_file = new double * [count];
    for (int i = 0; i < count;i++){
        wma_file[i] = new double[256];
        for (int j = 0; j < 256;j++){
            if(j == 255){
                getline(wma_in, sma_tmp, '\n');
                wma_file[i][j] = stod(sma_tmp);
            }else{
                getline(wma_in, sma_tmp, ',');
                wma_file[i][j] = stod(sma_tmp);
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
    for (int n = 0; n < 10000;n++){                          //10000代
        best_tmp = 0;
        worst_porf = 2000000;
        for (int i = 0; i < 300; i++){                       //300粒子
            for (int j = 0; j < 32; j++){ // b1 b2 s1 s2
                r = rand() / (double)(RAND_MAX + 1);
                if (r > beta[j]){
                    par[j] = 1;
                    sum_count += pow(2, index);
                }
                else if (r <= beta[j]){
                    par[j] = 0;
                }
                index--;
                if (j % 8 == 7){
                    stre[j / 8] = sum_count;
                    //pop << sum_count+1 <<endl;
                    sum_count = 0;
                    index = 7;
                }
            }
            for (int j = 1; j < count; j++){ //第二天到最後一天找買賣時機
                if(j == 1){
                    stock_n = int(prof / m_price[j]);
                    remain = int(prof - (stock_n * m_price[j]));
                    b = 1;
                }
                if (wma_file[j][stre[0]] > wma_file[j][stre[1]] && wma_file[j - 1][stre[0]] <= wma_file[j - 1][stre[1]] && b == 0){
                    stock_n = int(prof / m_price[j]);
                    remain = int(prof - (stock_n * m_price[j]));
                    b = 1;
                    //cout << i << "--b--" << j << endl;
                }else if (wma_file[j][stre[2]] < wma_file[j][stre[3]] && wma_file[j - 1][stre[2]] >= wma_file[j - 1][stre[3]] && b == 1){
                    prof = int(stock_n * m_price[j]) + remain;
                    //init_m = prof; //賺的錢可以多買?
                    b = 0;
                    //cout << i << "--s--" << j << endl;
                }
                if (j == count - 1 && b == 1){
                    prof = int(stock_n * m_price[j]) + remain;
                    b = 0;
                    //cout << i << "//--s--//" << j << endl;
                }
            }
            if(prof > best_tmp){
                best_tmp = prof;
            }
            if (prof > best_prof){ //更新gbest
                best_prof = prof;
                for (int j = 0; j < 32; j++){
                    beta_gbest[j] = par[j]; //gbest 01矩陣
                }
                for (int j = 0; j < 4; j++){
                    best_bs[j] = stre[j]; //4個交易策略
                }
            }
            if (prof <= worst_porf){ //更新worst
                worst_porf = prof;
                for (int j = 0; j < 32; j++){
                    beta_worst[j] = par[j];
                }
                for (int j = 0; j < 4; j++){
                    worst_bs[j] = stre[j];
                }
            }
            //trad << i+1 << "," << prof << "," << (double(prof) - double(init_m)) / double(init_m) * 100.0 << endl;
            prof = 1000000;
        }                    //300粒子結束
        if(rec != best_tmp){
            update = n;
            rec = best_tmp;
        }
        if(n - update > 30){
            cout << n;
            gb << n << endl;
            break;
        }
        for (int i = 0; i < 32;i++){
            if(beta_gbest[i] == 1 && beta_worst[i] == 0 ){            //更新beta
                beta[i] -= delta;
            }else if(beta_worst[i] == 1 && beta_gbest[i] == 0 ){
                beta[i] += delta;
            }
        }
        // && beta[i] >= delta
        // && beta[i] <= 1-delta
        if(n < 30){
            for (int p = 0; p < 32;p++){
                gb << beta[p] << ",";
                if(p%8 == 7){
                    gb << endl;
                }
            }
            gb << endl;
        }

        
    }                         //10000代結束
    gb << "Best" << endl;
    for (int i = 0; i < 4; i++){
        gb << best_bs[i]+1 << endl;
    }
    gb << setprecision(8) << best_prof << "," << worst_porf <<endl;
    gb << "Best" << endl;
    for (int i = 0; i < 32;i++){
        gb << beta_gbest[i] << ",";
        if(i%8 == 7){
            gb << endl;
        }
    }
    // gb << "Worst" << endl;
    // for (int i = 0; i < 32;i++){
    //     gb << beta_worst[i] << ",";
    //     if(i%8 == 7){
    //         gb << endl;
    //     }
    // }
    gb << endl;
    for (int i = 0; i < 32;i++){
        gb << beta[i] << ",";
        if(i%8 == 7){
            gb << endl;
        }
    }

    system("pause");
} 