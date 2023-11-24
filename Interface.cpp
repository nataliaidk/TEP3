#include "Interface.h"

#include <iostream>
#include <sstream>

void ExpressionInterface::run() {

	existingTree = new ExpressionTree();
	while (stop) {

		std::cout << "Choose function: enter, vars, print, comp, join, stop " << std::endl;
		std::string choice;
		std::cin >> choice;

		if (choice == COMMAND_ENTER) {
			std::cout << "Enter expression: ";
			std::cin.ignore();  // Ignore the newline character left in the input stream
			std::getline(std::cin, exp);
			existingTree = new ExpressionTree(exp);
			std::cout << existingTree->printErrors();

		}
		else if (choice == COMMAND_VARS) {

			existingTree->printVariables();
		}
		else if (choice == COMMAND_PRINT) {

			std::cout << "Tree: " << existingTree->toString() << std::endl;

		}
		else if (choice == COMMAND_COMP) {
			ExpressionTree* clonedTree = new ExpressionTree(*existingTree);
			std::unordered_set<std::string> variableSet;
			int variablesSize = existingTree->countVariables(variableSet);



			if (variablesSize > 0) {
				std::cout << "Enter variable values: ( " << variablesSize << " )" << std::endl;
				std::cin.ignore();  // Ignore the newline character left in the input stream
				std::getline(std::cin, exp);

				if (variablesSize == existingTree->removeSpacesAndSymbols(exp).size()) {
					std::istringstream iss(exp);
					existingTree->assignVariableValues(iss);
					std::cout << "Result of the expression:  " << existingTree->compute() << std::endl;
					std::cout << existingTree->printErrors();
					std::cout << existingTree->printErrors();
					delete existingTree;  // Usuñ oryginalne drzewo
					existingTree = clonedTree;  // Przywróæ klonowane drzewo
				}
				else {
					std::cout << "Incorrect number of variables ";
				}
			}
			else {
				std::cout << "Result of the expression:  " << existingTree->compute() << std::endl;
				std::cout << existingTree->printErrors();
				delete existingTree;  // Usuñ oryginalne drzewo

				existingTree = clonedTree;  // Przywróæ klonowane drzewo

			}
		}
		else if (choice == COMMAND_JOIN) {
			std::cout << "Enter expression: " << std::endl;
			std::cin.ignore();  // Ignore the newline character left in the input stream
			std::getline(std::cin, exp);
			ExpressionTree* joinedTree = new ExpressionTree(exp);
			ExpressionTree* resultTree;

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

