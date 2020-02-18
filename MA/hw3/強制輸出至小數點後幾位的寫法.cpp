#include<iostream>
#include<fstream>
#include<iomanip>							//must include this

using namespace std;

ofstream output1,output2;

int main()
{
	double pi = 3.1415826879;


	/////////////////////demo of cout/////////////////////
	cout << pi<< '\n';

	cout << fixed<<  setprecision(10);			//forcing cout to 10-bit after dot

	cout << pi<< '\n';

	/////////////////////demo of ofstream/////////////////////
	output1.open("default test.csv");
	output1 << pi<< '\n';
	output1.close();

	output2.open("10bit test.csv");
	output2 << fixed<<  setprecision(10);		//it also work on ofstream          you can put this code at begin of main function
	output2 << pi<< '\n';
	output2.close();

	return 0;
}