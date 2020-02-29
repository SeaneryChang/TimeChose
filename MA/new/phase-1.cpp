#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>
#include<math.h>
using namespace std;
void tr_te(int init,string d[],int typea,int typeb,string file_name,int *init_tr,int *tr_day,int *init_te,int *te_day,int *stop){
    //ofstream file("C:/Users/sean/Desktop/vs code MA/MA/new/train_test/" + file_name + ".csv");
    int count, m, sum = 0;
    *stop = 0;
    string mon,mon_tmp;
    //while(stop == 0){
        count = 0;
        m = 0;
        mon_tmp = "m";
        while(typea-m > -1){
            mon = d[init + sum - count - 1][1];
            count++;
            if(mon != mon_tmp){
                m++;
                mon_tmp = mon;
            }
        }
        *init_tr = init + sum - count + 1;
        *tr_day = count - 1;
        //file << "Train" << "," << d[init + sum - count + 1].substr(2) << "," << init + sum - count+1 << "," << count-1 << endl;
        count = 0;
        m = 0;
        mon_tmp = "m";
        while(typeb-m > -1){
            if(init + sum + count == 1742){
                *stop = 1;
                count++;
                m++;
                continue;
            }
            mon = d[init + sum + count][1];
            count++;
            if(mon != mon_tmp){
                m++;
                mon_tmp = mon;
            }
        }
        *init_te = init + sum;
        *te_day = count - 1;
        //file << "Test" << "," << d[init + sum].substr(2) << ","  << init + sum << "," << count-1 << endl;
        sum += (count-1);
        //file << endl;
    //}
    //file.close();
}
void block_type(string type, int *a,int *b){
    switch(type[0]){
        case 'M':
            *a = 1;
            break;
        case 'Q':
            *a = 3;
            break;
        case 'H':
            *a = 6;
            break;
        case 'Y':
            *a = 12;
            break;
        default:
            *a = 0;
            break;
    }
    switch(type[1]){
        case 'M':
            *b = 1;
            break;
        case 'Q':
            *b = 3;
            break;
        case 'H':
            *b = 6;
            break;
        case 'Y':
            *b = 12;
            break;
        default:
            *b = 0;
            break;
    }
}
int main(){
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/new/00501101-1512.csv");
    ofstream sma_out("C:/Users/sean/Desktop/vs code MA/MA/new/MA.csv");
    stringstream ss;
    string date, stock, tmp, type,d[1742],btype;
    double price[1742],ma_sum = 0,man,r;
    int init_tr, init_te, tr_day, te_day,re = 0,count = 0,init_day = 507,phase;
    //cin >> init_day;
    cin >> btype;
    cin >> phase;
    for (int i = 0; i < 1742;i++){
        getline(ma,date,',');
        getline(ma, stock, ',');
        getline(ma, tmp, '\n');
        ss << date;
        getline(ss, tmp, '/');
        getline(ss, tmp, '/');
        if(stoi(tmp) < 10){          
            tmp = "0" + tmp;
        }
        d[i] = tmp + date;                   //XXaaaa/bb/cc
        price[i] = stod(stock);
        if (i >= 256){
            sma_out << date << ',';
            for (int j = 1;j <= 256;j++){       //開始算每天的MA1-256
                for (int k = 0; k < j;k++){     //MA(j)
                    ma_sum += price[i - k];
                }
                man = ma_sum / j;
                
                sma_out << man << setprecision(8);
                sma_out << ',';
                ma_sum = 0;
            }
            sma_out << endl;
            count++;
        }else{
            sma_out << date << ",Non" << endl;
        }
        //cout << i << endl;
    }
    ma.close();
    ifstream sma_256("C:/Users/sean/Desktop/vs code MA/MA/new/MA.csv");
    ofstream gb("C:/Users/sean/Desktop/vs code MA/MA/new/output_2phase.csv");
    double ** sma_file;
    double beta[32],delta = 0.002;
    string sma_tmp;
    int beta_gbest[32], beta_worst[32], par[32], stre[4],best_bs[4],worst_bs[4];
    int sum_count, index,best_prof,b,prof = 1000000,stock_n,remain,best_tmp,worst_tmp,rec = 0,update = 0,x;

    
    
    sma_file = new double * [1742];               //從256天開始存 MA256 到最後一天
    for (int i = 0; i < 1742;i++){
        //cout << i;
        getline(sma_256, sma_tmp, ',');
        sma_file[i] = new double[256];
        for (int j = 0; j < 256;j++){
            //cout << j << endl;
            if(i < 256){
                getline(sma_256, sma_tmp, '\n');
                break;
            }
            if(j == 255){
                getline(sma_256, sma_tmp, '\n');
                sma_file[i][j] = stod(sma_tmp);
            }else{
                getline(sma_256, sma_tmp, ',');
                sma_file[i][j] = stod(sma_tmp);
            }
            
        }
    }
    ofstream pop("C:/Users/sean/Desktop/vs code MA/MA/new/pop.csv");
    //ofstream test("C:/Users/sean/Desktop/vs code MA/MA/new/test.csv");
    int c = 0,in = 0,b_tr,b_te;
    block_type(btype, &b_tr, &b_te);
    while(re == 0){  //直到區間結束 


    tr_te(init_day+c,d,b_tr,b_te,btype,&init_tr,&tr_day,&init_te,&te_day,&re);   //更改訓練期起始天
    
    cout << init_tr << " " << tr_day << " " << init_te << " " << te_day << endl; //加區間數的迴圈
    //     訓練起始           訓練區間         測試起始          測試區間
    //cout << "re = " << re << endl;
    x = init_tr;
    c += te_day;
    for (int i = 0; i < 32;i++){   //重製beta矩陣
        beta[i] = 0.5;
    }
    srand(10);
    best_prof = 0;
    index = 7;
    b = phase-1;
    for (int n = 0; n < 10000; n++){                        //1W代
        best_tmp = 0;
        worst_tmp = 1000000;
        for (int i = 0; i < 300; i++) {                        //300粒子
            b = 0;
            for (int j = 0; j < 32; j++){                  // b1 b2 s1 s2
                r = rand() / (double)(RAND_MAX + 1);
                if (r > beta[j]){
                    par[j] = 1;
                    sum_count += pow(2, index);
                }
                else if (r <= beta[j]){
                    par[j] = 0;
                }                                          // 0 or 1
                index--;
                if (j % 8 == 7){
                    stre[j / 8] = sum_count;
                    //pop << sum_count << ",";
                    sum_count = 0;
                    index = 7;
                }
            }
            
            //pop << endl;
            for (int j = 0; j < tr_day; j++){    //(第一天跟上個月底比)到最後一天找買賣時機
                if(j == 0 && b == 1){
                    if(b == 1){
                        stock_n = int(prof / price[j+x]);
                        remain = int(prof - (stock_n * price[j+x]));
                    }
                    if(i == 0 && n == 0){
                        cout << d[j] << endl;
                    }
                    continue;
                }
                if (sma_file[j+x][stre[0]] > sma_file[j+x][stre[1]] && sma_file[j +x- 1][stre[0]] <= sma_file[j +x- 1][stre[1]] && b == 0)
                {
                    stock_n = int(prof / price[j+x]);
                    remain = int(prof - (stock_n * price[j+x]));
                    b = 1;

                    
                }
                else if (sma_file[j+x][stre[2]] < sma_file[j+x][stre[3]] && sma_file[j +x- 1][stre[2]] >= sma_file[j+x- 1][stre[3]] && b == 1)
                {
                    prof = int(stock_n * price[j+x]) + remain;
                    b = 0;

                    
                }
                if (j == tr_day - 1 && b == 1){
                    if(b == 1){
                        prof = int(stock_n * price[j+x]) + remain;
                    }

                    
                    b = 0;
                }
            }
            if (prof > best_tmp){                       //更新該世代best
                best_tmp = prof;
            }
            if (prof > best_prof){                      //更新gbest
                best_prof = prof;
                for (int j = 0; j < 32; j++){
                    beta_gbest[j] = par[j];             //gbest 01矩陣
                }
                for (int j = 0; j < 4; j++){
                    best_bs[j] = stre[j];               //4個交易策略
                }
            }
            else if (prof <= worst_tmp){                 //更新worst
                worst_tmp = prof;
                for (int j = 0; j < 32; j++){
                    beta_worst[j] = par[j];
                }
                for (int j = 0; j < 4; j++){
                    worst_bs[j] = stre[j];
                }
            }
            //trad << i+1 << "," << prof << "," << (double(prof) - double(init_m)) / double(init_m) * 100.0 << endl;
            
            // if(n == 0){
            //     for (int e = 0; e < 32;e++){
            //         test << par[e] << ",";
            //         if(e%8 == 7){
            //             test << stre[(e+1)/8-1] << endl;
            //         }
            //     }
            //     test << endl;
            // }
            
            
            prof = 1000000;
        }                                             //------------300 粒子結束
        if (rec != best_tmp){                        //30代收斂            
            update = n;
            rec = best_tmp;
        }
        if (n - update > 30){                        //30代跳脫
            //cout << n;
            gb << n << endl;
            in = 1;
            break;
        }

        for (int i = 0; i < 32; i++){
            if (beta_gbest[i] == 1 && beta_worst[i] == 0 && beta[i] >= delta){    //更新beta
                beta[i] -= delta;
            }else if (beta_worst[i] == 1 && beta_gbest[i] == 0 && beta[i] <= 1-delta){
                beta[i] += delta;
            }
        }
        // if(in ==0){                                   //測試第一區間每代
        //     pop << n << endl;
        //     pop << "best," << best_prof << ",worst," << worst_tmp << endl;
        //     for (int e = 0; e < 32;e++){
        //         pop << beta_gbest[e] << ",";
        //         if(e % 8 == 7){
        //             pop << best_bs[(e+1)/8-1] << endl;
        //         }
        //     }
        //     pop << endl;
        //     for (int e = 0; e < 32;e++){
        //         pop << beta[e] << ",";
        //         if(e % 8 == 7){
        //             pop << endl;
        //         }
                
        //     }
        //     pop << endl;
        // }
        
            
    }                               // -----------------10000代結束
    update = 0;                     //重製跳脫世代
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
    } //區間結束
    //tr_te(507+te_day,d,1,1,"MtM",&init_tr,&tr_day,&init_te,&te_day);
    sma_256.close();
    gb.close();
    pop.close();
    system("pause");
}