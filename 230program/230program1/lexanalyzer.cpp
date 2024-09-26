#include <iostream>
#include <cctype>
#include "lexanalyzer.h"
#include "expevaluator.h"
using namespace std;


tokenLineType LexicalAnalyzer::gettingTokens(const string line)
{
	tokenLineType lineTokens;   // vector of token pairs
	string token;
	string yn;
	expEvaluator expeval;

	for (int i = 0; i < line.length(); i++)
	{
		token.clear();
		char c = line[i];
		token += c;

		if (isalpha(c))
		{
			int j = i + 1;
			while (isalpha(line[j]))
			{
				token += line[j];
				j++;
			}
			if (token == "and" || token == "or" || token == "not")
			{
				lineTokens.push_back(make_pair(token, categoryType::LOGICAL_OP));
			}
			else if (token == "if" || token == "elif" || token == "else" || token == "while" || token == "for" || token == "print" || token == "return")
			{
				lineTokens.push_back(make_pair(token, categoryType::KEYWORD));
			}
			else
			{
				lineTokens.push_back(make_pair(token, categoryType::IDENTIFIER));
			}
					
			i = j - 1; // goes to end of the token
		}
		else if (c == '#')
		{
			int j;
			for (j = i + 1; j < line.length(); j++)
			{
				token += line[j];
			}
			lineTokens.push_back(make_pair(token, categoryType::COMMENT));
			i = j;
		}
		else if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%')
		{
			lineTokens.push_back(make_pair(token, categoryType::ARITH_OP));
		}
		else if (c == '!' || c == '<' || c == '>')
		{
			if (line[i + 1] == '=')
			{
				token += line[i + 1];
				i++;
			}
			lineTokens.push_back(make_pair(token, categoryType::RELATIONAL_OP));
		}
		else if (c == '=')
		{
			if (line[i + 1] == '=')
			{
				token += line[i + 1];
				i++;
				lineTokens.push_back(make_pair(token, categoryType::RELATIONAL_OP));
			}
			else
			{
				lineTokens.push_back(make_pair(token, categoryType::ASSIGNMENT_OP));
			}
		}
		else if (c == '(')
		{
			lineTokens.push_back(make_pair(token, categoryType::LEFT_PAREN));
		}
		else if (c == ')')
		{
			lineTokens.push_back(make_pair(token, categoryType::RIGHT_PAREN));
		}
		else if (c == ':')
		{
			lineTokens.push_back(make_pair(token, categoryType::COLON));
		}
		else if (c == ',')
		{
			lineTokens.push_back(make_pair(token, categoryType::COMMA));
		}
		else if (c == '"' || c == '\'')
		{
			int j = i + 1;
			if (c == '"')
			{
				while ((j < line.length()) && line[j] != '"')
				{
					token += line[j];
					j++;
				}
			}
			if (c == '\'')
			{
				while ((j < line.length()) && line[j] != '\'')
				{
					token += line[j];
					j++;
				}
			}

			if (j != line.length())
			{
				token += line[j];
			}

			lineTokens.push_back(make_pair(token, categoryType::STRING_LITERAL));
			i = j;
		}

		else if (isdigit(c))
		{
			int j = i + 1;
			while (isdigit(line[j]))
			{
				token += line[j];
				j++;
			}
			lineTokens.push_back(make_pair(token, categoryType::NUMERIC_LITERAL));
			i = j - 1; // goes to end of the token
		}
		else if (c == ' ' || c == '\t')
		{
			if (i == 0)
			{
				lineTokens.push_back(make_pair(token, categoryType::INDENT));
			}
		}
		else
		{
			lineTokens.push_back(make_pair(token, categoryType::UNKNOWN));
		}
	}

	tokenInfo.push_back(lineTokens);

	return lineTokens;
}

void LexicalAnalyzer::readTokens()
{
	cout << "***** TOKEN INFORMATION *****" << endl;
	for (int i = 0; i < tokenInfo.size(); i++)
	{
		cout << "Line #" << i << ":" << endl;
		for (int j = 0; j < tokenInfo[i].size(); j++)
		{
			cout << "Token[" << j << "]: ";
			cout << tokenInfo[i][j].first << " - ";
			
			switch (tokenInfo[i][j].second)
			{
				case categoryType::KEYWORD:
					cout << "KEYWORD";
					break;
				case categoryType::IDENTIFIER:
					cout << "IDENTIFIER";
					break;
				case categoryType::STRING_LITERAL:
					cout << "STRING_LITERAL";
					break;
				case categoryType::NUMERIC_LITERAL:
					cout << "NUMERIC_LITERAL";
					break;
				case categoryType::ASSIGNMENT_OP:
					cout << "ASSIGNMENT_OP";
					break;
				case categoryType::ARITH_OP:
					cout << "ARITH_OP";
					break;
				case categoryType::LOGICAL_OP:
					cout << "LOGICAL_OP";
					break;
				case categoryType::RELATIONAL_OP:
					cout << "RELATIONAL_OP";
					break;
				case categoryType::LEFT_PAREN:
					cout << "LEFT_PAREN";
					break;
				case categoryType::RIGHT_PAREN:
					cout << "RIGHT_PAREN";
					break;
				case categoryType::COLON:
					cout << "COLON";
					break;
				case categoryType::COMMA:
					cout << "COMMA";
					break;
				case categoryType::COMMENT:
					cout << "COMMENT";
					break;
				case categoryType::INDENT:
					cout << "INDENT";
					break;
				case categoryType::UNKNOWN:
					cout << "UNKNOWN";
					break;

			}
			cout << endl;
		}
		cout << "-------------------------------------" << endl;
	}
}