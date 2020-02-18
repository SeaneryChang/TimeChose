#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
using namespace std;
int main(){
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/train_1Y/00501101-1512.csv");
    ofstream sma("C:/Users/sean/Desktop/vs code MA/MA/train_1Y/sma.csv");
    //ofstream trad("C:/Users/sean/Desktop/vs code MA/MA/hw8/prof.csv");
    ofstream gb("C:/Users/sean/Desktop/vs code MA/MA/train_1Y/gbest.csv");
    int init_m = 1000000,stock_n,remain,a = 0,prof = 1000000,count=0,skip = 0,best_tmp,rec = 0;
    double ma_sum = 0,man;
    string date, stock, tmp,m_tmp,sma_tmp;
    double price[1742];
    stringstream ss;
    int num_block = 12;
    int block_day[12],block_sum = 0;
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
                if (skip == num_block+1){
                    break;
                }
                if(skip > 0){
                    block_day[skip - 1] = count+1;
                    block_sum += count;
                }
                count = 0;
                m_tmp = tmp;
                skip += 1;
            }
        }
        if (a == 1){
            sma << date << ",";
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
    cout << block_sum << "yee" << endl;
    ifstream sma_in("C:/Users/sean/Desktop/vs code MA/MA/train_1Y/sma.csv");
    double ** sma_file;
    double beta[32],r,best_prof,worst_tmp,delta = 0.002;
    int par[32], index , sum_count = 0,update = 0,stre[4],b,beta_gbest[32],beta_worst[32],best_bs[4],worst_bs[4];
    sma_file = new double * [block_sum];
    for (int i = 0; i < block_sum+1;i++){                           //01
        getline(sma_in, sma_tmp, ',');
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
    m_price = new double[block_sum+1];          
    for (int i = 0; i < block_sum+1;i++){
        m_price[i] = price[483 + i];                              //不含上個月最後一天
    }
    int accu = 0;
    for (int block = 0; block < num_block;block++){                //區間
        srand(10);
        best_prof = 0;
        index = 7;
        b = 0;
        for (int i = 0; i < 32; i++){                            //beta重置
            beta[i] = 0.5;
        }
        for (int n = 0; n < 10000; n++){                         //10000代
            best_tmp = 0;
            worst_tmp = 20000000;
            for (int i = 0; i < 300; i++){                     //300粒子
                for (int j = 0; j < 32; j++){                  // b1 b2 s1 s2
                    r = rand() / (double)(RAND_MAX + 1);
                    if (r > beta[j]){                          //產生0 1
                        par[j] = 1;
                        sum_count += pow(2, index);
                    }
                    else if (r <= beta[j]){
                        par[j] = 0;
                    }
                    index--;
                    if (j % 8 == 7){                           //算指數
                        stre[j / 8] = sum_count;               //其實是要+1 但後面用陣列剛好  從0開始
                        sum_count = 0;
                        index = 7;
                    }
                }
                for (int j = 1; j < block_day[block]; j++){    //(第一天跟上個月底比)到最後一天找買賣時機
                    if (sma_file[j+accu][stre[0]] > sma_file[j+accu][stre[1]] && sma_file[j + accu - 1][stre[0]] <= sma_file[j + accu - 1][stre[1]] && b == 0)
                    {
                        stock_n = int(prof / m_price[j+accu-1]);
                        remain = int(prof - (stock_n * m_price[j+accu-1]));
                        b = 1;
                        // if(block < 5){
                        //     cout << block << " " << j << " " << m_price[j + accu - 1] << "  buy" << endl;
                        // }
                    }
                    else if (sma_file[j+accu][stre[2]] < sma_file[j+accu][stre[3]] && sma_file[j +accu - 1][stre[2]] >= sma_file[j + accu - 1][stre[3]] && b == 1)
                    {
                        prof = int(stock_n * m_price[j+accu-1]) + remain;
                        b = 0;
                        // if(block < 5){
                        //     cout << block << " " << j << " " << m_price[j + accu - 1] << "  sell" << endl;
                        // }
                    }
                    if (j == block_day[block] - 1 && b == 1){
                        prof = int(stock_n * m_price[j+accu-1]) + remain;
                        b = 0;
                        // if(block < 5){
                        //     cout << block << " " << j << " " << m_price[j + accu - 1] << "  //sell" << endl;
                        // }
                    }
                }
                if (prof > best_tmp){                      //更新該世代best
                    best_tmp = prof;
                }
                if (prof > best_prof){                        //更新gbest
                    best_prof = prof;
                    for (int j = 0; j < 32; j++){
                        beta_gbest[j] = par[j];             //gbest 01矩陣
                    }
                    for (int j = 0; j < 4; j++){
                        best_bs[j] = stre[j];                //4個交易策略
                    }
                }
                if (prof <= worst_tmp){ //更新worst
                    worst_tmp = prof;
                    for (int j = 0; j < 32; j++){
                        beta_worst[j] = par[j];
                    }
                    for (int j = 0; j < 4; j++){
                        worst_bs[j] = stre[j];
                    }
                }
                //trad << i+1 << "," << prof << "," << (double(prof) - double(init_m)) / double(init_m) * 100.0 << endl;
                prof = 1000000;
            }                                            //300粒子結束
            if (rec != best_tmp)                         //30代收斂
            {
                update = n;
                rec = best_tmp;
            }
            if (n - update > 30)                        //30代跳脫
            {
                cout << n;
                gb << n << endl;
                break;
            }
            for (int i = 0; i < 32; i++)
            {
                if (beta_gbest[i] == 1 && beta_worst[i] == 0 && beta[i] >= delta){ //更新beta
                    beta[i] -= delta;
                }else if (beta_worst[i] == 1 && beta_gbest[i] == 0 && beta[i] <= 1-delta)
                {
                    beta[i] += delta;
                }
            }
            // 
            // 
            
        } //10000代結束
    gb << "Best" << endl;
    for (int i = 0; i < 4; i++){
        gb << best_bs[i]+1 << endl;
    }
    gb << setprecision(8) << best_prof <<endl;
    gb << "Best" << endl;
    for (int i = 0; i < 32;i++){
        gb << beta_gbest[i] << ",";
        if(i%8 == 7){
            gb << endl;
        }
    }
    gb << endl;
    for (int i = 0; i < 32;i++){
        gb << beta[i] << ",";
        if(i%8 == 7){
            gb << endl;
        }
    }
    accu += block_day[block];
}
    system("pause");
} 