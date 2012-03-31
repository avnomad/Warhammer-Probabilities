#include <iostream>
using std::wcin;
using std::wcout;
using std::endl;

#include <cmath>
using std::pow;

#include <vector>
using std::vector;

#include <algorithm>
using std::swap;

#include <Math/mathematical functions.h>
using Math::MathematicalFunctions::BernsteinTriangle;

#include <StopWatch.h>

double determine_win_posibility(double p1, double p2, unsigned nModels1, unsigned nModels2, unsigned turns);

int main()
{
	StopWatch sw("ms");
	double t;

	sw.push();
	wcout << determine_win_posibility(1.0/(2*3*3)/* gaunt */, (double)(2*2*5)/(3*3*6)/* marine */, 30, 10, 5) << endl;
	wcout << determine_win_posibility((double)(2*2*5)/(3*3*6)/* marine */, 1.0/(2*3*3)/* gaunt */, 10, 30, 5) << endl;
	t = sw.pop();
	std::cout << "Elapsed time: " << t << sw.getUnit() << "\n" << endl;

	sw.push();
	wcout << determine_win_posibility(1.0/(2*3*3)/* gaunt */, (double)(2*2*5)/(3*3*6)/* marine */, 30, 10, 6) << endl;
	wcout << determine_win_posibility((double)(2*2*5)/(3*3*6)/* marine */, 1.0/(2*3*3)/* gaunt */, 10, 30, 6) << endl;
	t = sw.pop();
	std::cout << "Elapsed time: " << t << sw.getUnit() << "\n" << endl;

	sw.push();
	wcout << determine_win_posibility(1.0/(2*3*3)/* gaunt */, (double)(2*2*5)/(3*3*6)/* marine */, 30, 10, 7) << endl;
	wcout << determine_win_posibility((double)(2*2*5)/(3*3*6)/* marine */, 1.0/(2*3*3)/* gaunt */, 10, 30, 7) << endl;
	t = sw.pop();
	std::cout << "Elapsed time: " << t << sw.getUnit() << "\n" << endl;

	system("PAUSE");
	return 0;
} // end function main


inline double worker(unsigned n1, unsigned n2, const BernsteinTriangle &bernsteinValues1, const BernsteinTriangle &bernsteinValues2, const vector<vector<double>> &baseCases, const vector<vector<double>> &oldTurn)
{
	double sum = 0;
	// possibility to win = possibility to win later...

	// i: units of type 2 that will die
	// j: units of type 1 that will die
	for(auto i = 0u ; i<n2 && i<=n1 ; i++)	// at least 1 unit of type 2 survives and no more than all attacks succeed
	    for(auto j = 0u ; j<n1 && j<=n2 ; j++)	// at least 1 unit of type 1 survives and no more than all attacks succeed
			sum += bernsteinValues1(n1,i)	// possibility exactly i unit 1 attacks succeed...
				* bernsteinValues2(n2,j)	// ...and exactly j unit 2 attacks succeed...
				* oldTurn[n1-j][n2-i];	// ...and unit 1 defeats unit 2 later but not before game ends.

	// ... + possibility to win now
	sum += baseCases[n1][n2];

	return sum;
} // end function worker


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

double determine_win_posibility(double p1, double p2, unsigned nModels1, unsigned nModels2, unsigned turns)
{
	if(nModels1 == 0) return 0.0;	// if there are no attackers, attackers can't win.
	if(nModels2 == 0) return 1.0;	// if there are no defenders, attackers can't loose.

	BernsteinTriangle bernsteinValues1(p1,nModels1);
	BernsteinTriangle bernsteinValues2(p2,nModels2);

	vector<vector<double>> baseCases;	// will hold the possibility to win now for each (n1,n2) pair.
		
	// precompute base case determine_win_posibility_worker(n1,n2,1) for each (n1,n2) pair.
	baseCases.resize(nModels1+1);
	for(auto n1 = 1u ; n1 < baseCases.size() ; n1++)	// we will never try to access baseCases[0][x] nor baseCases[x][0]
	{													// should probably save the memory too..
		baseCases[n1].resize(nModels2+1);				// should also verify the proof...
		for(auto n2 = 1u ; n2 < baseCases[n1].size() ; n2++)
		{
			// calculate possibility to win in current turn
			double sum = 0;
			for(auto i = n2 ; i<=n1 ; i++)	// at least enough attacks to kill all enemy models need to succeed.
				for(auto j = 0u; j<n1 && j<=n2 ; j++)	// a least 1 model of 1st unit must survive.
	    			sum += bernsteinValues1(n1,i)	// possibility exactly i unit 1 attacks succeed...
						* bernsteinValues2(n2,j);	// ...and exactly j unit 2 attacks succeed.
			baseCases[n1][n2] = sum;
		} // end for
	} // end for

	// allocate space for one turn
	vector<vector<double>> turnA;
	turnA.resize(baseCases.size());
	for(auto n1 = 1u ; n1 < turnA.size() ; n1++)
		turnA[n1].resize(baseCases[n1].size());
	// allocate space for another turn
	vector<vector<double>> turnB;
	turnB.resize(baseCases.size());
	for(auto n1 = 1u ; n1 < turnB.size() ; n1++)
		turnB[n1].resize(baseCases[n1].size());

	vector<vector<double>> *oldTurn = &turnA;	// will be used to swap old and new vectors
	vector<vector<double>> *newTurn = &turnB;

	if(turns > 2)
	{
		for(auto n1 = 1u ; n1 < baseCases.size() ; n1++)
			for(auto n2 = 1u ; n2 < baseCases[n1].size() ; n2++)
				(*newTurn)[n1][n2] = worker(n1,n2,bernsteinValues1,bernsteinValues2,baseCases,baseCases/**/);
	} // end if
	for(auto t = 3u ; t < turns ; t++)
	{
		swap(oldTurn,newTurn);
		for(auto n1 = 1u ; n1 < baseCases.size() ; n1++)
			for(auto n2 = 1u ; n2 < baseCases[n1].size() ; n2++)
				(*newTurn)[n1][n2] = worker(n1,n2,bernsteinValues1,bernsteinValues2,baseCases,*oldTurn);
	} // end for

	return worker(nModels1,nModels2,bernsteinValues1,bernsteinValues2,baseCases,*newTurn);
} // end function determine_win_posibility
