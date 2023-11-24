#include "ExpressionNode.h"


// Implementacje metod klasy ExpressionNode

ExpressionNode::ExpressionNode() : value(0), type(NULL_TYPE), left(nullptr), right(nullptr) {}
ExpressionNode::ExpressionNode(const std::string& val) {
	value = val;
	set_type(val);
	left = nullptr;
	right = nullptr;
}

ExpressionNode::ExpressionNode(const ExpressionNode& cOther)
{
	copyNode(cOther);
}
void ExpressionNode::copyNode(const ExpressionNode& cOther) {
	value = cOther.value;
	type = cOther.type;
	if (cOther.left != NULL) {
		left = new ExpressionNode(*cOther.left);
	}
	if (cOther.right != NULL) {
		right = new ExpressionNode(*cOther.right);
	}
}

void ExpressionNode::set_type(const std::string& val) {
	if (isOperator(val)) { type = OPERATOR; }
	else if (isOperand(val)) { type = OPERAND; }
	else if (isVariable(val)) { type = VARIABLE; }
	else { type = NULL_TYPE; }

}
bool ExpressionNode::isOperator(const std::string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/" || token == "sin" || token == "cos";
}

bool ExpressionNode::isOperand(const std::string& token) {
	// SprawdŸ, czy token sk³ada siê z cyfr (mo¿e byæ wielocyfrowy)
	for (char c : token) {
		if (!std::isdigit(c)) {
			return false;
		}
	}
	return true;
}

bool ExpressionNode::isVariable(const std::string& token) {
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


