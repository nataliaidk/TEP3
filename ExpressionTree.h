#include "ExpressionNode.h"
#include <sstream>
#include <unordered_set>

const std::string ERROR_1 = "ERROR: too many arguments \n";
const std::string ERROR_2 = "ERROR: missing argument \n";
const std::string ERROR_3 = "ERROR: ignoring invalid token: ";
const std::string ERROR_4 = "Error: Not enough variable values provided. \n";
const std::string ERROR_5 = "Error: Division by zero. \n";
const std::string ERROR_6 = "Error: Unknown operator ";


class ExpressionTree {
public:
	ExpressionTree();
	ExpressionTree(const std::string& expression);
	ExpressionTree(const ExpressionTree& cOther);
	~ExpressionTree();

	ExpressionTree* operator+(const ExpressionTree& cOther);

	void operator=(const ExpressionTree& cOther);


	std::string removeSpacesAndSymbols(const std::string& expression);
	void buildExpressionTree(const std::string& expression);
	void assignVariableValues(std::istringstream& iss);

	void fixExpressionTree(ExpressionNode*& root);

	std::string toString();
	void printVariables();

	double compute();
	void collectVariables(ExpressionNode* root, std::unordered_set<std::string>& variableSet);

	int countVariables(std::unordered_set<std::string>& variableSet);
	std::string printErrors();
	bool isLeaf(ExpressionNode* node);

private:

	std::string errors;
	ExpressionNode* root;
	bool leftOver;


	void assignVariableValuesRec(ExpressionNode* root, std::istringstream& iss);
	ExpressionNode* buildTreeFromTokens(std::istringstream& iss);
	ExpressionNode* createNode(const std::string& token);

	std::string toStringRec(ExpressionNode* root);

	ExpressionNode* getLeafNode(ExpressionNode* root);
	double computeRec(ExpressionNode* root);
};