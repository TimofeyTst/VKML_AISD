#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>
#include <sstream>

std::random_device rd; // создание генератора случайных чисел
std::mt19937 gen(rd()); // инициализация генератора случайных чисел

template<class T>
class CmpDefault {
public:
    bool operator()(const T& l, const T& r) { return l <= r; }
};

template<class T, class Compare = CmpDefault<T>>
int partition(T arr[], int l, int r, Compare cmp = CmpDefault<T>()) {
    std::uniform_int_distribution<int> dis(l, r);
    int rand = dis(gen);
    T x = arr[rand];
    std::swap(arr[rand], arr[r]);
    int i = l - 1;
    for (int j = l; j < r; j++) {
        if (cmp(arr[j], x)) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[r]);
    return i + 1;
}

template<class T, class Compare = CmpDefault<T>>
int kthSmallest(T arr[], int l, int r, int k, Compare cmp = CmpDefault<T>()) {
    while (l <= r) {
        int pos = partition(arr, l, r, cmp);
        if (pos == k - 1) {
            return arr[pos];
        }
        if (pos < k - 1) {
            l = pos + 1;
        }
        else {
            r = pos - 1;
        }
    }
    return -1;
}


void run(std::istream& input, std::ostream& output) {
    size_t n = 0;
    input >> n;
    int* arr = new int[n];
    for (size_t i = 0; i < n; ++i) {
        input >> arr[i];
    }
    output << kthSmallest(arr, 0, n - 1, n * 0.1 + 1, [](const int& l, const int& r) { return l <= r; }) << "\n"
        << kthSmallest(arr, 0, n - 1, n * 0.5 + 1) << "\n"
        << kthSmallest(arr, 0, n - 1, n * 0.9 + 1);
    delete[] arr;
}


void testKth() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n1 2 3 4 5 6 7 8 9 10";
        run(input, output);
        assert(output.str() == "2\n6\n10");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10\n1 8 2 3 4 6 7 5 9 10";
        run(input, output);
        assert(output.str() == "2\n6\n10");
    }
}

int main() {
    testKth();
    run(std::cin, std::cout);

    return 0;
}