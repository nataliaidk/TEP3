#include "ExpressionTree.h"

#include <iostream>


#include <iostream>
#include <cmath>
#include "ExpressionNode.h"

ExpressionTree::ExpressionTree() {
	root = nullptr;
	errors = "";
}
ExpressionTree::ExpressionTree(const std::string& expression) {
	errors = "";
	buildExpressionTree(expression);

}
ExpressionTree::ExpressionTree(const ExpressionTree& cOther) {
	root = new ExpressionNode(*cOther.root);
	errors = cOther.errors;
}
ExpressionTree::~ExpressionTree() {
	  delete root;
}
ExpressionTree* ExpressionTree::operator+(const ExpressionTree& cOther)
{

	ExpressionTree* result = new ExpressionTree(*this);
	ExpressionNode* leaf = getLeafNode(result->root);
	leaf->copyNode(*cOther.root);
	return result;

}
void ExpressionTree::operator=(const ExpressionTree& cOther)
{
	if (root != NULL) delete root;
	root = new ExpressionNode(*cOther.root);
	errors = cOther.errors;
}


std::string ExpressionTree::removeSpacesAndSymbols(const std::string& expression) {
	std::string result;
	std::istringstream iss(expression);
	std::string token;
	while (iss >> token) {
		result += token;
	}
	return result;
}

void ExpressionTree::buildExpressionTree(const std::string& expression) {
	std::string modifiedExpression = expression;
	std::istringstream iss(modifiedExpression);
	root = buildTreeFromTokens(iss);
	if (!leftOver) {
		errors += ERROR_1;

	}

	// Napraw drzewo o zbudowaniu
	fixExpressionTree(root);


}



void ExpressionTree::fixExpressionTree(ExpressionNode*& root) {
	if (root == nullptr) {
		return;
	}

	// Rekurencyjnie napraw lewe i prawe poddrzewo
	fixExpressionTree(root->left);
	fixExpressionTree(root->right);

	// Je�li w�ze� jest operatorem i nie ma co najmniej jednego dziecka (operandu),
	// dodaj nowy w�ze� z warto�ci� 1 jako brakuj�ce dziecko.
	if ((root->type) == OPERATOR && (root->value == "sin" || root->value == "cos")) {
		ExpressionNode* oneNode = new ExpressionNode("1");
		if (root->left == nullptr && root->right == nullptr) {
			errors += ERROR_2;
			root->left = oneNode;
			root->left->type = oneNode->type;
		}

	}
	else if ((root->type) == OPERATOR && ((root->left == nullptr) || (root->right == nullptr))) {
		ExpressionNode* oneNode = new ExpressionNode("1");

		// Je�li lewe dziecko nie istnieje, dodaj nowy w�ze� z warto�ci� 1
		if (root->left == nullptr) {
			errors += ERROR_2;
			root->left = oneNode;
			root->left->type = oneNode->type;
		}

		// Je�li prawe dziecko nie istnieje, dodaj nowy w�ze� z warto�ci� 1
		if (root->right == nullptr) {
			errors += ERROR_2;
			root->right = oneNode;
			root->right->type = oneNode->type;
		}
	}
}





std::string ExpressionTree::toString() {

	return toStringRec(root);


}

void ExpressionTree::printVariables() {
	std::unordered_set<std::string> variableSet;
	collectVariables(root, variableSet);

	std::cout << "Variables in the expression tree:";
	for (const auto& variable : variableSet) {
		std::cout << " " << variable;
	}
	std::cout << " " << std::endl;
}


void ExpressionTree::assignVariableValues(std::istringstream& iss) {
	assignVariableValuesRec(root, iss);
}



void ExpressionTree::collectVariables(ExpressionNode* root, std::unordered_set<std::string>& variableSet) {
	if (root == nullptr) {
		return;
	}

	if (root->type == VARIABLE) {
		variableSet.insert(root->value);
	}

	// Rekurencyjnie zbieraj zmienne z lewego i prawego poddrzewa
	collectVariables(root->left, variableSet);
	collectVariables(root->right, variableSet);
}


double ExpressionTree::compute() {
	return computeRec(root);

}

