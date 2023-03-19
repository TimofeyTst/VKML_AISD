#include <iostream>
#include <sstream>
#include <cassert>

// 2_1.Дан отсортированный массив целых чисел A[0..n - 1] и массив целых чисел B[0..m - 1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
// равного или превосходящего B[i]: A[k] >= B[i].Если такого элемента нет, выведите n.n, m ≤ 10000.
// Требования : Время работы поиска k для каждого элемента B[i] : O(log(k)).
// Внимание!В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска 
// размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
// Формат входных данных.
// В первой строчке записаны числа n и m.Во второй и третьей массивы A и B соответственно.

int min(int x, int y) {
	return (x < y) ? x : y;
}


int binarySearch(const int* arr, const int& size, const int& biggerThan, int left, int right) {
	assert(left <= right);
	// Если массив не содержал элемента больше указанного, возвращаем size
	if (arr[right] >= biggerThan) {
		int min = right;
		while (left <= right) {
			int mid = left + (right - left) / 2;
			if (arr[mid] >= biggerThan) {
				if (arr[mid] <= arr[min]) {
					min = mid;
				}
				right -= 1;
			}
			else {
				left = mid + 1;
			}
		}
		return min;
	}
	else {
		return size;
	}

}


int exponentialSearch(const int* arr, const int& size, const int& biggerThan) {
	assert(size > 0);
	
	int power = 1;
	while (power < size && arr[power] < biggerThan) {
		power *= 2;
	}

	return binarySearch(arr, size, biggerThan, power/2, min(power, size-1));
}


void run(std::istream& input, std::ostream& output) {
	int n, m;
	input >> n >> m;
	int* A = new int[n], *B = new int[m];
	for (size_t i = 0; i < n; i++){
		input >> A[i];
	}
	for (size_t i = 0; i < m; i++) {
		input >> B[i];
	}

	for (size_t i = 0; i < m; i++){
		output << exponentialSearch(A, n, B[i]) << " ";
	}

	delete[] A;
	delete[] B;
}


void testCases() {
	{
		std::stringstream input;
		std::stringstream output;
		input << "4 3\n2 4 5 7\n4 6 1";
		run(input, output);
		assert(output.str() == "1 3 0 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "2 1\n1 2\n2";
		run(input, output);
		assert(output.str() == "1 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "5 2\n1 2 3 4 5\n9 5";
		run(input, output);
		assert(output.str() == "5 4 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "3 3\n10 20 30\n9 15 35";
		run(input, output);
		assert(output.str() == "0 1 3 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "3 4\n10 20 30\n8 9 10 32";
		run(input, output);
		assert(output.str() == "0 0 0 3 ");
	}
	{
		std::stringstream input;
		std::stringstream output;
		input << "3 6\n10 20 20\n8 9 10 12 12 32";
		run(input, output);
		assert(output.str() == "0 0 0 1 1 3 ");
	}
}

int main(){
	//testCases();
	run(std::cin, std::cout);

	return 0;
}