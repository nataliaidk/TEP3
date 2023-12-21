
#include "ExpressionTree.h"
#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
const std::string COMMAND_ENTER = "enter";
const std::string COMMAND_VARS = "vars";
const std::string COMMAND_PRINT = "print";
const std::string COMMAND_COMP = "comp";
const std::string COMMAND_JOIN = "join";
const std::string COMMAND_STOP = "stop";
template<typename T>
class ExpressionInterface {
public:
	void start() {
		std::cout << "Choose 1 - int 2 - double 3 - string" << std::endl;
		int choice;
		std::cin >> choice;

		switch (choice) {
		case 1: {
			ExpressionInterface<int> interfaceInt;
			interfaceInt.run();
			
		}

		case 2: {
			ExpressionInterface<double> interfaceDouble;
			interfaceDouble.run();
			
		}

		case 3: {
			ExpressionInterface<std::string> interfaceString;
			interfaceString.run();
			
		}


		}
	}
	void run() {

		existingTree = new ExpressionTree<T>();
		while (stop) {

			std::cout << "Choose function: enter, vars, print, comp, join, stop " << std::endl;
			std::string choice;
			std::cin >> choice;

			if (choice == COMMAND_ENTER) {
				std::cout << "Enter expression: ";
				std::cin.ignore();
				std::getline(std::cin, exp);
				existingTree = new ExpressionTree<T>(exp);
				std::cout << existingTree->printErrors();

			}
				else if (choice == COMMAND_VARS) {

				existingTree->printVariables();
			}
			else if (choice == COMMAND_PRINT) {

				std::cout << "Tree: " << existingTree->toString() << std::endl;

			}
			else if (choice == COMMAND_COMP) {
				ExpressionTree<T>* clonedTree = new ExpressionTree<T>(*existingTree);
				std::unordered_set<std::string> variableSet;
				int variablesSize = existingTree->countVariables(variableSet);



				if (variablesSize > 0) {
					std::cout << "Enter variable values: ( " << variablesSize << " )" << std::endl;
					std::cin.ignore();
					std::getline(std::cin, exp);

					if (variablesSize == existingTree->removeSpacesAndSymbols(exp).size()) {
						std::istringstream iss(exp);
						existingTree->assignVariableValues(iss);
						std::cout << "Result of the expression:  " << existingTree->comp() << std::endl;
						std::cout << existingTree->printErrors();
						std::cout << existingTree->printErrors();
						delete existingTree;  // Usuń oryginalne drzewo
						existingTree = clonedTree;  // Przywróć klonowane drzewo
					}
					else {
						std::cout << "Incorrect number of variables ";
					}
				}
				else {
					std::cout << "Result of the expression:  " << existingTree->comp() << std::endl;
					std::cout << existingTree->printErrors();
					delete existingTree;  // Usuń oryginalne drzewo

					existingTree = clonedTree;  // Przywróć klonowane drzewo

				}
			}
			else if (choice == COMMAND_JOIN) {
				std::cout << "Enter expression: " << std::endl;
				std::cin.ignore();
				std::getline(std::cin, exp);
				ExpressionTree<T>* joinedTree = new ExpressionTree<T>(exp);
				ExpressionTree<T>* resultTree;

				resultTree = *existingTree + *joinedTree;

				existingTree = resultTree;
				std::cout << joinedTree->printErrors();
			}
			else if (choice == COMMAND_STOP) {
				stop = false;
				delete existingTree;
			}
			else { std::cout << "Invalid command"; }
		}
	}
	


private:
	bool stop = true;
	ExpressionTree<T>* existingTree;
	std::string exp;
};
