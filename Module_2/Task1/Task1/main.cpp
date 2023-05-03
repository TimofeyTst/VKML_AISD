//#include "Heasher.h"
#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <functional> // для std::hash



using std::string;
using std::vector;


struct StringHasher {
	size_t operator()(const std::string& key) const {
		size_t hash = 0;
		for (size_t i = 0; i < key.size(); ++i) {
			hash = hash * 31 + key[i];
		}
		return hash;
	}
};

struct StringHasher1 {
	size_t operator()(const std::string& key) const {
		const size_t p = 31;
		const size_t m = 1e9 + 9;
		size_t hash = 0;
		size_t p_pow = 1;
		for (const char c : key) {
			hash = (hash + (c - 'a' + 1) * p_pow) % m;
			p_pow = (p_pow * p) % m;
		}
		return hash;
	}
};


struct StringHasher2 {
	size_t operator()(const std::string& key, size_t table_size) const {
		size_t hash = 0;
		for (const char c : key) {
			hash = hash * 137 + c;
		}
		hash %= (table_size - 1);
		if (hash % 2 == 0) hash += 1;
		return hash;
	}
};

template<class T, class H1, class H2>
class HashTable {
public:
	HashTable(int initialSize, const H1& _hasher, const H2& _hasher2);
	~HashTable() {};

	bool Has(const T& key) const;
	bool Add(const T& key);
	bool Delete(const T& key);

private:
	H1 hasher;
	H2 hasher2;
	struct HashTableNode {
		T key;
		enum State { Empty, Key, Deleted };
		State state;
		size_t absHash;

		HashTableNode() : state(Empty) {}
	};
	vector<HashTableNode> table;
	size_t keysCount;

	void growTable();

};


template<class T, class H1, class H2>
HashTable<T, H1, H2>::HashTable(int initialSize, const H1& _hasher, const H2& _hasher2) :
	hasher(_hasher),
	hasher2(_hasher2),
	keysCount(0),
	table(initialSize)
{}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Has(const T& key) const {
	// Key: key = Key ? return true : пробируемся дальше
	// Deleted: пробируемся дальше
	// Empty: return false
	size_t absHash = hasher(key);
	size_t absHash2 = hasher2(key, table.size());
	for (size_t i = 0; i < table.size(); ++i) {
		size_t hash = (absHash + i * absHash2) % table.size();
		if (table[hash].state == HashTableNode::Key && table[hash].key == key) {
			return true;
		}
		if (table[hash].state == HashTableNode::Empty) {
			return false;
		}
	}
	return false;
}


template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Add(const T& key) {
	// Key: key = Key ? return false : пробируемся дальше
	// Deleted: встречали ранее Deleted ? идём дальше : запоминаем позицию firstDeleted и идём дальше
	// Empty: вставляем ключ в firstDeleted (если такой был) или в текущую позицию и return true
	if (keysCount * 4 >= 3 * table.size()) {
		growTable();
	}
	size_t absHash = hasher(key);
	size_t absHash2 = hasher2(key, table.size());
	bool was_firstDeleted = false;
	size_t firstDeleted = 0;
	size_t hash = absHash % table.size();

	for (size_t i = 1; i <= table.size(); ++i) {

		if (table[hash].state == HashTableNode::Empty) {
			if (was_firstDeleted) {
				hash = firstDeleted; // Если firstDeleted был инициализирован
			}
			table[hash].key = key;
			table[hash].state = HashTableNode::Key;
			table[hash].absHash = absHash;
			++keysCount;
			return true;
		}
		if (table[hash].state == HashTableNode::Key && table[hash].key == key) {
			return false;
		}
		if (table[hash].state == HashTableNode::Deleted && !was_firstDeleted) {
			firstDeleted = hash;
			was_firstDeleted = true;
		}

		hash = (absHash + i * absHash2) % table.size();
	}
	if (was_firstDeleted) {
		table[firstDeleted].key = key;
		table[firstDeleted].state = HashTableNode::Key;
		table[firstDeleted].absHash = absHash;
		++keysCount;
		return true;
	}
	return false;
}


