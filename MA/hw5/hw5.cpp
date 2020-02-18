#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>
using namespace std;
//void sw(int init,int a,int b,string ab,int s[]){
    //
    
    //file.close();
//}
int main(){
    stringstream ss;
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw5/00501101-1512.csv");
    ofstream file("C:/Users/sean/Desktop/vs code MA/MA/hw5/MtM.csv");
    string date, stock, tmp,m_tmp = "0";
    string d[1742];
    int days[88],d_tmp = 0,m_num = 0,a,b,init_tmp = 0;
    int init,j = 1,date_num,train_d = 0,test_d = 0;;
    string type;
    cin >> init;
    cin >> type;
    for (int i = 0; i < 1742;i++){  //總資料數
        getline(ma,date,',');
        getline(ma, stock, ',');
        getline(ma, tmp, '\n'); 
        d[i] = date;     //該天日期
        ss << date;
        getline(ss, tmp, '/');
        getline(ss, tmp, '/');
        cout << tmp << endl;
        if(m_tmp != tmp){
            days[m_num] = d_tmp;
            m_num++;
            d_tmp = 1;
            m_tmp = tmp;
        }else{
            d_tmp++;
        }
        days[m_num] = d_tmp;
    }

    // for (int i = 1; i < 88; i++){
    //     cout << days[i] << endl;
    // }
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
    while(init != init_tmp){
        init_tmp += days[j];
        j++;
    }
    j--;
    //file << j;
    while(days[j-(b-1)] != 0){
        date_num = init;
        file << "Train" << ",";
        for (int tr = 1; tr <= a;tr++){
            train_d += days[j - tr];

        }
        file << d[date_num-train_d-1] << "," << date_num << "," << train_d  << endl;
        file << "Test" << "," << d[init] << "," << init << ",";
        for (int te = 0; te < b;te++){
            test_d += days[j + te];
            init += days[j + te];
        }
        file << test_d << endl;
        train_d = 0;
        test_d = 0;
        j += b;
    }
    //sw(init,a, b, type, days);
    ma.close();
    file.close();
    system("pause");
    return 0;
}