#include "ExpressionParser.h"
#include <iostream>


#include <iostream>
#include <cmath>
#include "ExpressionNode.h"



std::string ExpressionParser::removeSpacesAndSymbols(const std::string& expression) {
    std::string result;
    std::istringstream iss(expression);
    std::string token;
    while (iss >> token) {
        result += token;
    }
    return result;
}

ExpressionNode* ExpressionParser::buildExpressionTree(const std::string& expression) {
    std::string modifiedExpression = expression;
    std::istringstream iss(modifiedExpression);
    ExpressionNode* root = buildTreeFromTokens(iss);

    // Napraw drzewo po zbudowaniu
    fixExpressionTree(root);

    return root;
}

ExpressionNode* ExpressionParser::mergeTrees(ExpressionNode* treeA, ExpressionNode* treeB) {
    if (treeA == nullptr) {
        return treeB;
    }

    if (treeB == nullptr) {
        return treeA;
    }

    // ZnajdŸ liœæ w drzewie A
    ExpressionNode* leafA = getLeafNode(treeA);

    if (leafA != nullptr) {
        // Zast¹p liœæ w drzewie A drzewem B
        leafA->value = treeB->value;
        leafA->type = treeB->type;
        delete leafA->left;
        delete leafA->right;
        leafA->left = treeB->left;
        leafA->right = treeB->right;

        delete treeB; // Zwolnij pamiêæ drzewa B

        return treeA;
    }
    // Jeœli nie uda³o siê znaleŸæ liœcia w drzewie A, po prostu po³¹cz drzewa
    ExpressionNode* mergedNode = new ExpressionNode("");
    mergedNode->left = treeA;
    mergedNode->right = treeB;

    return mergedNode;
}

void ExpressionParser::fixExpressionTree(ExpressionNode*& root) {
    if (root == nullptr) {
        return;
    }

    // Rekurencyjnie napraw lewe i prawe poddrzewo
    fixExpressionTree(root->left);
    fixExpressionTree(root->right);

    // Jeœli wêze³ jest operatorem i nie ma co najmniej jednego dziecka (operandu),
    // dodaj nowy wêze³ z wartoœci¹ 1 jako brakuj¹ce dziecko.
    if (isOperator(root->value) && ((root->left == nullptr) || (root->right == nullptr))) {
        ExpressionNode* oneNode = new ExpressionNode("1");

        // Jeœli lewe dziecko nie istnieje, dodaj nowy wêze³ z wartoœci¹ 1
        if (root->left == nullptr) {
            root->left = oneNode;
        }

        // Jeœli prawe dziecko nie istnieje, dodaj nowy wêze³ z wartoœci¹ 1
        if (root->right == nullptr) {
            root->right = oneNode;
        }
    }
}

ExpressionNode* ExpressionParser::addNodeToTree(ExpressionNode* tree, const std::string& operand) {
    ExpressionNode* newOperandNode = new ExpressionNode(operand);
    return mergeTrees(tree, newOperandNode);
}

ExpressionNode* ExpressionParser::joinExpressions(ExpressionNode* existingTree, const std::string& expression) {
    // Buduj drzewo z nowego wyra¿enia
    ExpressionNode* newTree = buildExpressionTree(expression);

    // Po³¹cz drzewa za pomoc¹ operatora "="
    existingTree = mergeTrees(existingTree, newTree);

    return existingTree;
}

std::string ExpressionParser::toString(ExpressionNode* root) {
    if (root == nullptr) {
        return "";
    }

    std::string result = root->value;

    if (root->left != nullptr || root->right != nullptr) {
        result += " (" + toString(root->left) + " " + toString(root->right) + ")";
    }

    return result;
}

void ExpressionParser::printVariables(ExpressionNode* root) {
    std::unordered_set<std::string> variableSet;
    collectVariables(root, variableSet);

    std::cout << "Variables in the expression tree:";
    for (const auto& variable : variableSet) {
        std::cout << " " << variable;
    }
    std::cout << std::endl;
}

void ExpressionParser::assignVariableValues(ExpressionNode* root, std::istringstream& iss) {
    if (root == nullptr) {
        return;
    }

    if (isVariable(root->value)) {
        std::string token;
        if (iss >> token) {
            root->value = token;
            root->type = OPERAND;  // Zmiana typu na operand
        }
        else {
            std::cerr << "Error: Not enough variable values provided." << std::endl;
        }
    }

    // Rekurencyjnie przypisuj wartoœci zmiennym w lewym i prawym poddrzewie
    assignVariableValues(root->left, iss);
    assignVariableValues(root->right, iss);
}

double ExpressionParser::compute(ExpressionNode* root) {
    if (root == nullptr) {
        return 0.0;
    }

    if (isOperand(root->value)) {
        return std::stod(root->value); // Konwertuj string na double dla operandów
    }

    else {
        double leftValue = compute(root->left);
        double rightValue = compute(root->right);

        // Wykonaj odpowiedni¹ operacjê dla operatora
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
                std::cerr << "Error: Division by zero." << std::endl;
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
            std::cerr << "Error: Unknown operator " << root->value << std::endl;
            return 0.0; // Handle unknown operators
        }
    }
}

void ExpressionParser::collectVariables(ExpressionNode* root, std::unordered_set<std::string>& variableSet) {
    if (root == nullptr) {
        return;
    }

    if (isVariable(root->value)) {
        variableSet.insert(root->value);
    }

    // Rekurencyjnie zbieraj zmienne z lewego i prawego poddrzewa
    collectVariables(root->left, variableSet);
    collectVariables(root->right, variableSet);
}

bool ExpressionParser::isLeaf(ExpressionNode* node) {
    return node != nullptr && node->left == nullptr && node->right == nullptr;
}

ExpressionNode* ExpressionParser::buildTreeFromTokens(std::istringstream& iss) {
    std::string token;
    if (iss >> token) {
        ExpressionNode* newNode = createNode(token);

        if (newNode != nullptr) {
            if (isOperator(newNode->value)) {
                newNode->left = buildTreeFromTokens(iss);
                newNode->right = buildTreeFromTokens(iss);
            }
        }

        return newNode;
    }

    return nullptr;
}

ExpressionNode* ExpressionParser::createNode(const std::string& token) {
    if (isOperator(token) || isVariable(token) || isNumber(token)) {
        return new ExpressionNode(token);
    }
    else {
        std::cout << "Ignoring invalid token: " << token << std::endl;
        return nullptr;
    }
}

bool ExpressionParser::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "sin" || token == "cos";
}

bool ExpressionParser::isOperand(const std::string& token) {
    // SprawdŸ, czy token sk³ada siê z cyfr (mo¿e byæ wielocyfrowy)
    for (char c : token) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool ExpressionParser::isVariable(const std::string& token) {
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

bool ExpressionParser::isNumber(const std::string& token) {
    for (char c : token) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

ExpressionNode* ExpressionParser::getLeafNode(ExpressionNode* root) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->left == nullptr && root->right == nullptr) {
        // Znaleziono liœæ
        return root;
    }

    // Rekurencyjnie szukaj liœcia w lewym i prawym poddrzewie
    ExpressionNode* leftLeaf = getLeafNode(root->left);
    ExpressionNode* rightLeaf = getLeafNode(root->right);

    // Zwróæ liœæ z lewego poddrzewa, jeœli jest niepusty, w przeciwnym razie z prawego
    return (leftLeaf != nullptr) ? leftLeaf : rightLeaf;
}
