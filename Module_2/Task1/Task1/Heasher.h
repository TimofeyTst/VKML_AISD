#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional> // для std::hash

using std::string;
using std::vector;

struct StringHasher {
	size_t operator()(const std::string& key) const {
		size_t hash = 0;
		for (size_t i = 0; i < key.size(); ++i) {
			hash = hash * 137 + key[i];
		}
		return hash;
	}
};

struct StringHasher1 {
	size_t operator()(const std::string& str) const {
		return std::hash<std::string>{}(str);
	}
};

struct StringHasher2 {
	size_t operator()(const std::string& str) const {
		return std::hash<std::string>{}(str) * 31;
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
	size_t hash = hasher(key) % table.size();
	for (size_t i = 1; i <= table.size(); ++i) {
		if (table[hash].state == HashTableNode::Key && table[hash].key == key) {
			return true;
		}
		hash = (hash + i * hasher2(key)) % table.size();
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
	size_t hash = absHash % table.size();
	size_t firstDeleted = UINT_MAX;
	for (size_t i = 1; i <= table.size(); ++i) {
		if (table[hash].state == HashTableNode::Empty) {
			if (firstDeleted != UINT_MAX) {
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
		if (table[hash].state == HashTableNode::Deleted && firstDeleted == UINT_MAX) {
			firstDeleted = hash;
		}
		hash = (absHash + i * hasher2(key)) % table.size(); // 38895331533755587
	}
	if (firstDeleted != UINT_MAX) {
		table[hash].key = key;
		table[hash].state = HashTableNode::Key;
		table[hash].absHash = absHash;
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
	size_t hash = absHash % table.size();
	for (size_t i = 1; i <= table.size(); ++i) {
		if (table[hash].state == HashTableNode::Empty) {
			return false;
		}
		if (table[hash].state == HashTableNode::Key && table[hash].key == key) {
			table[hash].state = HashTableNode::Deleted;
			--keysCount;
			return true;
		}
		hash = (absHash + i * hasher2(key)) % table.size();
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
			size_t hash = node.absHash % newTable.size();
			size_t i = 0;
			while (true) {
				if (newTable[hash].state == HashTableNode::Empty ||
					newTable[hash].state == HashTableNode::Deleted) {
					newTable[hash].key = node.key;
					newTable[hash].state = HashTableNode::Key;
					newTable[hash].absHash = node.absHash;
					break;
				}
				hash = (hash + i * hasher2(node.key)) % newTable.size();
				++i;
			}
		}
	}
	// Заменяем старую таблицу на новую
	table.swap(newTable);
}