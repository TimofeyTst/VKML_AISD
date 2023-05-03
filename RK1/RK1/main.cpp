#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>
#include <sstream>


template<class T>
class CmpDefault {
public:
    bool operator()(const T& l, const T& r) { return l <= r; }
};

template<class T, class Compare = CmpDefault<T>>
int partition(T arr[], int l, int r, Compare cmp = CmpDefault<T>()) {
    T x = arr[r];
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
        if (arr[pos] < k && arr[pos+1] > k) {
            return pos + 1;
        }
        if (arr[pos] == k) {
            return pos;
        }
        if (arr[pos] < k) {
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
    int k = 0;
    input >> n;
    int* arr = new int[n];
    for (size_t i = 0; i < n; ++i) {
        input >> arr[i];
    }
    input >> k;
    int result = kthSmallest(arr, 0, n - 1, k);
    if (result == -1) {
        // Если за пределами массива
        if (k > arr[n-1]) {
            output << n << "\n";
        }
        else {
            output << 0 << "\n";
        }
    }
    else {
        output << result << "\n";
    }
    
    delete[] arr;
}


void testKth() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n1 3 5 6\n5";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n1 3 5 6\n2";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n1 3 5 6\n9";
        run(input, output);
        assert(output.str() == "4\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n1 3 5 6\n-12";
        run(input, output);
        assert(output.str() == "0\n");
    }
    //{
    //    std::stringstream input;
    //    std::stringstream output;
    //    input << "4\n1 3 5 6\n9";
    //    run(input, output);
    //    assert(output.str() == "4\n");
    //}
}

int main() {
    //testKth();
    run(std::cin, std::cout);

    return 0;
}