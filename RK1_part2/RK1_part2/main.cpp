#include <iostream>
#include <stack>
#include <string>
#include <cassert>
#include <sstream>


bool isStackAnagram(std::string s1, std::string s2) {
    std::stack<char> st;
    int i = 0;
    for (char c : s2) {
        // Пока буквы не совпадают мы помещаем в стек
        while (st.empty() || st.top() != c) {
            if (i == s1.size()) {
                return false;
            }
            st.push(s1[i++]);
        }
        st.pop();
    }
    return st.empty();
}


void run(std::istream& input, std::ostream& output) {
    std::string s1, s2;
    input >> s1 >> s2;
    if (isStackAnagram(s1, s2)) {
        output << "YES\n";
    }
    else {
        output << "NO\n";
    }
}


void testKth() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "STOL SLOT";
        run(input, output);
        assert(output.str() == "YES\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "STOL SLOTADD";
        run(input, output);
        assert(output.str() == "YES\n");
    }
}


int main() {
    testKth();
    //run(std::cin, std::cout);

    return 0;
}
