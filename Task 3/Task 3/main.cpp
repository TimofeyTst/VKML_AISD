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
        return value;
    }


    T pop_back() {
        if (empty()) {
            throw std::out_of_range("deque is empty");
        }
        T value = buffer[tail];
        tail = (tail - 1 + buffer_size) % buffer_size; // вычисление нового индекса
        --capacity;
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

void testDeque()
{
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 44 3 50 2 44";
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
        input << "3 3 44 4 44 4 -1";
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

    {
        std::stringstream input;
        std::stringstream output;
        input << "3" << std::endl;
        input << "3 44\n";
        input << "3 50\n";
        input << "2 44\n";
        //input << "3 3 44 3 50 2 44";
        run(input, output);

        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2" << std::endl;
        input << "2 -1\n";
        input << "1 10\n";
        run(input, output);

        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2" << std::endl;
        input << "3 44\n";
        input << "4 66\n";
        run(input, output);

        assert(output.str() == "NO");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4" << std::endl;
        input << "1 1\n";
        input << "1 2\n";
        input << "2 2\n";
        input << "4 1\n";
        run(input, output);

        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4" << std::endl;
        input << "3 1\n";
        input << "3 2\n";
        input << "4 2\n";
        input << "2 1\n";
        run(input, output);

        assert(output.str() == "YES");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "50 1 166 4 166 3 400 3 484 1 441 4 484 3 288 2 441 2 400 4 288 1 268"; // 11
        input << "2 268 1 161 3 278 2 161 1 121 3 379 4 379 3 34 2 121 4 34 1 162 4 278\n"; // 23
        input << "4 162 1 291 4 291 3 315 1 67 2 67 2 315 3 110 2 110 1 3\n"; // 33
        input << "3 330 1 394 4 330 4 3 2 394 1 63 3 80 3 170 3 137 1 200 1 69 1 50 4 137 1 471 3 91 3 450 2 471\n"; // 50
        run(input, output);

        assert(output.str() == "YES");
    }
}

int main() {
    //testDeque();
    run(std::cin, std::cout);

    return 0;
}
