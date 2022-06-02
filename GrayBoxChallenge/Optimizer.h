#pragma once

#include "Problem.h"

#include <random>
#include <vector>
#include <vector>
#include <string>

const int DEFAULT_POPULATION_SIZE = 100;
const double DEFAULT_CROSSOVER_THRESHOLD = 0.4;
const double DEFAULT_MUTATION_THRESHOLD = 0.3;

using namespace std;

class  CIndividual;

class COptimizer {

public:
	COptimizer(CProblem* pcProblem);
	~COptimizer();
	void vInitialize();
	void vRunIteration();
	double dRandomPropability();
	int iRandomPosInt();
	CIndividual* cChooseParent();
	void vFindBestResult();
	double dGetBestResult();
	int iGetCurrentSatClauses();
	CIndividual* pcGetBestFound();
	
private:
	std::vector<CIndividual*> vSolutionsPopulation;
	CProblem* pcProblemInstance;
	CIndividual* pcCurrentBest;
	double dCurrentBestFitness;
	int iPopulationSize;
	double dCrossoverThreshold;
	double dMutationThreshold;
	mt19937 c_rand_engine;
};//class COptimizer


class  CIndividual{
public:
	CIndividual(int iGenLen, CProblem* pcProblem);
	CIndividual(CIndividual& const cOther);
	~CIndividual();
	static std::vector<CIndividual*> vCrossover(CIndividual& cParent1, CIndividual& cParent2);
	void vMutation(double dMutThreshold);
	double dFitness();
	double dRandomPropability();
	int iRandomPosInt();
	bool* pbGetGenotype();
	void vRandomizeGenotype();
	int* piGetResultAsTable();

private:
	bool* pbGenotype;
	int iGenotypeLength;
	CProblem* pcProblemInstance;
	mt19937 c_rand_engine;
};//class  CIndividual