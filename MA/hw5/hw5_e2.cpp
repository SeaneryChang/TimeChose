#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>
using namespace std;
void tr_te(int init,string d[],int typea,int typeb,string file_name){
    ofstream file("C:/Users/sean/Desktop/vs code MA/MA/hw5/train_test/" + file_name + ".csv");
    int count,m,sum = 0,stop = 0;
    string mon,mon_tmp;
    while(stop == 0){
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
        file << "Train" << "," << d[init + sum - count + 1].substr(2) << "," << init + sum - count+1 << "," << count-1 << endl;
        count = 0;
        m = 0;
        mon_tmp = "m";
        while(typeb-m > -1){
            if(init + sum + count == 1742){
                stop = 1;
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
        file << "Test" << "," << d[init + sum].substr(2) << ","  << init + sum << "," << count-1 << endl;
        sum += (count-1);
        file << endl;
    }
    file.close();
}
int main(){
    ifstream ma("C:/Users/sean/Desktop/vs code MA/MA/hw5/00501101-1512.csv");
    
    stringstream ss;
    string date, stock, tmp, type,d[1742];
    int init;
    //cin >> init;
    //cin >> type;
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
        d[i] = tmp + date;
        //cout << d[i] << endl;
    }
    
    tr_te(507,d,1,1,"MtM");
    tr_te(507,d,3,1,"QtM");
    tr_te(507,d,6,1,"HtM");
    tr_te(507,d,12,1,"YtM");
    tr_te(507,d,3,3,"QtQ");
    tr_te(507,d,6,3,"HtQ");
    tr_te(507,d,12,3,"YtQ");
    tr_te(507,d,6,6,"HtH");
    tr_te(507,d,12,6,"YtH");
    tr_te(507,d,12,12,"YtY");
    system("pause");
}