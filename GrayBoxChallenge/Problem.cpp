#include "Problem.h"


#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <cstring>

const int iVariableSignIndex = 1;
const int iVariableInClauseArrLength = 2;


CProblem::CProblem() {
    iClauseLength = DEFAULT_CLAUSE_LENGTH;
    vClausesByVars = nullptr;
}

CProblem::~CProblem() {
    for (int i = 0; i < vClauses.size(); i++) {
        for (int j = 0; j < iClauseLength; j++) {
            delete[] vClauses[i][j];
        }
        delete[] vClauses[i];
    }
    delete[] vClausesByVars;
}

bool bIsInteger(std::string str) {
    int offset = 0;
    if (str[0] == '-')
        offset = 1;
    for (int i = offset; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool CProblem::bLoad(std::string sFileName) {


    std::ifstream fileStream;
    std::string sLine;
    int iNumberBuffer = 0;
    fileStream.open(sFileName);

    if (fileStream.is_open()) {
        while (std::getline(fileStream, sLine)) {
            int** nextClause = new int* [iClauseLength];
            for (int i = 0; i < iClauseLength; i++) {
                nextClause[i] = new int[iVariableInClauseArrLength];
            }
            std::stringstream ss(sLine);
            std::string word;
            int i = 0;
            while (ss >> word) {
                if (bIsInteger(word)) {
                    int iCurrentVar = abs(stoi(word));
                    nextClause[i][0] = iCurrentVar;

                    if (word[0] == '-')
                        nextClause[i][iVariableSignIndex] = -1;
                    else {
                        nextClause[i][iVariableSignIndex] = 0;
                    }

                    if (sVariables.find(iCurrentVar) == sVariables.end())
                        sVariables.insert(iCurrentVar);
                    i++;
                }
            }
            vClauses.push_back(nextClause);
        }
        fileStream.close();
        //vPrintClauses();       
        vSegregateClausesToVariables();
        //vPrintSegregatedClauses();
        return true;
    }
    else {
        //std::cout << "false";
        return false;
    }

}

void CProblem::vSegregateClausesToVariables() {
    vClausesByVars = new std::vector<int>[sVariables.size()];

    for (int i = 0; i < vClauses.size(); i++) {
        for (int j = 0; j < iClauseLength; j++) {
            int iVar = vClauses[i][j][0];
            int iVarIndx = std::distance(sVariables.begin(), sVariables.find(iVar));
            vClausesByVars[iVarIndx].push_back(i);
        }
    }
}

int  CProblem::iCompute(bool* const& pbSolution) {
    int iSatCount = 0;
    for (int i = 0; i < vClauses.size(); i++) {
        bool bIsSatisfied = false;
        for (int j = 0; j < iClauseLength && !bIsSatisfied; j++) {
            int iVar = vClauses[i][j][0];
            int iVarIndx = std::distance(sVariables.begin(), sVariables.find(iVar));

            if (vClauses[i][j][iVariableSignIndex] == -1) {
                if (!pbSolution[iVarIndx]) {
                    iSatCount++;
                    bIsSatisfied = true;
                }
            }
            else {
                if (pbSolution[iVarIndx]) {
                    iSatCount++;
                    bIsSatisfied = true;
                }
            }
        }
    }
    return iSatCount;
}

int CProblem::iComputeClausesContainingVariable(bool* const& pbSolution, int iVariablePosition) {
    int iSatCount = 0;
    for (int i = 0; i < vClausesByVars[iVariablePosition].size(); i++) {
        bool bIsSatisfied = false;
        int** piCurrClause = vClauses[vClausesByVars[iVariablePosition][i]];

        for (int j = 0; j < iClauseLength && !bIsSatisfied; j++) {
            int iVar = piCurrClause[j][0];
            int iVarIndx = std::distance(sVariables.begin(), sVariables.find(iVar));

            if (piCurrClause[j][iVariableSignIndex] == -1) {
                if (!pbSolution[iVarIndx]) {
                    iSatCount++;
                    bIsSatisfied = true;
                }
            }
            else {
                if (pbSolution[iVarIndx]) {
                    iSatCount++;
                    bIsSatisfied = true;
                }
            }
        }
    }
    return iSatCount;
}

int CProblem::iGetVarsNumber() {
    return sVariables.size();
}

int  CProblem::iGetClausesNumber() {
    return vClauses.size();
}

void CProblem::vPrintClauses() {
    for (int i = 0; i < vClauses.size(); i++) {
        std::cout << "[ ";
        for (int j = 0; j < iClauseLength; j++) {
            if (vClauses[i][j][iVariableSignIndex] == -1)
                std::cout << "-";
            std::cout << vClauses[i][j][0] << " ";
        }
        std::cout << "]\n";
    }
}

void CProblem::vPrintSegregatedClauses() {
    for (int i = 0; i < sVariables.size(); i++) {
        for (int j = 0; j < vClausesByVars[i].size(); j++) {
            std::cout << vClausesByVars[i][j] << ", ";
        }
        std::cout << "\n";
    }
}

void CProblem::vPrintVariables() {
    std::set<int>::iterator it = sVariables.begin();
    while (it != sVariables.end()) {
        std::cout << *it << ", ";
        it++;
    }
}


