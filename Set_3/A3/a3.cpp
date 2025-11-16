#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll MAX = 1e5;

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}


void buildMaxHeap(vector<int>& arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}

// Heap Sort для подмассива [left, right]
void heapSort(vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    vector<int> temp(n);

    // Копируем подмассив во временный массив
    for (int i = 0; i < n; i++) {
        temp[i] = arr[left + i];
    }

    // Сортируем временный массив
    buildMaxHeap(temp, n);
    for (int i = n - 1; i > 0; i--) {
        swap(temp[0], temp[i]);
        heapify(temp, i, 0);
    }

    // Копируем обратно
    for (int i = 0; i < n; i++) {
        arr[left + i] = temp[i];
    }
}

// Insertion Sort для подмассива [left, right]
void insertionSort(vector<int>& array, int left, int right) {
    if (left >= right || left < 0 || right >= array.size()) {
        return;
    }
    for (int i = left + 1; i <= right; ++i) {
        int cur = array[i];
        int j = i - 1;
        while (j >= left && array[j] > cur) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = cur;
    }
}

// Partition для Quick Sort
int partition(vector<int>& arr, int left, int right) {
    int randomIndex = left + rand() % (right - left + 1);
    swap(arr[randomIndex], arr[right]);

    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1;
}

// Quick Sort с ограничением глубины рекурсии
void quickSort(vector<int>& arr, int left, int right, int depthLimit) {
    if (left >= right) return;

    int n = right - left + 1;

    // Переключение на Insertion Sort
    if (n < 16) {
        insertionSort(arr, left, right);
        return;
    }

    // Переключение на Heap Sort
    if (depthLimit <= 0) {
        heapSort(arr, left, right);
        return;
    }

    // Обычный Quick Sort
    int pivotIndex = partition(arr, left, right);

    // Рекурсивная сортировка левой и правой частей
    quickSort(arr, left, pivotIndex - 1, depthLimit - 1);
    quickSort(arr, pivotIndex + 1, right, depthLimit - 1);
}

// Основная функция Introsort
void introsort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int n = right - left + 1;
    int depthLimit = 2 * log2(n);

    quickSort(arr, left, right, depthLimit);
}

// Стандартный Quick Sort
void quickSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivotIndex = partition(arr, left, right);
    quickSort(arr, left, pivotIndex - 1);
    quickSort(arr, pivotIndex + 1, right);
}

class ArrayGenerator {
private:
    vector<int> base_array_;
    int max_length_;
    int min_range_;
    int max_range_;
    mt19937 gen_;

    void GenerateBaseArray() {
        uniform_int_distribution<int> dist(min_range_, max_range_);
        for (int i = 0; i < max_length_; ++i) {
            base_array_.push_back(dist(gen_));
        }
    }

public:
    ArrayGenerator(int max_length = MAX, int min_range = 0, int max_range = 10000)
            : max_length_(max_length), min_range_(min_range), max_range_(max_range), gen_(228) {
        GenerateBaseArray();
    }

    vector<int> GetRandomArray(int size) const {
        size = min(size, max_length_);
        return vector<int>(base_array_.begin(), base_array_.begin() + size);
    }

    vector<int> GetReversedArray(int size) const {
        size = min(size, max_length_);
        vector<int> reversed_array(base_array_.begin(), base_array_.begin() + size);
        sort(reversed_array.rbegin(), reversed_array.rend());
        return reversed_array;
    }

    vector<int> GetNearlySortedArray(int size, int swap_count) const {
        size = min(size, max_length_);
        vector<int> nearly_sorted_array(base_array_.begin(), base_array_.begin() + size);
        sort(nearly_sorted_array.begin(), nearly_sorted_array.end());
        uniform_int_distribution<int> dist(0, size - 1);
        mt19937 local_gen = gen_;
        for (int i = 0; i < swap_count; ++i) {
            swap(nearly_sorted_array[dist(local_gen)], nearly_sorted_array[dist(local_gen)]);
        }
        return nearly_sorted_array;
    }
};

class SortTester {
private:
    const int num_trials_ = 5;
public:
    double MeasureQuickSortTime(const vector<int>& original_array) {
        ll total_time = 0;

        for (int trial = 0; trial < num_trials_; ++trial) {
            vector<int> array_copy = original_array;
            auto start = chrono::high_resolution_clock::now();
            quickSort(array_copy, 0, array_copy.size() - 1);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            total_time += duration.count();
        }
        return static_cast<double>(total_time) / num_trials_;
    }

    double MeasureHybridSortTime(const vector<int>& original_array) {
        ll total_time = 0;
        for (int trial = 0; trial < num_trials_; ++trial) {
            vector<int> array_copy = original_array;
            auto start = chrono::high_resolution_clock::now();
            introsort(array_copy, 0, array_copy.size() - 1);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            total_time += duration.count();
        }
        return static_cast<double>(total_time) / num_trials_;
    }
};


signed main() {
    ArrayGenerator generator(MAX, 0, 10000);
    SortTester tester;

    ofstream out_file1("C:\\Users\\79655\\CLionProjects\\hse\\quick_random.txt");
    ofstream out_file2("C:\\Users\\79655\\CLionProjects\\hse\\quick_reversed.txt");
    ofstream out_file3("C:\\Users\\79655\\CLionProjects\\hse\\quick_nearly_sorted.txt");
    ofstream out_file4("C:\\Users\\79655\\CLionProjects\\hse\\hybrid_random.txt");
    ofstream out_file5("C:\\Users\\79655\\CLionProjects\\hse\\hybrid_reversed.txt");
    ofstream out_file6("C:\\Users\\79655\\CLionProjects\\hse\\hybrid_nearly_sorted.txt");

    for (int size = 500; size <= 1e4; size += 100) {
        vector<int> random_array = generator.GetRandomArray(size);
        vector<int> reversed_array = generator.GetReversedArray(size);
        vector<int> nearly_sorted_array = generator.GetNearlySortedArray(size, size / 20);

        double random_time = tester.MeasureQuickSortTime(random_array);
        double reversed_time = tester.MeasureQuickSortTime(reversed_array);
        double nearly_sorted_time = tester.MeasureQuickSortTime(nearly_sorted_array);

        out_file1 << size << ' ' << random_time << '\n';
        out_file2 << size << ' ' << reversed_time << '\n';
        out_file3 << size << ' ' << nearly_sorted_time << '\n';

        random_time = tester.MeasureHybridSortTime(random_array);
        reversed_time = tester.MeasureHybridSortTime(reversed_array);
        nearly_sorted_time = tester.MeasureHybridSortTime(nearly_sorted_array);

        out_file4 << size << ' ' << random_time << '\n';
        out_file5 << size << ' ' << reversed_time << '\n';
        out_file6 << size << ' ' << nearly_sorted_time << '\n';
    }
    cout << "OK";
    return 0;
}