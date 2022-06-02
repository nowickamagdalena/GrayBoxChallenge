#pragma once

#include <string>
#include <vector>
#include <set>
const int DEFAULT_CLAUSE_LENGTH = 3;

using namespace std;



class CProblem
{
public:
	CProblem();
	virtual ~CProblem();
	bool bLoad(std::string sFileName);
	int iCompute(bool* const& pbSolution);
	void vSegregateClausesToVariables();
	int iComputeClausesContainingVariable(bool* const& pbSolution, int iVariable);
	int iGetVarsNumber();
	int iGetClausesNumber();
	void vPrintClauses();
	void vPrintSegregatedClauses();
	void vPrintVariables();

private:
	std::vector<int**> vClauses;
	std::set<int> sVariables;
	std::vector<int>* vClausesByVars;
	int iClauseLength;

};//class CProblem