int ExpressionTree::countVariables(std::unordered_set<std::string>& variableSet) {
	collectVariables(root, variableSet);
	return variableSet.size();

}

std::string ExpressionTree::printErrors()
{
	return errors;
}

bool ExpressionTree::isLeaf(ExpressionNode* node) {
	return node != nullptr && node->left == nullptr && node->right == nullptr;
}

ExpressionNode* ExpressionTree::buildTreeFromTokens(std::istringstream& iss) {

	std::string token;
	if (iss >> token) {
		ExpressionNode* newNode = createNode(token);

		if (newNode != nullptr) {
			if (newNode->type == OPERATOR && (newNode->value == "sin" || newNode->value == "cos")) {
				newNode->left = buildTreeFromTokens(iss);

			}
			else if (newNode->type == OPERATOR) {
				newNode->left = buildTreeFromTokens(iss);
				newNode->right = buildTreeFromTokens(iss);
			}
		}
		leftOver = iss.eof();
		return newNode;
	}

	return nullptr;
}

ExpressionNode* ExpressionTree::createNode(const std::string& token) {
	ExpressionNode* newNode = new ExpressionNode(token);

	if (newNode->type == OPERAND || newNode->type == OPERATOR || newNode->type == VARIABLE) {
		return new ExpressionNode(token);
	}
	else {
		errors += ERROR_3 + token + "\n";
		delete newNode;
		return nullptr;
	}
}

std::string ExpressionTree::toStringRec(ExpressionNode* root) {
	if (root == nullptr) {
		return "";
	}

	std::string result = root->value;

	if (root->left != nullptr || root->right != nullptr) {
		result += " (" + toStringRec(root->left) + " " + toStringRec(root->right) + ")";
	}

	return result;
}

void ExpressionTree::assignVariableValuesRec(ExpressionNode* root, std::istringstream& iss) {
	if (root == nullptr) {
		return;
	}
	if (root->value == "sin" || root->value == "cos") {
		return;
	}

	if (root->type == VARIABLE) {
		std::string token;
		if (iss >> token) {
			root->value = token;
			root->type = OPERAND;  // Zmiana typu na operand
		}
		else {
			errors += ERROR_4;
		}
	}

	// Rekurencyjnie przypisuj warto�ci zmiennym w lewym i prawym poddrzewie
	assignVariableValuesRec(root->left, iss);
	assignVariableValuesRec(root->right, iss);
}
ExpressionNode* ExpressionTree::getLeafNode(ExpressionNode* root) {
	if (root == nullptr) {
		return nullptr;
	}

	if (root->left == nullptr && root->right == nullptr) {
		// Znaleziono li��
		return root;
	}

	// Rekurencyjnie szukaj li�cia w lewym i prawym poddrzewie
	ExpressionNode* leftLeaf = getLeafNode(root->left);
	ExpressionNode* rightLeaf = getLeafNode(root->right);

	// Zwr�� li�� z lewego poddrzewa, je�li jest niepusty, w przeciwnym razie z prawego
	return (leftLeaf != nullptr) ? leftLeaf : rightLeaf;
}
double ExpressionTree::computeRec(ExpressionNode* root) {
	errors = "";
	if (root == nullptr) {
		return 0.0;
	}

	if (root->type == OPERAND) {
		return std::stod(root->value); // Konwertuj string na double dla operand�w
	}

	else {
		double leftValue = computeRec(root->left);
		double rightValue = computeRec(root->right);

		// Wykonaj odpowiedni� operacj� dla operatora
		if (root->value == "+") {
			return leftValue + rightValue;
		}
		else if (root->value == "-") {
			return leftValue - rightValue;
		}
		else if (root->value == "*") {
			return leftValue * rightValue;
		}
		else if (root->value == "/") {
			if (rightValue != 0.0) {
				return leftValue / rightValue;
			}
			else {
				errors += ERROR_5;
				return 0.0; // Handle division by zero
			}
		}
		else if (root->value == "sin") {
			return std::sin(leftValue);
		}
		else if (root->value == "cos") {
			return std::cos(leftValue);
		}
		else {
			errors += ERROR_6 + root->value + "\n";
			return 0.0; // Handle unknown operators
		}
	}
}