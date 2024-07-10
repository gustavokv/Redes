#include <iostream>

using namespace std; 

void swap(int *i, int *j){
    int aux = *i;

    *i = *j;
    *j = aux;
}
  
void bubbleSort(int arr[], int n) { 
    int i, j; 
    for (i = 0; i < n - 1; i++) 
        for (j = 0; j < n - i - 1; j++) 
            if (arr[j] > arr[j + 1]) 
                swap(arr[j], arr[j + 1]); 
} 

int main() {
    int array[] = {5, 4, 8, 9, 1, 15};

    bubbleSort(array, 6);

    for(int i = 0; i<6; i++)
        cout << array[i] << " ";

    return 0;
}