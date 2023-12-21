#include "ExpressionNode.h"
#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
const std::string SIN =  "sin";
const std::string COS  = "cos";
const std::string DEFAULT_STR = "\"ala\"";
const std::string ERROR_1 = "ERROR: too many arguments \n";
const std::string ERROR_2 = "ERROR: missing argument \n";
const std::string ERROR_3 = "ERROR: ignoring invalid token: \n";
const std::string ERROR_4 = "Error: Not enough variable values provided. \n";
const std::string ERROR_5 = "Error: Division by zero. \n";
const std::string ERROR_6 = "Error: Unknown operator  \n";


template<typename T>
class ExpressionTree {
public:
	ExpressionTree() {
		root = nullptr;
		errors = "";
	}
	ExpressionTree(const std::string& expression) {
		errors = "";
		buildExpressionTree(expression);
	}
	ExpressionTree(const ExpressionTree& otherTree) {
		root = new ExpressionNode<T>(*otherTree.root);
		errors = otherTree.errors;
	}
	~ExpressionTree() { delete root; }
	 

	T comp();

	ExpressionTree* operator+(const ExpressionTree& otherTree) {
		ExpressionTree* result = new ExpressionTree(*this);
		ExpressionNode<T>* leaf = getLeafNode(result->root);
		leaf->copyNode(*otherTree.root);
		return result;
	}

	void operator=(const ExpressionTree& otherTree) {
		if (root != NULL) delete root;
		root = new ExpressionNode(*otherTree.root);
		errors = otherTree.errors;
	}


	std::string removeSpacesAndSymbols(const std::string& expression) {
		std::string result;
		std::istringstream iss(expression);
		std::string token;
		while (iss >> token) {
			result += token;
		}
		return result;
	}
	int countInputVariables(const std::string& expression) {
		int result = 0;
		std::istringstream iss(expression);
		std::string token;
		while (iss >> token) {
			result++;
		}
		return result;
	}
	void buildExpressionTree(const std::string& expression) {
		std::string modifiedExpression = expression;
		std::istringstream iss(modifiedExpression);
		root = buildTreeFromTokens(iss);
		if (!leftOver) {
			errors += ERROR_1;

		}

		fixExpressionTree(root);
	}
	void assignVariableValues(std::istringstream& iss) {
		assignVariableValuesRec(root, iss);
	}

	//how to do this
	std::string fixString(const std::string toFix) {
		std::string result = "";
		result =  toFix.substr(0,1) + toFix.substr(2, toFix.size() - 3);
		return result;
	}
	void fixExpressionTree(ExpressionNode<T>*& root) {
		if (root == nullptr) {
			return;
		}

		//rekurencyjnie napraw lewe i prawe poddrzewo
		for (int i = 0; i < root->childrenCount; i++) {
			fixExpressionTree(root->children[i]);
		}

		if ((root->type) == OPERATOR && (root->value == SIN || root->value == COS)) {

			if (root->children[0] == nullptr) {
				ExpressionNode<T>* oneNode = new ExpressionNode<T>("1");
				errors += ERROR_2;
				root->children[0] = oneNode;
				root->children[0]->type = oneNode->type;
			}

		}
		else if ((root->type) == OPERATOR && ((root->children[0] == nullptr) || (root->children[1] == nullptr))) {

			if (root->children[0] == nullptr) {
				ExpressionNode<T>* oneNode = new ExpressionNode<T>("1");
				errors += ERROR_2;
				root->children[0] = oneNode;
				root->children[0]->type = oneNode->type;
			}

			if (root->children[1] == nullptr) {
				ExpressionNode<T>* oneNode = new ExpressionNode<T>("1");
				errors += ERROR_2;
				root->children[1] = oneNode;
				root->children[1]->type = oneNode->type;
			}
		}
	
	}

