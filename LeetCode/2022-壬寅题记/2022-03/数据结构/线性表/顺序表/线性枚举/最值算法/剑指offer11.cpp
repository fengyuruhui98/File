int minArray(int* numbers, int numbersSize){
    int low = 0, high = numbersSize - 1, pivote;
    while(low < high){
        pivote = low + (low + high)/2;
        if(numbers[pivote] < numbers[high]) pivote = high;
        else if(numbers[pivote] > numbers[low]) pivote = low + 1;
        else high--;
    }
    return numbers[pivote];                 //return numbers[low];
}