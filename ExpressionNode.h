#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

enum type {
	OPERATOR,
	OPERAND,
	VARIABLE,
	NULL_TYPE
};
template<typename T>
class ExpressionNode {
public:
	ExpressionNode() {
		value = "0";
		type = NULL_TYPE;
	}
	ExpressionNode(const std::string& val) {
		value = val;
		set_type(val);
	}
	ExpressionNode(const ExpressionNode& otherNode) {
		copyNode(otherNode);
	}
	void addChild(ExpressionNode* node) {
		children.push_back(node);
		childrenCount++;
	}
	void setChild(int index, ExpressionNode* node) {
		children[index] = node;
	}
	ExpressionNode* getChild(int index) {
		return children[index];
	}
	void copyNode(const ExpressionNode& otherNode) {
		value = otherNode.value;
		type = otherNode.type;

		for (int i = 0; i < otherNode.childrenCount; i++)
		{
			addChild(new ExpressionNode(*otherNode.children[i]));

		}
	}
	~ExpressionNode() {
		for (int i = 0; i < childrenCount; i++)
		{
			delete children[i];
		}
	}

	std::string value;
	type type;
	int childrenCount;
	std::vector<ExpressionNode*> children;
	

private:

	void set_type(const std::string& val) {
		if (isOperator(val)) { type = OPERATOR; }
		else if (isOperand(val)) { type = OPERAND; }
		else if (isVariable(val)) { type = VARIABLE; }
		else { type = NULL_TYPE; }

	}
	bool isOperator(const std::string& token);

	bool isOperand(const std::string& token);
		
	
	bool isVariable(const std::string& token) {
		if (!std::isalpha(token[0])) {
			return false;
		}
		if (token == "sin" || token == "cos") {
			return false;
		}

		for (auto it = token.begin() + 1; it != token.end(); ++it) {
			if (!std::isalnum(*it)) {
				return false;
			}
		}

		return true;
	}

};

template <>
inline bool ExpressionNode<int>::isOperand(const std::string& token) {
	// Sprawdź, czy token jest liczbą całkowitą (int)
	std::istringstream iss(token);

	int testValue;
	if (iss >> testValue) {
		// Poprawnie wczytano liczbę całkowitą, sprawdź, czy cały token został przeczytany
		char remainingChar;
		if (iss >> remainingChar) {
			return false;  // Dodatkowy znak, np. '.' w przypadku liczby zmiennoprzecinkowej
		}
		return true;
	}

	return false;  // Nie jest liczbą całkowitą
}
template <>
inline  bool  ExpressionNode<double>::isOperand(const std::string& token) {

	// Sprawdź, czy token jest liczbą (int lub double)
	std::istringstream iss(token);

	double testValue;
	if (iss >> testValue) {
		// Poprawnie wczytano liczbę, więc to może być int lub double
		// Sprawdź, czy cały token został przeczytany
		char remainingChar;
		if (iss >> remainingChar) {
			return false;  // Dodatkowy znak, np. '.' w przypadku liczby zmiennoprzecinkowej
		}
		return true;
	}

	return false;  // Nie jest liczbą
}

template <>
inline  bool ExpressionNode<std::string>::isOperand(const std::string& token) {

	if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
		return true;  // Token jest stringiem w cudzysłowach
	}
	return false;  
}

template <>
inline bool ExpressionNode<int>::isOperator(const std::string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/" || token == "sin" || token == "cos";
}
template <>
inline bool ExpressionNode<double>::isOperator(const std::string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/" || token == "sin" || token == "cos";
}
template <>
inline bool ExpressionNode<std::string>::isOperator(const std::string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/";
}
#endif  // EXPRESSION_NODE_H