	std::string toString() {
		return toStringRec(root);
	}
	void printVariables() {
		std::unordered_set<std::string> variableSet;
		collectVariables(root, variableSet);

		std::cout << "Variables in the expression tree:";
		for (const auto& variable : variableSet) {
			std::cout << " " << variable;
		}
		std::cout << " " << std::endl;
	}

	
	void collectVariables(ExpressionNode<T>* root, std::unordered_set<std::string>& variableSet) {
		if (root == nullptr) {
			return;
		}

		if (root->type == VARIABLE) {
			variableSet.insert(root->value);
		}

		//rekurencyjnie zbieraj zmienne z lewego i prawego poddrzewa
		for (int i = 0; i < root->childrenCount; i++)
		{
			collectVariables(root->children[i], variableSet);
		}
	}

	int countVariables(std::unordered_set<std::string>& variableSet) {
		collectVariables(root, variableSet);
		return variableSet.size();
	
	}
	std::string printErrors() { return errors; }
	bool isLeaf(ExpressionNode<T>* node) {
		return node != nullptr && node->childrenCount == 0;
	}
	



private:

	std::string errors;
	ExpressionNode<T>* root;
	bool leftOver;
	
	std::string concatenateStrings(std::string str1, std::string str2) const {
		return str1.substr(0,str1.size()-1) + str2.substr(1);
	}


	// Funkcja do usuwania ostatniego podstringu
	std::string removeLastSubstring(std::string str, std::string substr) const {
		str.erase(0, 1);
		str.erase(str.length() - 1, 1);
		substr.erase(0, 1);
		substr.erase(substr.length() - 1, 1);

		size_t pos = str.rfind(substr);
		if (pos != std::string::npos) {
			return str.substr(0, pos);
		}
		return str;
	
	}

	// Funkcja do mnożenia stringów
	std::string multiplyStrings(std::string str1, std::string str2) {
		if (str2.empty()) {
			return str1;
		}
		str1.erase(0, 1);
		str1.erase(str1.length() - 1, 1);
		str2.erase(0, 1);
		str2.erase(str2.length() - 1, 1);
		char firstChar = str2[0];
		std::string result = str1;
		for (int i = 0; i < result.size(); i++) {
			if (result.at(i) == firstChar) {
				result.insert(i + 1, str2.substr(1));
				i += str2.size();
			}
		}
		return result;
	}

	// Funkcja do dzielenia stringów
	std::string divideStrings(std::string str1, std::string str2) {
		if (str2.empty()) {
			return str1;
		}
		str1.erase(0, 1);
		str1.erase(str1.length() - 1, 1);
		str2.erase(0, 1);
		str2.erase(str2.length() - 1, 1);
		size_t pos = str1.find(str2);
		while (pos != std::string::npos) {
			// Keep only the first character of the divisor
			str1.erase(pos + 1, str2.size() - 1);
			pos = str1.find(str2, pos + 1);
		}

		return str1;
	}

	void assignVariableValuesRec(ExpressionNode<T>* root, std::istringstream& iss) {
		if (root == nullptr) {
			return;
		}
		

		if (root->type == VARIABLE) {
			std::string token;
			if (iss >> token) {
				root->value = token;
				root->type = OPERAND;
			}
			else {
				errors += ERROR_4;
			}
		}

		for (int i = 0; i < root->childrenCount; i++)
		{
			assignVariableValuesRec(root->children[i], iss);
		}
	}
	ExpressionNode<T>* buildTreeFromTokens(std::istringstream& iss) {
		std::string token;
		if (iss >> token) {
			ExpressionNode<T>* newNode = createNode(token);

			if (newNode != nullptr) {
				if (newNode->type == OPERATOR && (newNode->value == SIN || newNode->value == COS)) {
					newNode->addChild(buildTreeFromTokens(iss));
				}
				else if (newNode->type == OPERATOR) {
					newNode->addChild(buildTreeFromTokens(iss));
					newNode->addChild(buildTreeFromTokens(iss));
				}
			}
			leftOver = iss.eof();
			return newNode;
		}

		return nullptr;
	}
	ExpressionNode<T>* createNode(const std::string& token) {
		ExpressionNode<T>* newNode = new ExpressionNode<T>(token);

		if (newNode->type == OPERAND || newNode->type == OPERATOR || newNode->type == VARIABLE) {
			return newNode;
		}
		else {
			errors += ERROR_3 + token + "\n";
			delete newNode;
			return nullptr;
		}
	}

