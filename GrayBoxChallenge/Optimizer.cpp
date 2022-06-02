#include "Optimizer.h"

#include <cfloat>
#include <iostream>
#include <windows.h>

using namespace std;

const int I_MODULO_BOOL = 2;

COptimizer::COptimizer(CProblem* pcProblem) {
	iPopulationSize = DEFAULT_POPULATION_SIZE;
	dCrossoverThreshold = DEFAULT_CROSSOVER_THRESHOLD;
	dMutationThreshold = DEFAULT_MUTATION_THRESHOLD;
	pcProblemInstance = pcProblem;
	pcCurrentBest = nullptr;
	dCurrentBestFitness = 0.0;
	c_rand_engine.seed(random_device()());
}

COptimizer::~COptimizer() {
	for (int i = 0; i < vSolutionsPopulation.size(); i++) {
		delete vSolutionsPopulation[i];
	}
	if (pcCurrentBest != nullptr)
		delete pcCurrentBest;
}

void COptimizer::vInitialize() {
	for (int i = 0; i < iPopulationSize; i++) {
		CIndividual* cIndiv = new CIndividual(pcProblemInstance->iGetVarsNumber(), pcProblemInstance);
		cIndiv->vRandomizeGenotype();
		vSolutionsPopulation.push_back(cIndiv);
	}
}

void COptimizer::vRunIteration() {
	std::vector<CIndividual*> vNewSolPopulation;

	while (vNewSolPopulation.size() < vSolutionsPopulation.size()) {
		CIndividual* cParent1 = cChooseParent();
		CIndividual* cParent2 = cChooseParent();
		CIndividual* cChild1;
		CIndividual* cChild2;
		if (dRandomPropability() <= dCrossoverThreshold) {
			std::vector<CIndividual*> vChildren = CIndividual::vCrossover(*cParent1, *cParent2);
			cChild1 = vChildren[0];
			cChild2 = vChildren[1];
		}
		else {
			cChild1 = new CIndividual(*cParent1);
			cChild2 = new CIndividual(*cParent2);
		}
		cChild1->vMutation(dMutationThreshold);
		cChild2->vMutation(dMutationThreshold);

		vNewSolPopulation.push_back(cChild1);
		vNewSolPopulation.push_back(cChild2);
	}

	for (int i = 0; i < vSolutionsPopulation.size(); i++) {
		delete vSolutionsPopulation[i];
	}

	vSolutionsPopulation = vNewSolPopulation;
	vFindBestResult();
}

double COptimizer::dRandomPropability() {
	std::uniform_real_distribution<> dis(0.0, 1.0);
	return dis(c_rand_engine);
}

int COptimizer::iRandomPosInt() {
	int result = c_rand_engine();
	if (result > 0) {
		return result;
	}
	else {
		return -result;
	}
}

CIndividual* COptimizer::cChooseParent() {
	int iP1Index = iRandomPosInt() % iPopulationSize;
	int iP2Index = iRandomPosInt() % iPopulationSize;

	if (vSolutionsPopulation[iP1Index]->dFitness() < vSolutionsPopulation[iP2Index]->dFitness())
		return vSolutionsPopulation[iP2Index];
	else
		return vSolutionsPopulation[iP1Index];
}

void COptimizer::vFindBestResult() {

	for (int i = 0; i < vSolutionsPopulation.size(); i++) {
		if (vSolutionsPopulation[i]->dFitness() > dCurrentBestFitness) {
			if (pcCurrentBest != nullptr)
				delete pcCurrentBest;
			pcCurrentBest = new CIndividual(*vSolutionsPopulation[i]);
			dCurrentBestFitness = vSolutionsPopulation[i]->dFitness();
		}
	}
}

double COptimizer::dGetBestResult() {
	return dCurrentBestFitness;
}

int COptimizer::iGetCurrentSatClauses() {
	return pcProblemInstance->iCompute(pcCurrentBest->pbGetGenotype());
}


CIndividual* COptimizer::pcGetBestFound() {
	return pcCurrentBest;
}

CIndividual::CIndividual(int iGenLen, CProblem* pcProblem) {
	iGenotypeLength = iGenLen;
	pbGenotype = new bool[iGenLen];
	pcProblemInstance = pcProblem;
	c_rand_engine.seed(random_device()());
}

CIndividual::CIndividual(CIndividual& const cOther) {
	iGenotypeLength = cOther.iGenotypeLength;
	pbGenotype = new bool[cOther.iGenotypeLength];
	pcProblemInstance = cOther.pcProblemInstance;
	for (int i = 0; i < cOther.iGenotypeLength; i++) {
		pbGenotype[i] = cOther.pbGenotype[i];
	}
	c_rand_engine.seed(random_device()());
}

CIndividual::~CIndividual() {
	if (pbGenotype != nullptr)
		delete[] pbGenotype;
}

std::vector<CIndividual*> CIndividual::vCrossover(CIndividual& cParent1, CIndividual& cParent2) {
	CIndividual* cChild1 = new CIndividual(cParent1.iGenotypeLength, cParent1.pcProblemInstance);
	CIndividual* cChild2 = new CIndividual(cParent1.iGenotypeLength, cParent1.pcProblemInstance);
	mt19937 c_rand_engine;
	for (int i = 0; i < cParent1.iGenotypeLength; i++) {
		if (c_rand_engine() % I_MODULO_BOOL == 0) {
			cChild1->pbGetGenotype()[i] = cParent1.pbGetGenotype()[i];
			cChild2->pbGetGenotype()[i] = cParent2.pbGetGenotype()[i];
		}
		else {
			cChild1->pbGetGenotype()[i] = cParent2.pbGetGenotype()[i];
			cChild2->pbGetGenotype()[i] = cParent1.pbGetGenotype()[i];
		}
	}
	std::vector<CIndividual*> vChildren;
	vChildren.push_back(cChild1);
	vChildren.push_back(cChild2);

	return vChildren;
}

void CIndividual::vMutation(double dMutThreshold) {
	for (int i = 0; i < iGenotypeLength; i++) {
		//std::cout << i << "\n";
		if (dRandomPropability() <= dMutThreshold) {
			int iSatisfied = pcProblemInstance->iComputeClausesContainingVariable(pbGenotype, i);
			pbGenotype[i] = !pbGenotype[i];

			if (iSatisfied > pcProblemInstance->iComputeClausesContainingVariable(pbGenotype, i))
				pbGenotype[i] = !pbGenotype[i];
		}
	}
}
double CIndividual::dRandomPropability() {
	std::uniform_real_distribution<> dis(0.0, 1.0);
	return dis(c_rand_engine);
}

int CIndividual::iRandomPosInt() {
	int result = c_rand_engine();
	if (result > 0) {
		return result;
	}
	else {
		return -result;
	}
}

double CIndividual::dFitness() {
	return (double)pcProblemInstance->iCompute(this->pbGenotype) / pcProblemInstance->iGetClausesNumber();
}

bool* CIndividual::pbGetGenotype() {
	return pbGenotype;
}

void CIndividual::vRandomizeGenotype() {
	for (int i = 0; i < iGenotypeLength; i++) {
		pbGenotype[i] = iRandomPosInt() % I_MODULO_BOOL;
	}
}

int* CIndividual::piGetResultAsTable() {
	int iSize = pcProblemInstance->iGetVarsNumber();
	int* piResult = new int[iSize];

	for (int i = 0; i < iSize; i++) {
		piResult[i] = pbGenotype[i];
	}

	return piResult;
}

