#include <iostream>
#include <sstream>
#include <cassert>


template<typename T>
class Deque {
public:
    Deque() : buffer_size(2), head(0), tail(1), capacity(0), buffer(new T[2]) {}

    bool empty() const {
        return capacity == 0;
    }

    int size() const {
        return capacity;
    }

    void push_front(T value) {
        if (buffer_size == capacity) {
            resize(buffer_size * 2); // увеличение вместимости буфера
        }
        head = (head - 1 + buffer_size) % buffer_size; // вычисление нового индекса первого элемента
        buffer[head] = value;
        ++capacity;
    }

    void push_back(T value) {
        if (buffer_size == capacity) {
            resize(buffer_size * 2); // увеличение вместимости буфера
        }
        tail = (tail + 1) % buffer_size; // вычисление нового индекса последнего элемента
        buffer[tail] = value;
        ++capacity;
    }

    T pop_front() {
        if (empty()) {
            throw std::out_of_range("deque is empty");
        }
        T value = buffer[head];
        head = (head + 1) % buffer_size; // вычисление нового индекса первого элемента
        --capacity;
        if (capacity < buffer_size / 4) {
            resize(buffer_size / 2); // уменьшение вместимости буфера
        }
        return value;
    }


    T pop_back() {
        if (empty()) {
            throw std::out_of_range("deque is empty");
        }
        T value = buffer[tail];
        tail = (tail - 1 + buffer_size) % buffer_size; // вычисление нового индекса
        --capacity;
        if (capacity < buffer_size / 4) {
            resize(buffer_size / 2); // уменьшение вместимости буфера
        }
        return value;
    }

    void show() {
        for (int i = head; i != tail; i = (i + 1) % buffer_size) {
            std::cout << buffer[i] << " "; // копирование элементов в новый буфер
        }
        std::cout << buffer[tail];
        std::cout << "\nSize = " << buffer_size << "\tHead: " << head << "\tTail: " << tail <<
            "\tCapacity: " << capacity << "\n";
    }


    ~Deque() {
        delete[] buffer;
    }

private:
    void resize(int new_size) {
        T* new_buffer = new T[new_size]; // выделение нового буфера
        int j = 0;
        for (int i = head; i != tail; i = (i + 1) % buffer_size) {
            new_buffer[j] = buffer[i]; // копирование элементов в новый буфер
            j++;
        }
        new_buffer[j] = buffer[tail]; // копирование последнего элемента
        head = 0; // обновление индексов
        tail = j;
        buffer_size = new_size;
        delete[] buffer; // освобождение старого буфера
        buffer = new_buffer; // замена указателя на буфер
    }

    T* buffer; // указатель на буфер с элементами дека
    int buffer_size; // текущая вместимость буфера
    int head; // индекс первого элемента дека
    int tail; // индекс последнего элемента дека
    int capacity; // количество элементов в деке
};


void run(std::istream& input, std::ostream& output)
{
    Deque<int> dq;
    int n = 0;
    input >> n;
    bool result = true;
    for (int i = 0; i < n; ++i) {
        int command = 0;
        int data = 0;
        input >> command >> data;
        switch (command) {
        case 1:
            dq.push_front(data);
            break;
        case 2:
            if (dq.empty()) {
                result = result && data == -1;
            }
            else {
                result = result && dq.pop_front() == data;
            }
            break;
        case 3:
            dq.push_back(data);
            break;
        case 4:
            if (dq.empty()) {
                result = result && data == -1;
            }
            else {
                result = result && dq.pop_back() == data;
            }
            break;
        }
    }

    output << (result ? "YES" : "NO");
}

void testQueue()
{
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 44 3 50 2 44";
        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 2 -1 1 10";
        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 4 66";
        run(input, output);
        assert(output.str() == "NO");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 4 44 4 -1";
        run(input, output);
        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 2 -1 4 15";
        run(input, output);
        assert(output.str() == "NO");
    }
}


int main() {
    //testQueue();
    run(std::cin, std::cout);

    return 0;
}