template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Delete(const T& key) {
	// Key: key == Key ? помечаем ячейку Deleted и return true : пробируемся дальше
	// Deleted: пробируемся дальше
	// Empty: return false
	size_t absHash = hasher(key);
	size_t absHash2 = hasher2(key, table.size());

	for (size_t i = 0; i < table.size(); ++i) {
		size_t hash = (absHash + i * absHash2) % table.size();

		if (table[hash].state == HashTableNode::Empty) {
			return false;
		}
		if (table[hash].state == HashTableNode::Key && table[hash].key == key) {
			table[hash].state = HashTableNode::Deleted;
			--keysCount;
			return true;
		}
	}
	return false;
}


template<class T, class H1, class H2>
void HashTable<T, H1, H2 >::growTable() {
	std::vector<HashTableNode> newTable(table.size() * 2);

	// Перехешируем все ключи в новую таблицу
	for (auto& node : table) {
		if (node.state == HashTableNode::Key) {
			// Добавляем ключ в новую таблицу
			size_t i = 0;
			size_t absHash2 = hasher2(node.key, newTable.size());
			while (true) {
				size_t hash = (node.absHash + i * absHash2) % newTable.size();

				if (newTable[hash].state == HashTableNode::Empty ||
					newTable[hash].state == HashTableNode::Deleted) {
					newTable[hash].key = node.key;
					newTable[hash].state = HashTableNode::Key;
					newTable[hash].absHash = node.absHash;
					break;
				}
				++i;
			}
		}
	}
	// Заменяем старую таблицу на новую
	table.swap(newTable);
}


void run(std::istream& input, std::ostream& output) {
	StringHasher1 hasher1;
	StringHasher2 hasher2;
	HashTable<std::string, StringHasher1, StringHasher2> table(8, hasher1, hasher2);
	char operation = 0;
	string word;
	while (input >> operation >> word) {
		switch (operation) {
		case '+':
			output << (table.Add(word) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			output << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
			break;
		case '?':
			output << (table.Has(word) ? "OK" : "FAIL") << std::endl;
			break;
		}
	}
}


void test() {
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n? bye\n+ bye\n- bye\n? bye\n? hello";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "? bye\n+ bye\n- bye\n+ bye";
		run(input, output);
		assert(output.str() == "FAIL\nOK\nOK\nOK\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "? bye\n+ asd\n? asd\n+ test\n? asd\n? test\n- asd\n? test\n? asd";
		run(input, output);
		assert(output.str() == "FAIL\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ ala\n+ ala2\n+ ala3\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\n");
	} 
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n+ world\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ abc\n+ def\n+ ghi\n+ jkl\n+ mno\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nOK\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n- hello\n- bye\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ abc\n+ def\n+ ghi\n- abc\n- ghi\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nOK\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n+ world\n? hello\n? bye\n? world\n? test\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nFAIL\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ abc\n+ def\n+ ghi\n? def\n? ghi\n? abc\n? mno\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nFAIL\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n+ hello\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nFAIL\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ abc\n+ def\n+ ghi\n+ def\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nFAIL\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n- hello\n- hello\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nFAIL\n");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ abc\n+ def\n- def\n- def\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nFAIL\n");
	}
	// тест на добавление и поиск существующих элементов
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n? bye\n+ bye\n- bye\n? bye\n? hello";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
	}

	// тест на добавление и удаление элемента
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n- hello\n? hello";
		run(input, output);
		assert(output.str() == "OK\nOK\nFAIL\n");
	}

	// тест на добавление и удаление нескольких элементов
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ hello\n+ bye\n- hello\n? hello\n? bye\n- bye\n? hello\n? bye";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nOK\nFAIL\nFAIL\n");
	}
	// тест на добавление большого количества элементов и их удаление
	{
		std::stringstream input;
		std::stringstream output;
		std::string should_be = "";
		for (int i = 0; i < 100000; i++) {
			input << "+ elem_" << i << "\n";
			should_be += "OK\n";
		}
		for (int i = 0; i < 100000; i++) {
			input << "- elem_" << i << "\n";
			should_be += "OK\n";
		}
		for (int i = 0; i < 100000; i++) {
			input << "? elem_" << i << "\n";
			should_be += "FAIL\n";
		}
		run(input, output);
		assert(output.str() == should_be);
	}
	// тест на добавление и поиск большого количества элементов
	{
		std::stringstream input;
		std::stringstream output;
		std::string should_be = "";
		for (int i = 0; i < 100000; i++) {
			input << "+ elem_" << i << "\n";
			should_be += "OK\n";
		}
		for (int i = 0; i < 100000; i++) {
			input << "? elem_" << i << "\n";
			should_be += "OK\n";
		}
		run(input, output);
		assert(output.str() == should_be);
	}
	// тест на добавление большого количества элементов и их удаление
	{
		std::stringstream input;
		std::stringstream output;
		std::string should_be = "";
		for (int i = 0; i < 100000; i++) {
			input << "+ elem_" << i << "\n";
			should_be += "OK\n";
		}
		for (int i = 0; i < 50000; i++) {
			input << "- elem_" << i << "\n";
			should_be += "OK\n";
		}
		for (int i = 0; i < 50000; i++) {
			input << "? elem_" << i << "\n";
			should_be += "FAIL\n";
		}
		for (int i = 50000; i < 100000; i++) {
			input << "? elem_" << i << "\n";
			should_be += "OK\n";
		}
		run(input, output);
		assert(output.str() == should_be);
	}
	// тест на добавление и удаление элементов с совпадающими хеш-значениями
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ a\n+ b\n+ c\n+ abc\n+ bac\n- abc\n? a\n? b\n? c\n? abc\n? bac\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\n");
	}
}

