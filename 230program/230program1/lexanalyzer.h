#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum class categoryType
{
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN 
};

typedef vector<vector<pair<string, categoryType>>> tokenType;
typedef vector<pair<string, categoryType>> tokenLineType;
typedef pair<string, categoryType> pairType;


class LexicalAnalyzer
{
private:
    tokenType tokenInfo;
public:
    tokenLineType gettingTokens(const string);
    void readTokens();
    void clear() { tokenInfo.clear(); }
};
#endif