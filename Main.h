
#include "interface.h"

template<typename T>
int main() {
	ExpressionInterface<T> interface; // Change int to the desired type
	interface.start();
	return 0; // Ensure to return an integer from main
}
