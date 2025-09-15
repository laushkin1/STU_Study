#include <iostream>
#include <vector>
#include <algorithm>

void bubbleSort(std::vector<int>& array);
void insertionSort(std::vector<int>& array);
void selectionSort(std::vector<int>& array);
void mergeSort(std::vector<int>& array);

void mergeSortRec(std::vector<int>& array, int left, int right);
void merge(std::vector<int>& array, int left, int middle, int right);

int main(){

    std::vector<int> array = {10, 9, 1, 2, 8, 3, 6, 4, 7, 5};
    
    // std::sort(array.begin(), array.end());
    mergeSort(array);

    for(int element : array){
        std::cout << element << ' ';
    }
    std::cout << std::endl;

    return 0;
}

// O(n^2)
void bubbleSort(std::vector<int>& array){
    for (int i = 0; i < array.size()-1; i++){
        for(int j = 0; j < array.size()-1-i; j++){
            if(array[j] > array[j+1]){
                std::swap(array[j], array[j+1]);
            }
        }
    }
}

// O(n^2)
void insertionSort(std::vector<int>& array){
    int key;
    int j;
    for(int i = 1; i < array.size(); i++){
        key = array[i];
        j = i-1;

        while(j >= 0 && key < array[j]){
            array[j+1] = array[j];
            j--;
        }
        array[j+1] = key;
    }
}

// O(n^2)
void selectionSort(std::vector<int>& array){
    int min_indx;
    for(int i = 0; i < array.size()-1; i++){
        min_indx = i;

        for(int j = i+1; j < array.size(); j++)
            if(array[min_indx] > array[j]) min_indx = j;

        std::swap(array[i], array[min_indx]);
    }
}

// O(n log n)
void mergeSort(std::vector<int>& array){
    if(!array.empty()){
        mergeSortRec(array, 0, array.size()-1);
    }
}


void mergeSortRec(std::vector<int>& array, int left, int right){
    if(right <= left) return;
    int middle = left + (right-left)/2;
    mergeSortRec(array, left, middle);
    mergeSortRec(array, middle+1, right);
    merge(array, left, middle, right);
}

void merge(std::vector<int>& array, int left, int middle, int right){
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<int> L(n1), R(n2);
    for(int i = 0; i < n1; i++) L[i] = array[left+i];
    for(int j = 0; j < n2; j++) R[j] = array[middle+1+j];

    int i = 0, j = 0, k = left;
    while(i < n1 && j < n2){
        if(L[i] <= R[j]) array[k++] = L[i++];
        else array[k++] = R[j++];
    }
    while(i < n1) array[k++] = L[i++];
    while(j < n2) array[k++] = R[j++];
}
