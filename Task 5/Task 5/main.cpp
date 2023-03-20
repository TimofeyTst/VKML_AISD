#include <iostream>

void merge(int* arr, int size, int from, int mid, int end) {
    int i = from, j = mid + 1, k = from;
    int* tmpArr = new int[size];

    while (i <= mid && j <= end) {
        if (arr[i] < arr[j]) {
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

void mergeSort(int* arr, size_t size) {
    for (size_t i = 1; i < size; i *= 2){
        for (size_t j = 0; j < size - 1; j += 2 * i){
            merge(arr, size, j, j + i - 1, std::min(j + 2 * i - 1, size - 1));
        }
    }
}

int main() {

    //int arr[9] = { 1,3,5,6,2,4,7,8,9 };
    int arr[9] = { 19, 18, 17, 16, 15, 14, 13, 12, 11 };
    //merge(arr, 9, 0, 3, 8);
    mergeSort(arr, 9);
    for (size_t i = 0; i < 9; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
	return 0;
}