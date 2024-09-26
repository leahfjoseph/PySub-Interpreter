#include <iostream>
#include <stack>
#include <string>
#include "expevaluator.h"

using namespace std;

int expEvaluator::in2post(vector<pair<std::string, categoryType>>& infixVect)
{

	vector<pair<string, categoryType>> stackVect;
	vector<pair<string, categoryType>> postfixVect;
	int send2eval;

	for (int i = 0; i < infixVect.size(); i++)
	{
		if (infixVect[i].second == categoryType::NUMERIC_LITERAL) //if number put in vector
		{
			//pop off element from infixVect and push it into the postfixVect
			postfixVect.push_back(infixVect[i]);
		}
		if (infixVect[i].second == categoryType::LEFT_PAREN) //if ( push into stack
		{
			stackVect.push_back(infixVect[i]);
		}
		if (infixVect[i].second == categoryType::RIGHT_PAREN) //if ) take out everything from stack until (
		{
			while (!stackVect.empty() && stackVect.back().first != "(")
			{
				postfixVect.push_back(stackVect.back());
				stackVect.pop_back();
			}
			if (stackVect.empty())
			{
				cout << "Not a valid expression!" << endl;
				return -1;
			}
			stackVect.pop_back();
		}
		if (infixVect[i].second == categoryType::ARITH_OP || infixVect[i].second == categoryType::LOGICAL_OP || infixVect[i].second == categoryType::RELATIONAL_OP) //if operators
		{
			while (!stackVect.empty() && stackVect.back().first != "(" && getPrec(stackVect.back()) >= getPrec(infixVect[i])) //three conditional statements loop
			{
				postfixVect.push_back(stackVect.back());
				stackVect.pop_back();
				
			}
			stackVect.push_back(infixVect[i]);
		}
	}
	while (!stackVect.empty()) //at end loop until stack is empty
	{
		postfixVect.push_back(stackVect.back());
		stackVect.pop_back();
	}
	send2eval = evalpost(postfixVect); //setting variable to the evaluated postfi exp 
	return send2eval; //return variable
}

int expEvaluator::evalpost(const vector<pair<std::string, categoryType>>& postfixVect)
{
	vector<int> stackVect;
	int operand2;
	int operand1;
	int result = 0;
	int push2stack = 0;
	for (int i = 0; i < postfixVect.size(); i++) //stepping through postfix vector
	{
		if (postfixVect[i].second == categoryType::NUMERIC_LITERAL) //if number convert then push into stack
		{
			
			
			result = stoi(postfixVect[i].first);
			stackVect.push_back(result);
		}
		else if (postfixVect[i].first == "not") //if its an unary op
		{
			if (stackVect.empty())
			{
				cout << "Error! Cannot do if stack is empty!" << endl;
				return -1;
			}

			operand1 = stackVect.back();
			stackVect.pop_back();

			result = !operand1;
			stackVect.push_back(result);
		}
		else //if anything else, which is assuming a binary op
		{
			if (stackVect.size() < 2) //checking to make sure that there are at least 2 operands in the stack to eval
			{
				cout << "Error! There is not another operand in the stack to evaluate!" << endl;
				return -1;
			}
			else
			{
				//taking them out of stack and eval
				operand2 = stackVect.back();
				stackVect.pop_back();

				operand1 = stackVect.back();
				stackVect.pop_back();

				if (postfixVect[i].second == categoryType::ARITH_OP)
				{
					if (postfixVect[i].first == "*")
						result = operand1 * operand2;
					if (postfixVect[i].first == "+")
						result = operand1 + operand2;
					if (postfixVect[i].first == "-")
						result = operand1 - operand2;
					if (postfixVect[i].first == "/")
						result = operand1 / operand2;
					if (postfixVect[i].first == "%")
						result = operand1 % operand2;
				}

				if (postfixVect[i].second == categoryType::LOGICAL_OP)
				{
					if (postfixVect[i].first == "and")
						result = operand1 && operand2;
					if (postfixVect[i].first == "or")
						result = operand1 || operand2;
				}

				if (postfixVect[i].second == categoryType::RELATIONAL_OP)
				{
					if (postfixVect[i].first == ">")
						result = operand1 > operand2;
					if (postfixVect[i].first == "<")
						result = operand1 < operand2;
					if (postfixVect[i].first == "==")
						result = operand1 == operand2;
					if (postfixVect[i].first == "<=")
						result = operand1 <= operand2;
					if (postfixVect[i].first == ">=")
						result = operand1 >= operand2;
					if (postfixVect[i].first == "!=")
						result = operand1 != operand2;
				}

				stackVect.push_back(result); //putting back eval result in
			}
		}
		
	}
	return result;
}

