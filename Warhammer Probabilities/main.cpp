#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <cmath>
using std::pow;
using std::exp;

#include "Math/mathClass.h"

double determine_win_posibility(double p1, double p2, int n1, int n2, int turns);

int main()
{
	cout<<determine_win_posibility(1.0/(2*3*3), (double)(2*2*5)/(3*3*6), 30, 10, 6)<<endl;
	cout<<determine_win_posibility((double)(2*2*5)/(3*3*6), 1.0/(2*3*3), 10, 30, 6)<<endl;
	
	system("PAUSE");
	return 0;
}//end function main

double determine_win_posibility(double p1, double p2, int n1, int n2, int turns)
{
	Math function;
	int i, j;
	double sum, q1, q2;
	
	sum = 0;
	q1 = 1 - p1;
	q2 = 1 - p2;
	
	if(turns>1)
	{
		for(i=0;i<n2 && i<=n1;i++)
	    	for(j=0;j<n1 && j<=n2;j++)
	    	    sum += function.binomialCoefficient(n1,i) * pow(p1,(int)i) * pow(q1,(int)(n1-i))
					* function.binomialCoefficient(n2,j) * pow(p2,(int)j) * pow(q2,(int)(n2-j))
					* determine_win_posibility(p1, p2, n1-j, n2-i, turns-1);
	}//end if
	for(i=n2;i<=n1;i++)
		for(j=0;j<n1 && j<=n2;j++)
	    	sum += function.binomialCoefficient(n1,i) * pow(p1,(int)i) * pow(q1,(int)(n1-i))
				* function.binomialCoefficient(n2,j) * pow(p2,(int)j) * pow(q2,(int)(n2-j));
	return sum;
}//end function determine_win_posibility
