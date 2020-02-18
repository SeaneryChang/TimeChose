#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>
using namespace std;
int main(){
    stringstream ss;
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw5/00501101-1512.csv");
    ofstream file("C:/Users/sean/Desktop/vs code MA/MA/hw5/MtM.csv");
    int init,init_tmp,a,b,test_d = 0,train_d = 0;
    string type,d[1742],date,stock,tmp,m_tmp = "";
    //float price[1742]; 
    cin >> init;
    cin >> type;
    for (int i = 0; i < 1742;i++){  //總資料數
        getline(ma,date,',');
        getline(ma, stock, ',');
        getline(ma, tmp, '\n');
        d[i] = date;     //該天日期
        //ss << date;
        //getline(ss, tmp, '/');
        //getline(ss, tmp, '/');         //tmp為月份
        //d[i] = tmp;
        
        //price[i] = stod(stock);
        // if(m_tmp != tmp){              //只要月份不同就重置
        //     days[m_num] = d_tmp;
        //     m_num++;
        //     d_tmp = 1;
        //     m_tmp = tmp;              //更新月份
        // }else{
        //     d_tmp++;
        // }
        // days[m_num] = d_tmp;
    }
    switch(type[0]){
        case 'M':
            a = 1;
            break;
        case 'Q':
            a = 3;
            break;
        case 'H':
            a = 6;
            break;
        case 'Y':
            a = 12;
            break;
        default:
            a = 0;
            break;
    }
    switch(type[1]){
        case 'M':
            b = 1;
            break;
        case 'Q':
            b = 3;
            break;
        case 'H':
            b = 6;
            break;
        case 'Y':
            b = 12;
            break;
        default:
            b = 0;
            break;
    }
    
    while(init < 1742){
        init_tmp = init;
        while(a != -1){
            if(d[init_tmp-1] != m_tmp){
                a--;
                m_tmp = d[init_tmp - 1];
            }
            init_tmp--;
            train_d++;
        }
        train_d--;
        file << "Train" << "," << d[init_tmp] << "," << train_d << endl;

        while(b != -1){
            if(d[init] != m_tmp){
                b--;
                m_tmp = d[init];
            }
            init++;
            test_d++;
        }
        
        test_d--;
        file << "test" << "," << d[init] << "," << test_d << endl;
        
        //init += test_d;
        train_d = 0;
        test_d = 0;
    }
    system("pause");
    return 0;
}