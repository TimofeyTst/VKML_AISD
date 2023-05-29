#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>

#include<array>
#include<cassert>
#include<queue>
#include<iostream>
#include<string>
#include<unordered_map>

using std::array;
using std::queue;
using std::cout;
using std::ostream;
using std::swap;
using std::string;
using std::unordered_map;

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
//const array<char, FieldSize> FinishField({ '1', '2', '3', '4', '5', '6', '7', '8', '0' });
const array<int, FieldSize> FinishField({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 });

class GameState {
public:
	GameState(const array<int, FieldSize>& _field);

	bool CanMoveLeft() const;
	bool CanMoveUp() const;
	bool CanMoveRight() const;
	bool CanMoveDown() const;

	GameState MoveLeft() const;
	GameState MoveUp() const;
	GameState MoveRight() const;
	GameState MoveDown() const;

	bool IsFinish() const;

	char GetFieldValue(size_t pos) const;

	bool operator == (const GameState& state) const { return field == state.field; }
	bool operator != (const GameState& state) const { return !(*this == state); }

private:
	array<int, FieldSize> field;
	size_t zeroPos;
};

GameState::GameState(const array<int, FieldSize>& _field) :
	field(_field),
	zeroPos(-1)
{
	for (size_t i = 0; i < field.size(); ++i) {
		if (field[i] == 0) {
			zeroPos = i;
			break;
		}
	}
	assert(zeroPos != -1);
}

bool GameState::CanMoveLeft() const
{
	return zeroPos % SideSize != 0;
}

bool GameState::CanMoveUp() const
{
	return zeroPos >= SideSize;
}

bool GameState::CanMoveRight() const
{
	return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveDown() const
{
	return zeroPos < FieldSize - SideSize;
}

GameState GameState::MoveLeft() const
{
	assert(CanMoveLeft());
	GameState newState(*this);
	swap(newState.field[zeroPos], newState.field[zeroPos - 1]);
	--newState.zeroPos;
	return newState;
}

GameState GameState::MoveUp() const
{
	assert(CanMoveUp());
	GameState newState(*this);
	swap(newState.field[zeroPos], newState.field[zeroPos - SideSize]);
	newState.zeroPos -= SideSize;
	return newState;
}

GameState GameState::MoveRight() const
{
	assert(CanMoveRight());
	GameState newState(*this);
	swap(newState.field[zeroPos], newState.field[zeroPos + 1]);
	++newState.zeroPos;
	return newState;
}

GameState GameState::MoveDown() const
{
	assert(CanMoveDown());
	GameState newState(*this);
	swap(newState.field[zeroPos], newState.field[zeroPos + SideSize]);
	newState.zeroPos += SideSize;
	return newState;
}

bool GameState::IsFinish() const
{
	return field == FinishField;
}

char GameState::GetFieldValue(size_t pos) const
{
	assert(pos >= 0 && pos < FieldSize);
	return field[pos];
}

struct StateHasher {
	size_t operator()(const GameState& state) const {
		size_t hash = 0;
		size_t p = 1;
		for (int i = 0; i < FieldSize; ++i) {
			p *= 137;
			hash += state.GetFieldValue(i) * p;
		}
		return hash;
	}
};

std::vector<char> Get8thSolution(const GameState& state)
{
	queue<GameState> bfsQueue;
	bfsQueue.push(state);
	unordered_map<GameState, char, StateHasher> visited;
	visited[state] = 'S';
	while (bfsQueue.size() > 0) {
		GameState current = bfsQueue.front();
		bfsQueue.pop();
		if (current.IsFinish()) {
			break;
		}
		if (current.CanMoveLeft()) {
			GameState newState = current.MoveLeft();
			if (visited.find(newState) == visited.end()) {
				visited[newState] = 'R';
				bfsQueue.push(newState);
			}
		}
		if (current.CanMoveUp()) {
			GameState newState = current.MoveUp();
			if (visited.find(newState) == visited.end()) {
				visited[newState] = 'D';
				bfsQueue.push(newState);
			}
		}
		if (current.CanMoveRight()) {
			GameState newState = current.MoveRight();
			if (visited.find(newState) == visited.end()) {
				visited[newState] = 'L';
				bfsQueue.push(newState);
			}
		}
		if (current.CanMoveDown()) {
			GameState newState = current.MoveDown();
			if (visited.find(newState) == visited.end()) {
				visited[newState] = 'U';
				bfsQueue.push(newState);
			}
		}
	}

	std::vector<char> moves;
	GameState current(FinishField);
	char move = visited[current];
	while (move != 'S') {
		moves.push_back(move);
		switch (move) {
		case 'L':
			current = current.MoveLeft();
			break;
		case 'U':
			current = current.MoveUp();
			break;
		case 'R':
			current = current.MoveRight();
			break;
		case 'D':
			current = current.MoveDown();
			break;
		}
		move = visited[current];
	}

	return moves;
}

void run(std::istream& input, std::ostream& output) {

	std::array<int, FieldSize> CurrentField;
	for (size_t i = 0; i < FieldSize; i++) {
		char field;
		//input >> field;
		//CurrentField[i] = static_cast<int>(field);
		input >> CurrentField[i];
	}
	GameState state(CurrentField);
	std::vector<char> moves = Get8thSolution(state);

	output << moves.size() << std::endl;
	for (char move : moves) {
		output << move;
	}

}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 2 3 0 5 6 7 4 9 10 11 8 13 14 15 12\n";
        run(input, output);
        assert(output.str() == "3\nUUU");
    }
	{
		std::stringstream input;
		std::stringstream output;
		input << "1 2 3 4\n5 6 7 8\n9 10 11 0\n13 14 15 12\n";
		run(input, output);
		assert(output.str() == "1\nU");
	}
}

int main()
{
    test();
    run(std::cin, std::cout);
    return 0;
}