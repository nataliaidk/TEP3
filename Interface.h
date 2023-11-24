
#include "ExpressionTree.h"


const std::string COMMAND_ENTER = "enter";
const std::string COMMAND_VARS = "vars";

const std::string COMMAND_PRINT = "print";

const std::string COMMAND_COMP = "comp";

const std::string COMMAND_JOIN = "join";

const std::string COMMAND_STOP = "stop";

class ExpressionInterface {
public:

	void run();

private:

	bool stop = true;
	ExpressionTree* existingTree;
	std::string exp;
};