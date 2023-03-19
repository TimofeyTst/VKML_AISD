#include <iostream>
#include <sstream>
#include <cassert>

struct User {
    int id = 0;
    size_t count = 0;
};

template<class T>
class CmpDefault {
public:
    bool operator()(const T& l, const T& r) { return l < r; };
};

class CmpUsers {
public:
    bool operator()(const User& u1, const User& u2) { return u1.count > u2.count; }
};


template<class T, class Compare = CmpDefault<T>>
class Heap {
public:
    Heap() : size_(0), capacity_(1), data_(new T[capacity_]) {}

    ~Heap() {
        delete[] data_;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return data_[0];
    }

    T pop() {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        T result = data_[0];
        data_[0] = data_[size_ - 1];
        size_--;
        sift_down(0);
        return result;
    }

    void push(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_] = value;
        sift_up(size_);
        size_++;
    }

private:
    size_t size_;
    size_t capacity_;
    T* data_;
    Compare cmp_;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; i++) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void sift_up(size_t index) {
        while (index > 0) {
            size_t parent_index = (index - 1) / 2;
            if ( cmp_(data_[index], data_[parent_index]) ) {
                std::swap(data_[index], data_[parent_index]);
                index = parent_index;
            }
            else {
                break;
            }
        }
    }

    void sift_down(size_t index) {
        while (true) {
            size_t left_child_index = 2 * index + 1;
            size_t right_child_index = 2 * index + 2;
            size_t new_child_index = index;
            if (left_child_index < size_ && cmp_(data_[left_child_index], data_[new_child_index]) ) {
                new_child_index = left_child_index;
            }
            if (right_child_index < size_ && cmp_(data_[right_child_index], data_[new_child_index]) ) {
                new_child_index = right_child_index;
            }
            if (new_child_index != index) {
                std::swap(data_[index], data_[new_child_index]);
                index = new_child_index;
            }
            else {
                break;
            }
        }
    }
};


void run(std::istream& input, std::ostream& output) {
    int TotalUsers = 0, ShowUsers = 0;
    Heap<User, CmpUsers> hp;

    input >> TotalUsers >> ShowUsers;
    for (size_t i = 0; i < TotalUsers; ++i) {
        User u1;
        input >> u1.id >> u1.count;
        hp.push(u1);
    }

    // Чтобы вывести в возрастающем порядке, добавим записи 
    // в массив с конца, а потом выведем его
    User* ShowArr = new User[ShowUsers];
    for (size_t i = ShowUsers; i > 0; --i) { ShowArr[i - 1] = hp.pop(); }
    for (size_t i = 0; i < ShowUsers; ++i) {
        output << ShowArr[i].id << " ";
    }
    delete[] ShowArr;
}


void testHeap()
{
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 3 100 36 80 3 1 5";
        run(input, output);
        assert(output.str() == "80 1 100 ");
    }
}

int main() {
    //testHeap();
    run(std::cin, std::cout);


    return 0;
}