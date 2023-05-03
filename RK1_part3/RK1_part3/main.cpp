#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <sstream>

using namespace std;

struct Box {
    int id;
    vector<int> size;

    bool operator<(const Box& other) const {
        for (int i = 0; i < size.size(); i++) {
            if (size[i] >= other.size[i]) {
                return false;
            }
        }
        return true;
    }
};

void insertionSort(vector<Box>& boxes) {
    for (int i = 1; i < boxes.size(); i++) {
        Box key = boxes[i];
        int j = i - 1;
        while (j >= 0 && key < boxes[j]) {
            boxes[j + 1] = boxes[j];
            j--;
        }
        boxes[j + 1] = key;
    }
}


void run(std::istream& input, std::ostream& output) {
    int n;
    input >> n;

    vector<Box> boxes(n);
    for (int i = 0; i < n; i++) {
        boxes[i].id = i;
        for (int j = 0; j < 3; j++) {
            int size;
            input >> size;
            boxes[i].size.push_back(size);
        }
        sort(boxes[i].size.begin(), boxes[i].size.end());
    }

    insertionSort(boxes);

    for (const auto& box : boxes) {
        output << box.id << " ";
    }
    output << endl;
}


void testKth() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2 3 5\n1 1 1\n10 4 10";
        run(input, output);
        assert(output.str() == "1 0 2 \n");
    }
}


int main() {
    //testKth();
    run(std::cin, std::cout);

    return 0;
}
