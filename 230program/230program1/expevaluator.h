#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include "lexanalyzer.h"

using namespace std;


typedef std::map<std::string, std::string> symbolTableType;


class expEvaluator
{
private:

	symbolTableType symbolTable;

public:
	int in2post(vector<pair<std::string, categoryType>>&);

	int evalpost(const vector<pair<std::string, categoryType>>&);

	void setSymbolTable(const vector<pair<std::string, categoryType>>&);
	void setSymbolTable(const vector<pair<std::string, categoryType>>&, int);
	void clear();
	void display();

	int getTableValue(std::string);
	int getPrec(const pair<std::string, categoryType>&);

};

#endif