	std::string toStringRec(ExpressionNode<T>* root) {
		if (root == nullptr) {
			return "";
		}

		std::string result = root->value + " ";
		for (int i = 0; i < root->childrenCount; i++)
		{
			result += " " + toStringRec(root->children[i]);
		}

		return result;
	}


	ExpressionNode<T>* getLeafNode(ExpressionNode<T>* root) {
		bool allChildrenNull = true;
		for (int i = 0; i < root->childrenCount; i++)
		{
			if (root->children[i] != NULL) allChildrenNull = false;
		}
		if (allChildrenNull)
		{
			return root;
		}
		else
		{
			getLeafNode(root->children[root->childrenCount - 1]);
		}
	}

	std::string computeRecString(ExpressionNode<T>* root) {
		errors = "";
		if (!root) {
			return std::string();
		}

		// Jeśli węzeł jest operatorem
		if (root->type == OPERATOR) {

			if (root->value == "+") {
				return concatenateStrings(computeRecString(root->children[0]), computeRecString(root->children[1]));
			}
			else if (root->value == "-") {
				return removeLastSubstring(computeRecString(root->children[0]), computeRecString(root->children[1]));
			}
			else if (root->value == "*") {
				return multiplyStrings(computeRecString(root->children[0]), computeRecString(root->children[1]));
			}
			else if (root->value == "/") {
				return divideStrings(computeRecString(root->children[0]), computeRecString(root->children[1]));
			}
		}
		else {
			// W przeciwnym razie to jest string w cudzysłowie
			return root->value;
		}

		return std::string(); // Domyślna wartość dla pozostałych przypadków
	
	
	}
	double computeRec(ExpressionNode<T>* root) {
		errors = "";
		if (root == nullptr) {
			return 0.0;
		}

		if (root->type == OPERAND) {
			return std::stod(root->value);
		}

		else {

			if (root->value == "+") {
				return computeRec(root->children[0]) + computeRec(root->children[1]);
			}
			else if (root->value == "-") {
				return computeRec(root->children[0]) - computeRec(root->children[1]);
			}
			else if (root->value == "*") {
				return computeRec(root->children[0]) * computeRec(root->children[1]);
			}
			else if (root->value == "/") {
				double dzielnik = computeRec(root->children[1]);
				if (dzielnik != 0.0) {
					return computeRec(root->children[0]) / dzielnik;
				}
				else {
					errors += ERROR_5;
					return 0.0;
				}
			}
			else if (root->value == "sin") {
				return std::sin(computeRec(root->children[0]));
			}
			else if (root->value == "cos") {
				return std::cos(computeRec(root->children[0]));
			}
			else {
				errors += ERROR_6 + root->value + "\n";
				return 0.0;
			}
		}
	}
	std::string getKnownType();
};
template <typename T>
inline std::string ExpressionTree<T>::getKnownType()
{
	return "UNKNOWN";
}
template <>
inline std::string ExpressionTree<int>::getKnownType()
{
	return "INT";
}

template <>
inline std::string ExpressionTree<double>::getKnownType()
{
	return "DOUBLE";
}

