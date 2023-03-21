#include <iostream>
#include <sstream>
#include <cassert>


template<class T>
class CmpDefault {
public:
    bool operator()(const T& l, const T& r) { return l < r; }
};


template<class T, class Compare = CmpDefault<T>>
void merge(T* arr, size_t size, size_t from, size_t mid, size_t end, Compare cmp = CmpDefault<T>()) {
    size_t i = from, j = mid + 1, k = from;
    T* tmpArr = new T[size];

    while (i <= mid && j <= end) {
        if ( cmp(arr[i],arr[j]) ) {
            tmpArr[k++] = arr[i++];
        }
        else {
            tmpArr[k++] = arr[j++];
        }
    }

    while (i < size && i <= mid) {
        tmpArr[k++] = arr[i++];
    }

    while (j < size && j <= end) {
        tmpArr[k++] = arr[j++];
    }

    for (size_t i = from; i <= end; ++i){
        arr[i] = tmpArr[i];
    }

    delete[] tmpArr;
}


template<class T, class Compare = CmpDefault<T>>
void mergeSort(T* arr, size_t size, Compare cmp = CmpDefault<T>()) {
    for (size_t i = 1; i < size; i *= 2){
        for (size_t j = 0; j < size - 1; j += 2 * i){
            merge(arr, size, j, j + i - 1, std::min(j + 2 * i - 1, size - 1), cmp);
        }
    }
}


struct Date {
    int day = 0;
    int month = 0;
    int year = 0;
    bool birthday = false;
};


class CmpDate {
public:
    bool operator()(const Date& l, const Date& r) { 
        return (l.year < r.year) || (l.year == r.year && l.month < r.month) || (l.year == r.year && l.month == r.month && l.day < r.day)
            || (l.year == r.year && l.month == r.month && l.day == r.day && !l.birthday); // Дата становления современником должна быть после
    }
};


int maxModerns(Date* sortArr, size_t size) {
    int count = 0, max = 0;
    for (size_t i = 0; i < size; ++i){
        if (sortArr[i].birthday) {
            ++count;
            max = std::max(count, max);
        }
        else {
            count = std::max(count - 1, 0);
        }
    }
    return max;
}


void run(std::istream& input, std::ostream& output) {
    int n;
    input >> n;
    size_t size = 0;
    Date* Dates = new Date[2 * n]{};
    // В массиве будем отличать дату смерти от даты рождения по полю birthday
    Date birth = {0, 0, 0, true}, death;
    int yearsOld = 0;
    
    for (size_t i = 0; i < 2 * n; i += 2){
        input >> birth.day >> birth.month >> birth.year;
        input >> death.day >> death.month >> death.year;
        yearsOld = death.year - birth.year;
        bool isAdult = yearsOld > 18 || (yearsOld == 18 && (death.month > birth.month || (death.month == birth.month && death.day >= birth.day)));
        // Добавляем в массив только если возраст больше или равен 18
        if (isAdult) {
            // Сокращаем возраст до 80 лет
            if (yearsOld >= 80) {
                death.day = birth.day;
                death.month = birth.month;
                death.year = birth.year + 80;
            }
            // В массиве храним дату рождения начиная с возраста современника (от 18 лет)
            birth.year += 18;
            Dates[size++] = birth;
            Dates[size++] = death;
        }
    }
    CmpDate cmp;

    mergeSort(Dates, size, cmp);
    output << maxModerns(Dates, size) << "\n";

    delete[] Dates;
}


void testModerns() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2 5 1980 13 11 2055\n1 1 1982 1 1 2030\n2 1 1920 2 1 2000";
        run(input, output);
        assert(output.str() == "3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5\n10 10 1900 10 10 1990\n10 10 1915 10 10 1935\n10 10 1920 10 10 1945\n10 10 1930 10 10 1970\n10 10 1940 10 10 1990";
        run(input, output);
        assert(output.str() == "3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n10 10 1900 10 10 1990\n10 10 1915 10 10 1935\n10 10 1920 10 10 1945\n10 10 1930 10 10 1970";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1\n10 10 1900 10 10 1990";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1\n10 10 1900 10 10 1917";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1\n10 10 1900 10 10 1918";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1\n10 10 1900 11 10 1918";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "0";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2 5 1980 13 11 2055\n1 1 1982 1 1 2030\n2 1 1982 2 1 2000";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2 5 1980 13 11 2055\n1 1 1982 1 1 2030\n2 1 1982 3 1 2000";
        run(input, output);
        assert(output.str() == "3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2 5 1980 13 11 2055\n1 1 1982 1 1 2030\n2 1 1982 2 2 2000";
        run(input, output);
        assert(output.str() == "3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2 5 1988 13 11 2005\n1 1 1 1 1 10\n2 1 1910 1 1 1928";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7\n1 1 1980 1 1 2050\n1 1 1990 1 1 2040\n1 1 1950 1 1 2030\n1 1 2100 1 1 2120\n1 1 2101 1 1 2121\n1 1 2200 1 1 2232\n1 1 2201 1 1 2222";
        run(input, output);
        assert(output.str() == "3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n1 1 2100 1 1 2120\n1 1 2102 1 1 2121\n1 1 2099 1 1 2130\n1 1 2102 1 1 2192";
        run(input, output);
        assert(output.str() == "3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n1 1 2100 2 1 2120\n1 1 2102 1 1 2121\n1 1 2099 1 1 2130\n1 1 2102 1 1 2192";
        run(input, output);
        assert(output.str() == "4\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n1 1 2100 1 1 2118\n1 1 2100 1 1 2200";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1\n1 1 2000 1 1 2018";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n1 1 2000 1 1 2090\n1 1 2062 1 1 2120";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1000\n";
        for (int i = 1; i <= 1000; i++) {
            input << i << " 1 1990 1 1 2020\n";
        }
        run(input, output);
        assert(output.str() == "1000\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1000\n";
        for (int i = 1; i <= 1000; i++) {
            input << i << " 2 1990 1 1 2008\n";
        }
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1000\n1 1 1990 1 1 2020\n";
        for (int i = 2; i <= 1000; i++) {
            input << i << " 1 1990 1 1 2008\n";
        }
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1001\n";
        for (int i = 1; i <= 1000; i++) {
            input << i << " 2 1990 1 1 2008\n";
        }
        input << "1 1 1990 1 1 2020\n";
        run(input, output);
        assert(output.str() == "1\n");
    }
}

int main() {
    //testModerns();
    run(std::cin, std::cout);
	return 0;
}