void expEvaluator::setSymbolTable(const vector<pair<std::string, categoryType>>& lineTokens, int indent) //indented
{
	int userinput;
	//loop through tokens in line
	for (int i = indent; i < lineTokens.size(); i++)
	{
		if (lineTokens[i].second == categoryType::IDENTIFIER) //if token identifier, check for assignment op
		{
			if (i < lineTokens.size() - 1 && lineTokens[i + 1].second == categoryType::ASSIGNMENT_OP)
			{
				if (lineTokens[i+2].first == "int")
				{
					if (lineTokens[i + 4].first == "input")
					{
						string noQuotes = lineTokens[i+6].first;

						for (int j = 1; j < noQuotes.size() - 1; j++)
						{
							cout << noQuotes[j];
						}
						cout << " ";
						cin >> userinput;
						cin.ignore();

						symbolTable[lineTokens[i].first] = to_string(userinput);
						i += 7;
					}
				}
				else
				{
					vector<pair<std::string, categoryType>> expression;
					//if assign op, get rest of line and eval
					for (int j = i + 2; j < lineTokens.size(); j++)
					{
						expression.push_back(lineTokens[j]);
					}
					
					int result;

					result = in2post(expression);
					//add to map
					symbolTable[lineTokens[i].first] = to_string(result);
					return;
				}
			}
		}

	}
	

	

	
}

void expEvaluator::setSymbolTable(const vector<pair<std::string, categoryType>>& lineTokens)
{
	int userinput;
	//loop through tokens in line
	for (int i = 0; i < lineTokens.size(); i++)
	{
		if (lineTokens[i].second == categoryType::IDENTIFIER) //if token identifier, check for assignment op
		{
			if (i < lineTokens.size() - 1 && lineTokens[i + 1].second == categoryType::ASSIGNMENT_OP)
			{
				if (lineTokens[i + 2].first == "int")
				{
					if (lineTokens[i + 4].first == "input")
					{
						string noQuotes = lineTokens[i + 6].first;

						for (int j = 1; j < noQuotes.size() - 1; j++)
						{
							cout << noQuotes[j];
						}
						cout << " ";
						cin >> userinput;
						cin.ignore();

						symbolTable[lineTokens[i].first] = to_string(userinput);
						i += 7;
					}
				}
				else
				{
					vector<pair<std::string, categoryType>> expression;
					//if assign op, get rest of line and eval
					for (int j = i + 2; j < lineTokens.size(); j++)
					{
						expression.push_back(lineTokens[j]);
					}

					int result;

					result = in2post(expression);
					//add to map
					symbolTable[lineTokens[i].first] = to_string(result);
					return;
				}
			}
		}

	}





}

void expEvaluator::clear()
{
	symbolTable.clear();
}

int expEvaluator::getTableValue(std::string var)
{
	string assign;
	
	assign = symbolTable[var];

	return stoi(assign);

}

void expEvaluator::display()
{
	cout << "***** Show Table Variables and Values *****" << endl;
	for (auto i = symbolTable.begin(); i != symbolTable.end(); i++)
	{
		cout << i->first << " = " << i->second << endl;
	}
	cout << endl;
}

int expEvaluator::getPrec(const pair<std::string, categoryType>& op)
{
	if (op.second == categoryType::ARITH_OP)
	{
		if (op.first == "*" || op.first == "/" || op.first == "%")
			return 5;
		if (op.first == "+" || op.first == "-")
			return 4;
	}
	else if (op.second == categoryType::RELATIONAL_OP)
		return 3;
	else if (op.second == categoryType::LOGICAL_OP)
	{
		if (op.first == "not")
			return 2;
		if (op.first == "and")
			return 1;
		if (op.first == "or")
			return 0;
	}
	else
	{
		cout << "Error!" << endl;
		return -1;
	}
}