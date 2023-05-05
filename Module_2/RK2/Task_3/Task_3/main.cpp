#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <unordered_set>

struct Product {
	std::string product_name;
	int color;
	int size;
};

bool operator==(const Product& l, const Product& r) {
	return l.product_name == r.product_name
		&& l.color == r.color
		&& l.size == r.size;
}

namespace std {
	template <> // Чтобы переопределить стркутуру лишь для конкретного типа
	struct hash<Product> {
		size_t operator()(const Product& prd) const {
			return std::hash<std::string>()(prd.product_name) +
				std::hash<int>()(prd.color) +
				std::hash<int>()(prd.size);
		}
	};
}

void run(std::istream& input, std::ostream& output) {
	std::unordered_set<Product> set;

	char operation = 0;
	Product product;
	while (input >> operation >> product.product_name >> product.color >> product.size) {
		switch (operation) {
		case '+':
			if (set.find(product) == set.end()) {
				set.insert(product);
				output << "OK" << std::endl;
			} else {
				output << "FAIL" << std::endl;
			}
			break;
		case '-':
			output << (set.erase(product) ? "OK" : "FAIL") << std::endl;
			break;
		case '?':
			output << (set.find(product) != set.end() ? "OK" : "FAIL") << std::endl;
			break;
		}
	}
}

void test() {
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ a 1 2\n+ a 1 1\n+ a 2 1\n? a 1 1\n? b 2 1\n? a 1 3\n? b 1 1\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\n");
	}
}

int main()
{
	//test();
	run(std::cin, std::cout);
	return 0;
}