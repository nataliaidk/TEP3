#include "ExpressionNode.h"
#include <sstream>
#include <unordered_set>

class ExpressionParser {
public:
    std::string removeSpacesAndSymbols(const std::string& expression);
    ExpressionNode* buildExpressionTree(const std::string& expression);
    ExpressionNode* mergeTrees(ExpressionNode* treeA, ExpressionNode* treeB);
    void fixExpressionTree(ExpressionNode*& root);
    ExpressionNode* addNodeToTree(ExpressionNode* tree, const std::string& operand);
    ExpressionNode* joinExpressions(ExpressionNode* existingTree, const std::string& expression);
    std::string toString(ExpressionNode* root);
    void printVariables(ExpressionNode* root);
    void assignVariableValues(ExpressionNode* root, std::istringstream& iss);
    double compute(ExpressionNode* root);
    void collectVariables(ExpressionNode* root, std::unordered_set<std::string>& variableSet);

    bool isLeaf(ExpressionNode* node);

private:
    ExpressionNode* buildTreeFromTokens(std::istringstream& iss);
    ExpressionNode* createNode(const std::string& token);
    bool isOperator(const std::string& token);
    bool isOperand(const std::string& token);
    bool isVariable(const std::string& token);
    bool isNumber(const std::string& token);
    ExpressionNode* getLeafNode(ExpressionNode* root);
};