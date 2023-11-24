#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include <string>

enum type {
	OPERATOR,
	OPERAND,
	VARIABLE,
	NULL_TYPE
};

class ExpressionNode {
public:
	std::string value;
	type type;
	ExpressionNode* left;
	ExpressionNode* right;

	ExpressionNode();
	ExpressionNode(const std::string& val);
	ExpressionNode(const ExpressionNode& cOther);
	void copyNode(const ExpressionNode& cOther);


private:
	void set_type(const std::string& val);
	bool isOperator(const std::string& token);
	bool isOperand(const std::string& token);
	bool isVariable(const std::string& token);

};

#endif  // EXPRESSION_NODE_H
