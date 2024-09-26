#include <iostream>
#include <fstream>
#include "interface.h"
#include <string>
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"
using namespace std;

void Interface::startInterface()
{
	LexicalAnalyzer lexanalysis;
	expEvaluator expeval;
	Interpreter pySubI;
	//entering the interface
	cout << "Enter program lines or read (<filename>.py) at command line interface" << endl
		<< "Type 'help' for more information or 'quit' to exit" << endl;

	string cmd;
	string yn;
	bool isExp = true;
	cout << ">>>  ";
	getline(cin, cmd); //user input

	while (cmd != "quit" && cmd != "quit()") //while loop until the user chooses to quit
	{
		if (cmd == "help" || cmd == "help()") //help utility
		{
			cout << "Welcome to the Help Utility!" << endl
				<< "*To exit and return to the interpreter, type 'exit'" << endl
				<< "*To get a list of commands, type 'commands'" << endl
				<< "*To get a description of any command, just type the command at the help> prompt" << endl << endl;
			cout << "help>  ";
			getline(cin, cmd); //user input
			while (cmd != "exit" && cmd != "exit()") //while loop until user chooses to exit help utility
			{
				if (cmd == "quit") // **ENTER DEFINITIONS**
				{
					cout << "This is the quit command. It will quit the program when not in the help utility." << endl;
				}
				else if (cmd == "help")
				{
					cout << "This is the help command. It will send you to the help utility for more info on commands and definitons." << endl;
				}
				else if (cmd == "read")
				{
					cout << "This is the read command. It will read your .py files and store the contents in a vector" << endl;
				}
				else if (cmd == "show")
				{
					cout << "This is the show command. It will show whatever has been stored in the vector." << endl;
				}
				else if (cmd == "clear")
				{
					cout << "This is the clear command. It will clear whatever is inside of the vector." << endl;
				}
				else if (cmd == "commands")
				{
					cout << "Below is a list of commands. Enter any command at the prompt to get more help:"
						<< endl << endl
						<< "clear	help	quit	read	show" << endl << endl
						<< "NOTE: All commands can also be entered as functions: " << endl << endl
						<< "clear()		help()		quit()		read()		show()" << endl;
				}
				else
				{
					cout << "Error!" << endl
						<< "*To exit and return to the interpreter, type 'exit'" << endl
						<< "*To get a list of commands, type 'commands'" << endl
						<< "*To get a description of any command, just type the command at the help> prompt" << endl << endl;
				}
				cout << endl << "help>  ";
				getline(cin, cmd);
			}
		}
		else if (cmd == "show" || cmd == "show()") //if statement to show whats in the vector
		{
			for (int i = 0; i < programCode.size(); i++) //printing vector w/ lines, too
			{
				cout << "[" << i << "]  " << programCode[i] << endl;
			}
		}
		else if (cmd == "show(tokens)")
		{
			lexanalysis.readTokens();		
		}
		else if (cmd == "clear" || cmd == "clear()") //if statement to clear vector
		{
			programCode.clear();
			lexanalysis.clear();
			isExp = true;
		}
		else if (cmd == "run" || cmd == "run()")
		{
			vector<vector<pair<string, categoryType>>> allLines;
			for (int i = 0; i < programCode.size(); i++) //running through vector
			{
				vector<pair<string, categoryType>> lineTokens;
				lineTokens = lexanalysis.gettingTokens(programCode[i]); //grabbing tokens

				allLines.push_back(lineTokens); //putting each line into big vector
			}

			for (int i = 0; i < programCode.size(); i++) //printing vector w/ lines, too
			{
				if (pySubI.getwhileIndex() < i < pySubI.getendwhileIndex()) //while i is inbetween the start and end of the while loop
				{
					if (pySubI.whileCondition() > 0) //while the condition is going, interpret the line
					{

						pySubI.interpretLine(allLines, expeval, i);
					}
					else
						continue;
					if (i + 1 == pySubI.getendwhileIndex()) //if our i is right before the end of the block
					{
						i = pySubI.getwhileIndex() - 1; //push i back to before start
					}
				}
				else
				{
					pySubI.interpretLine(allLines, expeval, i); //all else interpret the line
				}
			}
		}
		else if (cmd == "show(variables)")
		{
			expeval.display();
		}
		else //this is for the ability for the user to input ex. help(show) and for the definition to come up. Also the read command for .py files
		{
			string begin;
			begin = cmd.substr(0, 5); //begin reads for either "read(" or "help("
			if (begin == "read(" || begin == "help(")
			{
				string checkclose;
				checkclose = cmd.substr(5, cmd.length()); //checkclose makes sure theres a closing parentheses
				if (checkclose == "" || checkclose[checkclose.length() - 1] != ')') //if no closing parentheses
				{
					cout << "Syntax Error!";
				}
				else //if closing parentheses
				{
					string op;
					op = checkclose.substr(0, checkclose.length() - 1); //op is whats inside the parentheses, ex help(clear)
					if (begin == "read(") //if its for reading a .py
					{
						programCode.clear();
						lexanalysis.clear();
						expeval.clear();
						ifstream fileinput;
						fileinput.open(op);
						if (!fileinput)
						{
							cout << "Error!" << endl; //if no file
						}
						else
						{
							string content;
							while (getline(fileinput, content))
							{
								programCode.push_back(content); //entering file's contents into the vector
								lexanalysis.gettingTokens(content);
							}
						}
					}
					if (begin == "help(") //if its asking for help
					{
						if (op == "quit") // **ENTER DEFINITIONS**
						{
							cout << "This is the quit command. It will quit the program when not in the help utility." << endl;
						}
						else if (op == "help")
						{
							cout << "This is the help command. It will send you to the help utility for more info on commands and definitons." << endl;
						}
						else if (op == "read")
						{
							cout << "This is the read command. It will read your .py files and store the contents in a vector" << endl;
						}
						else if (op == "show")
						{
							cout << "This is the show command. It will show whatever has been stored in the vector." << endl;
						}
						else if (op == "clear")
						{
							cout << "This is the clear command. It will clear whatever is inside of the vector." << endl;
						}
						if (op == "commands")
						{
							cout << "Below is a list of commands. Enter any command at the prompt to get more help:"
								<< endl << endl
								<< "clear	help	quit	read	show" << endl << endl
								<< "NOTE: All commands can also be entered as functions: " << endl << endl
								<< "clear()		help()		quit()		read()		show()" << endl;
						}
					}
				}
			}
			else
			{
				
				
				vector<pair<string, categoryType>> lineTokens;
				lineTokens = lexanalysis.gettingTokens(cmd);
				if (lineTokens.empty())
				{
					isExp = false;
				}
				
				for (auto token : lineTokens)
				{
					if (token.second == categoryType::IDENTIFIER || token.second == categoryType::COLON || token.second == categoryType::COMMA || token.second == categoryType::ASSIGNMENT_OP || token.second == categoryType::KEYWORD || token.second == categoryType::STRING_LITERAL)
					{
						isExp = false;
						
					}
					else
					{
						isExp = true;
					}
				}

				if (isExp)
				{
					cout << expeval.in2post(lineTokens);
					lexanalysis.clear();
				}
				else
				{
					programCode.push_back(cmd);
				}
					
			}
		}
		cout << endl << ">>>  ";
		getline(cin, cmd);
	}

}