void test2() {
	// тест на добавление элемента в пустое множество
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ elem\n";
		run(input, output);
		assert(output.str() == "OK\n");
	}

	// тест на добавление существующего элемента
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ elem\n+ elem\n";
		run(input, output);
		assert(output.str() == "OK\nFAIL\n");
	}

	// тест на удаление несуществующего элемента
	{
		std::stringstream input;
		std::stringstream output;
		input << "- elem\n";
		run(input, output);
		assert(output.str() == "FAIL\n");
	}

	// тест на добавление и удаление элемента
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ elem\n- elem\n";
		run(input, output);
		assert(output.str() == "OK\nOK\n");
	}

	// тест на поиск несуществующего элемента
	{
		std::stringstream input;
		std::stringstream output;
		input << "? elem\n";
		run(input, output);
		assert(output.str() == "FAIL\n");
	}

	// тест на добавление, удаление и поиск элемента
	{
		std::stringstream input;
		std::stringstream output;
		input << "+ elem\n- elem\n? elem\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nFAIL\n");
	}

	// тест на добавление элементов, перехеширование и поиск
	{
		std::stringstream input;
		std::stringstream output;
		for (int i = 0; i < 10; i++) {
			input << "+ elem_" << i << "\n";
		}
		input << "+ elem\n";
		run(input, output);
		assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n");
	}

	// тест на добавление элементов до перехеширования и после перехеширования
	{
		std::stringstream input;
		std::stringstream output;
		std::string should_be;
		for (int i = 0; i < 5; i++) {
			input << "+ elem_" << i << "\n";
			should_be += "OK\n";
		}
		input << "+ elem\n";
		should_be += "OK\n";
		for (int i = 5; i < 13; i++) {
			input << "+ elem_" << i << "\n";
			should_be += "OK\n";
		}
		run(input, output);
		assert(output.str() == should_be);
	}
}


int main() {
	//test();
	//test2();
	run(std::cin, std::cout);

	return 0;
}