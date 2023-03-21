#include <iostream>
#include <sstream>


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
        return (l.year < r.year) || (l.year == r.year && l.month < r.month) || (l.year == r.year && l.month == r.month && l.day < r.day);
    }
};

void show(Date *arr, int size) {
    std::cout << "BirthDates: ";
    for (size_t i = 0; i < size; i++){
        if (arr[i].birthday) {
            std::cout << arr[i].day << " " << arr[i].month << " " << arr[i].year << "\t";
        }
    }
    std::cout << "\nDethDates: ";
    for (size_t i = 0; i < size; i++) {
        if (!arr[i].birthday) {
            std::cout << arr[i].day << " " << arr[i].month << " " << arr[i].year << "\t";
        }
    }
    std::cout << "\n";
}

size_t maxModerns(Date* sortArr, size_t size) {
    size_t count = 0, max = 0;
    for (size_t i = 0; i < size; ++i){
        if (sortArr[i].birthday) {
            ++count;
        }
        else {
            max = std::max(count, max);
            --count;
        }
    }
    return max;
}

void run(std::istream& input, std::ostream& output) {
    int n;
    input >> n;
    Date* Dates = new Date[2*n];
    // В массиве будем отличать дату смерти от даты рождения по полю birthday
    Date birth = {0, 0, 0, true}, death;
    int yearsOld = 0;
    
    for (size_t i = 0; i < 2 * n; ++i){
        input >> birth.day >> birth.month >> birth.year;
        input >> death.day >> death.month >> death.year;
        yearsOld = death.year - birth.year;
        // Добавляем в массив только если возраст больше или равен 18
        if (yearsOld >= 18) {
            // Сокращаем возраст до 80 лет
            if (yearsOld >= 80) {
                death.day = birth.day;
                death.month = birth.month;
                death.year = birth.year + 80;
            }
            // В массиве храним дату рождения начиная с возраста современника (от 18 лет)
            birth.year += 18;
            Dates[i++] = birth;
            Dates[i] = death;
        }
    }
    CmpDate cmp;
    show(Dates, 2 * n);

    mergeSort(Dates, 2 * n, cmp);

    show(Dates, 2 * n);
    std::cout << "Max Moderns: " << maxModerns(Dates, 2 * n) << "\n";

    delete[] Dates;
}

int main() {
    std::stringstream input;
    //input << "3\n2 5 1980 13 11 2055\n1 1 1982 1 1 2030\n2 1 1920 2 1 2000";
    input << "5\n10 10 1900 10 10 1990\n10 10 1915 10 10 1935\n10 10 1920 10 10 1945\n10 10 1930 10 10 1970\n10 10 1940 10 10 1990";
    run(input, std::cout);
    //int arr[9] = { 19, 18, 17, 16, 15, 14, 13, 12, 11 };
    //mergeSort(arr, 9);
    //for (size_t i = 0; i < 9; i++){
    //    std::cout << arr[i] << " ";
    //}
    //std::cout << "\n";
	return 0;
}