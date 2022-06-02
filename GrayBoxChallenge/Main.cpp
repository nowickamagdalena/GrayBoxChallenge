#include "Problem.h"
#include "Optimizer.h"
#include "Timer.h"

#include <exception>
#include <iostream>

using namespace TimeCounters;

using namespace std;

#define dMAX_TIME 5


void vRunExperiment(CProblem *pcProblem)
{
	try
	{
		CTimeCounter c_time_counter;

		double d_time_passed;

		COptimizer c_optimizer(pcProblem);

		c_time_counter.vSetStartNow();


		c_optimizer.vInitialize();

		c_time_counter.bGetTimePassed(&d_time_passed);

		while (d_time_passed <= dMAX_TIME)
		{
			c_optimizer.vRunIteration();
			c_optimizer.pcGetBestFound();
			//std::cout << c_optimizer.dGetBestResult() << "\n";
			//std::cout << c_optimizer.iGetCurrentSatClauses() << "\n";

			c_time_counter.bGetTimePassed(&d_time_passed);
		}//while (d_time_passed <= MAX_TIME)
		std::cout << c_optimizer.dGetBestResult() << "\n";
		std::cout << c_optimizer.iGetCurrentSatClauses() << "\n";
	}//try
	catch (exception &c_exception)
	{
		cout << c_exception.what() << endl;
	}//catch (exception &c_exception)
}//void vRunExperiment(const CEvaluator &cConfiguredEvaluator)

double vIteration(CProblem* pcProblem) {

	COptimizer c_opt(pcProblem);

	c_opt.vInitialize();

	for (int ii = 0; ii < 30; ii++) {
		std::cout << ii << "\n";
		c_opt.vRunIteration();
		c_opt.pcGetBestFound();
		std::cout << c_opt.dGetBestResult() << "\n";
		std::cout << c_opt.iGetCurrentSatClauses() << "\n";
	}

	return c_opt.dGetBestResult();
}

void check() {
	CProblem  c_problem;

	if (c_problem.bLoad("m3s_350_41.txt") == true){
		std::cout << c_problem.iGetClausesNumber() << "\n";
		//std::cout << vIteration(&c_problem);
		vRunExperiment(&c_problem);
	}//if (c_problem.bLoad("m3s_100_2") == true)
}

void main(int iArgCount, char **ppcArgValues)
{
	
	check();

}//void main(int iArgCount, char **ppcArgValues)