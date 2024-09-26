#include<iostream>
#include "interpreter.h"
using namespace std;


int checkCondition = -1;
bool loopSkip = 0;


bool Interpreter::interpretLine(const vector<vector<pair<string, categoryType>>> allLines, expEvaluator& expeval, int position)
{
	
	auto lineTokens = allLines[position];
	int indent = 0;
	if (lineTokens.empty())
		return 0;

	while (lineTokens[indent].second == categoryType::INDENT) //checking indents
	{
		indent++;
	}
	if (lineTokens[indent].second == categoryType::COMMENT || lineTokens.empty()) //if its a comment skip
	{
		return 0;
	}
	else if (lineTokens[indent].first == "if" || lineTokens[0].first == "elif" || lineTokens[0].first == "else") //if/else/elif
	{
		vector<pair<string, categoryType>> statement;
		for (int i = 1; i < lineTokens.size()-1; i++) //pushing linetokens into statement
		{
			statement.push_back(lineTokens[i]);
		}
		if (lineTokens[0].first == "if")
		{
			checkCondition = evalcondition(statement, expeval); //making check condition equal to the eval to mark it went through
		}
		if (lineTokens[0].first == "elif")
		{
			if (checkCondition == 0)
				checkCondition = evalcondition(statement, expeval); //if checkcondition is 0, then we set it to the evaluation of the elif
		}
		if (lineTokens[0].first == "else")
		{
			if (checkCondition == 0) //if check condition is 0 we set it to 1 so that we make sure it runs if nothing else does
				checkCondition = 1;
		}
	}


	if (!checkCondition) //if check condition is 0 we end function to move to next line
	{
		return 0;
	}

	if (indent == 0)
	{
		checkCondition = -1;
	}

	if ((lineTokens[0].first == "else" || lineTokens[0].first == "elif") && checkCondition > 0) // if we're working with else/elif AND we have already done if, we reset and end function
	{
		checkCondition = 0;
		return 0;
	}
	else if (lineTokens[indent].first == "print")
	{
		vector<pair<string, categoryType>> currentClassifier;
		vector<vector<pair<string, categoryType>>> classifiers;

		int i = indent+2;

		while (i < lineTokens.size() - 1)
		{
			if (lineTokens[i].second == categoryType::COMMA || lineTokens[i+1].second == categoryType::RIGHT_PAREN)
			{
				if (lineTokens[i + 1].second == categoryType::RIGHT_PAREN)
				{
					if (lineTokens[i].second == categoryType::IDENTIFIER)
					{
						int varval = expeval.getTableValue(lineTokens[i].first);

						currentClassifier.push_back(make_pair(to_string(varval), categoryType::NUMERIC_LITERAL));
					}
					else
					{
						currentClassifier.push_back(lineTokens[i]);
					}
				}

				classifiers.push_back(currentClassifier);
				currentClassifier.clear();
			}
			else
			{
				if (lineTokens[i].second == categoryType::IDENTIFIER)
				{
					int varval = expeval.getTableValue(lineTokens[i].first);

					currentClassifier.push_back(make_pair(to_string(varval), categoryType::NUMERIC_LITERAL));
				}
				else
				{
					currentClassifier.push_back(lineTokens[i]);
				}
			}
			i++;
		}

		for (int i = 0; i < classifiers.size(); i++)
		{
			if (classifiers[i].empty())
			{
				cout << "error!" << endl;
				return 1;
			}
			else if (classifiers[i].size() == 1)
			{
				if (classifiers[i][0].second == categoryType::STRING_LITERAL)
				{
					string noQuotes = classifiers[i][0].first;

					for (int j = 1; j < noQuotes.size()-1; j++)
					{
						cout << noQuotes[j];
					}
					cout << " ";
				}
				if (classifiers[i][0].second == categoryType::NUMERIC_LITERAL)
				{
					cout << classifiers[i][0].first;
				}
			}
			else
			{
				int result = expeval.in2post(classifiers[i]);
				cout << result;
			}
			
		}
		cout << endl;
		return 0;
	}
	else if (lineTokens[indent].first == "while")
	{
		vector<pair<string, categoryType>> whileConditional;

		startWhileIndex = position;

		int j = startWhileIndex + 1;
		while (j < allLines.size() && allLines[j][0].second == categoryType::INDENT)
		{
			j++;
		}

		if (j < allLines.size() && allLines[j][0].second == categoryType::INDENT)
		{
			j++;
		}
		
		endWhileIndex = j;

		for (int i = 1; i < lineTokens.size() - 1; i++)
		{
			whileConditional.push_back(lineTokens[i]);
		}

		if (evalcondition(whileConditional, expeval))
		{
			return 0;
		}
		else
		{
			loopSkip = 1;
			return 0;
		}
	}
	else if (lineTokens[indent].second == categoryType:: IDENTIFIER)
	{
		expeval.setSymbolTable(lineTokens, indent);
		return 0;
	}
	else
	{
		cout << "Error!" << endl;
		return 1;
	}
}

int Interpreter::evalcondition(const vector<pair<string, categoryType>> lineTokens, expEvaluator& expeval)
{
	vector<pair<string, categoryType>> expression;


	for (int i = 0; i < lineTokens.size(); i++)
	{
		if (lineTokens[i].second == categoryType::RIGHT_PAREN || lineTokens[i].second == categoryType::LEFT_PAREN || lineTokens[i].second == categoryType::COLON)
		{
			continue; //continue and skip over parens or commas
		}
		if (lineTokens[i].second == categoryType::IDENTIFIER)
		{
			int val = expeval.getTableValue(lineTokens[i].first); //grabbing values

			expression.push_back(make_pair(to_string(val), categoryType::NUMERIC_LITERAL));
		}
		else
		{
			expression.push_back(lineTokens[i]);
		}
	}
	int result = expeval.in2post(expression);
	return result;
}
