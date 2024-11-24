#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
using namespace std::chrono;

class ArrayGenerator {
public:
    vector<int> generateRandomArray(int size, int minValue, int maxValue) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(minValue, maxValue);

        vector<int> arr(size);
        generate(arr.begin(), arr.end(), [&]() { return distrib(gen); });
        return arr;
    }

    vector<int> generateReverseSortedArray(int size) {
        vector<int> arr(size);
        generate(arr.begin(), arr.end(), [&, i = size]() mutable { return i--; });
        return arr;
    }

    vector<int> generateAlmostSortedArray(int size, int numSwaps) {
        vector<int> arr(size);
        generate(arr.begin(), arr.end(), [&, i = 1]() mutable { return i++; });

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, size - 1);

        for (int i = 0; i < numSwaps; ++i) {
            swap(arr[distrib(gen)], arr[distrib(gen)]);
        }
        return arr;
    }
};

class SortTester {
public:

    long long measureQuickSort(vector<int>& arr) {
        vector<int> temp = arr;
        auto start = high_resolution_clock::now();
        quickSort(temp, 0, temp.size() - 1);
        auto elapsed = high_resolution_clock::now() - start;
        return duration_cast<microseconds>(elapsed).count();
    }


    long long measureIntroSort(vector<int>& arr) {
        vector<int> temp = arr;
        auto start = high_resolution_clock::now();
        introSort(temp);
        auto elapsed = high_resolution_clock::now() - start;
        return duration_cast<microseconds>(elapsed).count();
    }



private:

    void insertionSort(vector<int>& a, int l, int r) {
        for (int i = l + 1; i <= r; ++i) {
            int key = a[i];
            int j = i - 1;
            while (j >= l && a[j] > key) {
                a[j + 1] = a[j];
                j--;
            }
            a[j + 1] = key;
        }
    }

    int partition(vector<int>& a, int l, int r) {
        random_device rd;
        mt19937 g(rd());
        uniform_int_distribution<> distrib(l, r);
        swap(a[r], a[distrib(g)]); 


        int pivot = a[r];
        int i = l - 1;
        for (int j = l; j < r; ++j) {
            if (a[j] <= pivot) {
                i++;
                swap(a[i], a[j]);
            }
        }
        swap(a[i + 1], a[r]);
        return i + 1;
    }

    void heapSort(vector<int>& a, int l, int r) {
        make_heap(a.begin() + l, a.begin() + r + 1);
        sort_heap(a.begin() + l, a.begin() + r + 1);
    }

    void quickSort(vector<int>& a, int l, int r) {
        if (l < r) {
            int p = partition(a, l, r);
            quickSort(a, l, p - 1);
            quickSort(a, p + 1, r);
        }
    }


    void introSortImpl(vector<int>& a, int l, int r, int depth_limit) {
        if (r - l + 1 < 16) {
            insertionSort(a, l, r);
            return;
        }
        if (depth_limit == 0) {
            heapSort(a, l, r);
            return;
        }

        int p = partition(a, l, r);
        introSortImpl(a, l, p - 1, depth_limit - 1);
        introSortImpl(a, p + 1, r, depth_limit - 1);
    }

    void introSort(vector<int>& a) {
        int n = a.size();
        int depth_limit = 2 * log2(n);
        introSortImpl(a, 0, n - 1, depth_limit);
    }


};


int main() {

    ArrayGenerator generator;
    SortTester tester;

    const int MIN_SIZE = 500;
    const int MAX_SIZE = 10000;
    const int STEP = 100;
    const int MIN_VALUE = 0;
    const int MAX_VALUE = 6000;
    const int NUM_SWAPS = 100;
    const int NUM_TRIALS = 5;

    ofstream randomData("../data/random_data.csv");
    ofstream reverseData("../data/reverse_data.csv");
    ofstream almostData("../data/almost_data.csv");

    randomData << "Size,QuickSort,IntroSort" << endl;
    reverseData << "Size,QuickSort,IntroSort" << endl;
    almostData << "Size,QuickSort,IntroSort" << endl;

    for (int size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
        vector<int> randomArray = generator.generateRandomArray(size, MIN_VALUE, MAX_VALUE);
        vector<int> reverseArray = generator.generateReverseSortedArray(size);
        vector<int> almostSortedArray = generator.generateAlmostSortedArray(size, NUM_SWAPS);

        long long totalQuickSortRandom = 0;
        long long totalQuickSortReverse = 0;
        long long totalQuickSortAlmost = 0;

         long long totalIntroSortRandom = 0;
        long long totalIntroSortReverse = 0;
        long long totalIntroSortAlmost = 0;


        for (int trial = 0; trial < NUM_TRIALS; ++trial) {
            totalQuickSortRandom += tester.measureQuickSort(randomArray);
            totalQuickSortReverse += tester.measureQuickSort(reverseArray);
            totalQuickSortAlmost += tester.measureQuickSort(almostSortedArray);

             totalIntroSortRandom += tester.measureIntroSort(randomArray);
            totalIntroSortReverse += tester.measureIntroSort(reverseArray);
            totalIntroSortAlmost += tester.measureIntroSort(almostSortedArray);
        }

        randomData << size << "," << (double)totalQuickSortRandom / NUM_TRIALS << "," << (double)totalIntroSortRandom / NUM_TRIALS << endl;
        reverseData << size << "," << (double)totalQuickSortReverse / NUM_TRIALS << "," << (double)totalIntroSortReverse / NUM_TRIALS << endl;
        almostData << size << "," << (double)totalQuickSortAlmost / NUM_TRIALS << "," << (double)totalIntroSortAlmost / NUM_TRIALS <<  endl;

        if (size == MAX_SIZE) {
            cout << (double)totalQuickSortRandom / NUM_TRIALS << endl;
            cout << (double)totalQuickSortReverse / NUM_TRIALS << endl;
            cout << (double)totalQuickSortAlmost / NUM_TRIALS << endl;
        }
    }

    randomData.close();
    reverseData.close();
    almostData.close();


    return 0;
}