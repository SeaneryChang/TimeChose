#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
using namespace std;
int main(){
    srand(10);
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw7/00501101-1512.csv");
    ofstream wma("C:/Users/sean/Desktop/vs code MA/MA/hw7/wma.csv");
    ofstream trad_wma("C:/Users/sean/Desktop/vs code MA/MA/hw7/prof_wma.csv");
    int init_m = 1000000,stock_n,remain,a = 0,prof = 1000000,count=0,skip = 0;
    string date,stock,tmp,m_tmp,sma_tmp;
    stringstream ss;
    double ** wma_file;
    double beta[32],r,price[1742],wma_sum,wma_under,wman;
    int par[32], index = 7, sum_count = 0,stre[4],b = 0;
    wma_file = new double * [count];
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
                for (int k = 0; k < j;k++){    //WMA(j)
                    wma_sum += price[i - k]*(k+1);
                    wma_under += (k+1);
                }
                wman = wma_sum / wma_under;
                wma << wman << setprecision(8) << ',';
                wma_sum = 0;
                wma_under = 0;
            }
            wma << endl;
            count++;
        }
    }
    wma.close();
    ifstream wma_in("C:/Users/sean/Desktop/vs code MA/MA/hw7/wma.csv");
    ofstream pop("C:/Users/sean/Desktop/vs code MA/MA/hw7/pop_wma.csv");
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
    for (int i = 0; i < count;i++){           //抓12月股價
        m_price[i] = price[483 + i];
    }
    for (int i = 0; i < 32; i++){
        beta[i] = 0.5;
    }
    for (int i = 0; i < 10000;i++){       //1萬個粒子
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
            if(wma_file[j][stre[0]] > wma_file[j][stre[1]] && wma_file[j-1][stre[0]] <= wma_file[j-1][stre[1]] && b == 0){
                stock_n = int(init_m / m_price[j]);
                remain = int(init_m - (stock_n * m_price[j]));
                b = 1;
                cout << i << "--b--" << j << " " << stre[0] << " " << stre[1] << endl;
                cout << wma_file[j][stre[0]] << " > " << wma_file[j][stre[1]] << " " << wma_file[j-1][stre[0]] << " <= " << wma_file[j-1][stre[1]] << endl;
            }else if(wma_file[j][stre[2]] < wma_file[j][stre[3]] && wma_file[j-1][stre[2]] >= wma_file[j-1][stre[3]] && b == 1){
                prof = int(stock_n * m_price[j]) + remain ;
                b = 0;
                cout << i << "--s--" << j << " " << stre[2] << " " << stre[3] << endl;
                cout << wma_file[j][stre[2]] << " < " << wma_file[j][stre[3]] << " " << wma_file[j - 1][stre[2]] << " >= " << wma_file[j - 1][stre[3]] << endl;
            }
            if(j == count-1 && b == 1){
                prof = int(stock_n * m_price[j]) + remain;
                b = 0;
                cout << i << "//--s--//" << j << endl;
            }

        }
        trad_wma << i+1 << "," << prof << "," << (double(prof) - double(init_m)) / double(init_m) * 100.0 << endl;
        pop << stre[0] << "," << stre[1] << "," << stre[2] << "," << stre[3] << endl;
        prof = 1000000;
    }
    trad_wma.close();
    system("pause");
}