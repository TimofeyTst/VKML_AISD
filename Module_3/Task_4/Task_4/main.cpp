#include <iostream>
#include <cassert>
#include <sstream>

#include <vector>
#include <array>
#include <queue>
#include <string>
#include <unordered_map>
#include <algorithm>

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


struct AStarState {
	GameState state;
	int fScore;

	AStarState(const GameState& _state, int _fScore) :
		state(_state),
		fScore(_fScore)
	{}
};

struct AStarStateComparator {
	bool operator()(const AStarState& state1, const AStarState& state2) const {
		return state1.fScore > state2.fScore;
	}
};

int CalculateManhattanDistance(const GameState& state)
{
	int distance = 0;
	for (int i = 0; i < FieldSize; ++i) {
		int value = state.GetFieldValue(i);
		if (value != 0) {
			int targetX = (value - 1) % SideSize;
			int targetY = (value - 1) / SideSize;
			int currentX = i % SideSize;
			int currentY = i / SideSize;
			distance += abs(targetX - currentX) + abs(targetY - currentY);
		}
	}
	return distance;
}

int CalculateFScore(const GameState& state)
{
	int hScore = CalculateManhattanDistance(state);
	return hScore;
}

std::vector<char> ReconstructPath(const unordered_map<GameState, char, StateHasher>& cameFrom, const GameState& current)
{
	std::vector<char> path;
	GameState state = current;
	while (cameFrom.find(state) != cameFrom.end()) {
		char move = cameFrom.at(state);
		path.push_back(move);
		switch (move) {
		case 'L':
			state = state.MoveLeft();
			break;
		case 'U':
			state = state.MoveUp();
			break;
		case 'R':
			state = state.MoveRight();
			break;
		case 'D':
			state = state.MoveDown();
			break;
		}
	}
	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<char> Get15thSolution(const GameState& state)
{
	std::priority_queue<AStarState, std::vector<AStarState>, AStarStateComparator> openSet;
	unordered_map<GameState, char, StateHasher> cameFrom;
	unordered_map<GameState, int, StateHasher> fScore;

	fScore[state] = CalculateFScore(state);

	openSet.push(AStarState(state, fScore[state]));
	while (!openSet.empty()) {
		GameState current = openSet.top().state;
		openSet.pop();

		if (current.IsFinish()) {
			return ReconstructPath(cameFrom, current);
		}

		if (current.CanMoveLeft()) {
			GameState newState = current.MoveLeft();
			int newFScore = CalculateFScore(newState);
			if (fScore.find(newState) == fScore.end() || newFScore < fScore[newState]) {
				cameFrom[newState] = 'R';
				fScore[newState] = newFScore;
				openSet.push(AStarState(newState, fScore[newState]));
			}
		}

		if (current.CanMoveUp()) {
			GameState newState = current.MoveUp();
			int newFScore = CalculateFScore(newState);
			if (fScore.find(newState) == fScore.end() || newFScore < fScore[newState]) {
				cameFrom[newState] = 'D';
				fScore[newState] = newFScore;
				openSet.push(AStarState(newState, fScore[newState]));
			}
		}

		if (current.CanMoveRight()) {
			GameState newState = current.MoveRight();
			int newFScore = CalculateFScore(newState);
			if (fScore.find(newState) == fScore.end() || newFScore < fScore[newState]) {
				cameFrom[newState] = 'L';
				fScore[newState] = newFScore;
				openSet.push(AStarState(newState, fScore[newState]));
			}
		}

		if (current.CanMoveDown()) {
			GameState newState = current.MoveDown();
			int newFScore = CalculateFScore(newState);
			if (fScore.find(newState) == fScore.end() || newFScore < fScore[newState]) {
				cameFrom[newState] = 'U';
				fScore[newState] = newFScore;
				openSet.push(AStarState(newState, fScore[newState]));
			}
		}
	}

	return std::vector<char>();  // No solution found
}

void run(std::istream& input, std::ostream& output) {

	std::array<int, FieldSize> CurrentField;
	for (size_t i = 0; i < FieldSize; i++) {
		char field;
		input >> CurrentField[i];
	}
	GameState state(CurrentField);
	std::vector<char> moves = Get15thSolution(state);

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
	{
		std::stringstream input;
		std::stringstream output;
		input << "8 15 9 0 14 6 5 3 4 12 11 2 10 13 1 7";  // "57\nUU RUR DDD L UUU R D R DD LL UL UU RD RD RD LL UU RU LD LD RR UR U L D R DD LLL U R U L U"
		run(input, output);
		assert(output.str() == "57\nUURURDDDLUUURDRDDLLULUURDRDRDLLUURULDLDRRURULDRDDLLLURULU");

	}
}

int main()
{
    //test();
    run(std::cin, std::cout);
    return 0;
}