template <>
inline std::string ExpressionTree<std::string>::getKnownType()
{
	return "STRING";
}
template <>
inline int ExpressionTree<int>::comp() {
	return computeRec(root);
}
template <>
inline double ExpressionTree<double>::comp() {
	return computeRec(root);
}
template <>
 inline std::string ExpressionTree<std::string>::comp() {
	return computeRecString(root);
}


 template <>
 inline void ExpressionTree<int>::fixExpressionTree(ExpressionNode<int>*& root) {
	 if (root == nullptr) {
		 return;
	 }

	 //rekurencyjnie napraw lewe i prawe poddrzewo
	 for (int i = 0; i < root->childrenCount; i++) {
		 fixExpressionTree(root->children[i]);
	 }

	 if ((root->type) == OPERATOR && (root->value == SIN || root->value == COS)) {

		 if (root->children[0] == nullptr) {
			 ExpressionNode<int>* oneNode = new ExpressionNode<int>("1");
			 errors += ERROR_2;
			 root->children[0] = oneNode;
			 root->children[0]->type = oneNode->type;
		 }

	 }
	 else if ((root->type) == OPERATOR && ((root->children[0] == nullptr) || (root->children[1] == nullptr))) {

		 if (root->children[0] == nullptr) {
			 ExpressionNode<int>* oneNode = new ExpressionNode<int>("1");
			 errors += ERROR_2;
			 root->children[0] = oneNode;
			 root->children[0]->type = oneNode->type;
		 }

		 if (root->children[1] == nullptr) {
			 ExpressionNode<int>* oneNode = new ExpressionNode<int>("1");
			 errors += ERROR_2;
			 root->children[1] = oneNode;
			 root->children[1]->type = oneNode->type;
		 }
	 }

 }


 template <>
 inline void ExpressionTree<double>::fixExpressionTree(ExpressionNode<double>*& root) {
	 if (root == nullptr) {
		 return;
	 }

	 //rekurencyjnie napraw lewe i prawe poddrzewo
	 for (int i = 0; i < root->childrenCount; i++) {
		 fixExpressionTree(root->children[i]);
	 }

	 if ((root->type) == OPERATOR && (root->value == SIN || root->value == COS)) {

		 if (root->children[0] == nullptr) {
			 ExpressionNode<double>* oneNode = new ExpressionNode<double>("1.0");
			 errors += ERROR_2;
			 root->children[0] = oneNode;
			 root->children[0]->type = oneNode->type;
		 }

	 }
	 else if ((root->type) == OPERATOR && ((root->children[0] == nullptr) || (root->children[1] == nullptr))) {

		 if (root->children[0] == nullptr) {
			 ExpressionNode<double>* oneNode = new ExpressionNode<double>("1.0");
			 errors += ERROR_2;
			 root->children[0] = oneNode;
			 root->children[0]->type = oneNode->type;
		 }

		 if (root->children[1] == nullptr) {
			 ExpressionNode<double>* oneNode = new ExpressionNode<double>("1.0");
			 errors += ERROR_2;
			 root->children[1] = oneNode;
			 root->children[1]->type = oneNode->type;
		 }
	 }

 }
 template <>
 inline void ExpressionTree<std::string>::fixExpressionTree(ExpressionNode<std::string>*& root) {
	 if (root == nullptr) {
		 return;
	 }
	 //rekurencyjnie napraw lewe i prawe poddrzewo
	 for (int i = 0; i < root->childrenCount; i++) {
		 fixExpressionTree(root->children[i]);
	 }

	 if ((root->type) == OPERATOR && ((root->children[0] == nullptr) || (root->children[1] == nullptr))) {

		 if (root->children[0] == nullptr) {
			 ExpressionNode<std::string>* oneNode = new ExpressionNode<std::string>(DEFAULT_STR);
			 errors += ERROR_2;
			 root->children[0] = oneNode;
			 root->children[0]->type = oneNode->type;
		 }

		 if (root->children[1] == nullptr) {
			 ExpressionNode<std::string>* oneNode = new ExpressionNode<std::string>(DEFAULT_STR);
			 errors += ERROR_2;
			 root->children[1] = oneNode;
			 root->children[1]->type = oneNode->type;
		 }
	 }

 }
