#include <iostream>
using std::wcin;
using std::wcout;
using std::endl;

#include <cmath>
using std::pow;

#include <Math/mathematical functions.h>
using Math::MathematicalFunctions::BernsteinTriangle;


double determine_win_posibility(double p1, double p2, int n1, int n2, int turns);
double determine_win_posibility_worker(int n1, int n2, int turns);

int main()
{
	wcout << determine_win_posibility(1.0/(2*3*3)/* gaunt */, (double)(2*2*5)/(3*3*6)/* marine */, 30, 10, 6) << endl;
	wcout << determine_win_posibility((double)(2*2*5)/(3*3*6)/* marine */, 1.0/(2*3*3)/* gaunt */, 10, 30, 6) << endl;
	
	system("PAUSE");
	return 0;
} // end function main


/*	return: possibility unit 1 deffeats unit 2 before game ends.
 *	p1: probability an attack from a unit from 1st group will kill a unit of the 2nd group
 *	p2: probability an attack from a unit from 2nd group will kill a unit of the 1st group
 *	n1: units of type 1
 *	n2: units of type 2
 *	assume: each unit has exactly 1 attack per turn.
 *	assume: all bases touch all bases (all attacks can harm any enemy unit)
 *	assume: all units have same initiative (attack simultaneously. all attacks made before models removed)
 *	assume: close combat begins at 1st turn and no shouting preceeds it.
 *	This refers to close combat (both players attack in every turn)
 *	Each attack is independent of all others.
 *	ignore:	extra attack from charge.
 */

BernsteinTriangle *triangle1;
BernsteinTriangle *triangle2;

double determine_win_posibility(double p1, double p2, int n1, int n2, int turns)
{
	BernsteinTriangle bernsteinTriangle1(p1,n1);
	BernsteinTriangle bernsteinTriangle2(p2,n2);

	triangle1 = &bernsteinTriangle1;
	triangle2 = &bernsteinTriangle2;

	return determine_win_posibility_worker(n1,n2,turns);
} // end function determine_win_posibility


double determine_win_posibility_worker(int n1, int n2, int turns)
{
	int i, j;
	double sum = 0;
	
	// possibility to win = possibility to win later...
	if(turns>1)	// if not last turn of game
	{
		// i: units of type 2 that will die
		// j: units of type 1 that will die
		for(i = 0 ; i<n2 && i<=n1 ; i++)	// at least 1 unit of type 2 survives and no more than all attacks succeed
	    	for(j = 0 ; j<n1 && j<=n2 ; j++)	// at least 1 unit of type 1 survives and no more than all attacks succeed
				sum += (*triangle1)(n1,i)	// possibility exactly i unit 1 attacks succeed...
					* (*triangle2)(n2,j)	// ...and exactly j unit 2 attacks succeed...
					* determine_win_posibility_worker(n1-j, n2-i, turns-1);	// ...and unit 1 defeats unit 2 later but not before game ends.
	} // end if
	// ... + possibility to win now
	for(i = n2 ; i<=n1 ; i++)	// at least enough attacks to kill all enemy models need to succeed.
		for(j = 0; j<n1 && j<=n2 ; j++)	// a least 1 model of 1st unit must survive.
	    	sum += (*triangle1)(n1,i)	// possibility exactly i unit 1 attacks succeed...
				* (*triangle2)(n2,j);	// ...and exactly j unit 2 attacks succeed.
	return sum;
} // end function determine_win_posibility_worker
