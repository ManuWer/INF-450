#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector<int> values(1000000,0);

void quickSort(int begin, int end) {
    int i = begin, j = end-1, aux;
    int pivot = values[(begin + end) / 2];

    while(i<=j) {

        while(values[i] < pivot && i < end)
            i++;

        while(values[j] > pivot && j > begin)
            j--;

        if(i<=j) {
            aux = values[i];
            values[i] = values[j];
            values[j] = aux;
            i++;
            j--;
        }
    }

    if(j>begin)
        quickSort(begin, j+1);
    if(i<end)
        quickSort(i, end);
}

int main() {

    int n;
    cin >> n;
    for(int i=0;i<n;i++) cin >> values[i];

    quickSort(0,n-1);

    //for(int i=0;i<n;i++) 
      //  cout << values[i] << " ";

    return 0;
}

