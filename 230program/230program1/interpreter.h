#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexanalyzer.h"
#include "expevaluator.h"


class Interpreter
{
public:
	bool interpretLine(const vector<vector<pair<string, categoryType>>>, expEvaluator&, int);
	int evalcondition(const vector<pair<string, categoryType>>, expEvaluator&);

	int getwhileIndex() { return startWhileIndex; }
	int getendwhileIndex() { return endWhileIndex; }
	int whileCondition() { return whileCond; }

private:
	int whileCond;
	int startWhileIndex;
	int endWhileIndex;
};
#endif

