#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll MAX = 1e5;

// Функция объединения двух отсортированных частей массива
void Merge(vector<int>& array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> left_arr(n1);
    vector<int> right_arr(n2);

    for (int i = 0; i < n1; i++) {
        left_arr[i] = array[left + i];
    }
    for (int j = 0; j < n2; j++) {
        right_arr[j] = array[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            array[k] = left_arr[i++];
        } else {
            array[k] = right_arr[j++];
        }
        k++;
    }

    while (i < n1) {
        array[k++] = left_arr[i++];
    }

    while (j < n2) {
        array[k++] = right_arr[j++];
    }
}

// Сортировка массива алгоритмом Merge Sort
void MergeSort(vector<int>& array, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = (left + right) / 2;
    MergeSort(array, left, mid);
    MergeSort(array, mid + 1, right);
    Merge(array, left, mid, right);
}

// Сортировка массива алгоритмом Insertion Sort
void InsertionSort(vector<int>& array, int left, int right) {
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
    /*for (int i = left + 1; i <= right; ++i) {
      int el = array[i];
      int j = i - 1;
      while (el < array[j]) {
        array[j + 1] = array[j];
        --j;
      }
      array[j + 1] = el;
    }*/
}

// Сортировка массива гибридным алгоритмом Merge + Insertion Sort
void HybridMergeSort(vector<int>& array, int left, int right, int threshold) {
    if (left >= right) {
        return;
    }

    if (right - left + 1 <= threshold) {
        InsertionSort(array, left, right);
        return;
    }

    int mid = left + (right - left) / 2;
    HybridMergeSort(array, left, mid, threshold);
    HybridMergeSort(array, mid + 1, right, threshold);
    Merge(array, left, mid, right);
}

// Класс случайного генератора массивов
class ArrayGenerator {
private:
    vector<int> base_array_;
    int max_length_;
    int min_range_;
    int max_range_;
    mt19937 gen_;

    // Генерирует рандомный массив максимальной длины со значениями в нужном диапазоне
    void GenerateBaseArray() {
        std::uniform_int_distribution<int> dist(min_range_, max_range_);
        for (int i = 0; i < max_length_; ++i) {
            base_array_.push_back(dist(gen_));
        }
    }

public:
    ArrayGenerator(int max_length = MAX, int min_range = 0, int max_range = 10000)
            : max_length_(max_length), min_range_(min_range), max_range_(max_range), gen_(228) {
        GenerateBaseArray();
    }

    // Выдает массив нужной длины
    vector<int> GetRandomArray(int size) const {
        size = min(size, max_length_);
        return vector<int> (base_array_.begin(), base_array_.begin() + size);
    }

    // Перевернутый массив нужной длины
    vector<int> GetReversedArray(int size) const {
        size = min(size, max_length_);
        vector<int> reversed_array(base_array_.begin(), base_array_.begin() + size);
        std::sort(reversed_array.rbegin(), reversed_array.rend());
        return reversed_array;
    }

    // Почти отсортированный массив нужной длины
    vector<int> GetNearlySortedArray(int size, int swap_count) const {
        size = min(size, max_length_);
        vector<int> nearly_sorted_array(base_array_.begin(), base_array_.begin() + size);
        std::sort(nearly_sorted_array.begin(), nearly_sorted_array.end());
        std::uniform_int_distribution<int> dist(0, size - 1);
        mt19937 local_gen = gen_;
        for (int i = 0; i < swap_count; ++i) {
            std::swap(nearly_sorted_array[dist(local_gen)], nearly_sorted_array[dist(local_gen)]);
        }
        return nearly_sorted_array;
    }
};

// Класс измерений времени работы алгоритмов
class SortTester {
private:
    const int num_trials_ = 10;
public:
    // Измерение алгоритма Merge Sort
    double MeasureMergeSortTime(const vector<int>& original_array) {
        ll total_time = 0;

        for (int trial = 0; trial < num_trials_; ++trial) {
            vector<int> array_copy = original_array;
            auto start = chrono::high_resolution_clock::now();
            MergeSort(array_copy, 0, array_copy.size() - 1);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            total_time += duration.count();
        }
        return static_cast<double>(total_time) / num_trials_;
    }

    // Измерение гибридного алгоритма Merge + Insertion Sort
    double MeasureHybridMergeSortTime(const vector<int>& original_array, int threshold) {
        ll total_time = 0;
        for (int trial = 0; trial < num_trials_; ++trial) {
            vector<int> array_copy = original_array;
            auto start = chrono::high_resolution_clock::now();
            HybridMergeSort(array_copy, 0, array_copy.size() - 1, threshold);
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

    vector<int> thresholds = {5, 10, 20, 30, 50};

    ofstream out_file1("C:\\Users\\79655\\CLionProjects\\hse\\merge_random.txt");
    ofstream out_file2("C:\\Users\\79655\\CLionProjects\\hse\\merge_reversed.txt");
    ofstream out_file3("C:\\Users\\79655\\CLionProjects\\hse\\merge_nearly_sorted.txt");
    ofstream out_file4("C:\\Users\\79655\\CLionProjects\\hse\\hybrid_random.txt");
    ofstream out_file5("C:\\Users\\79655\\CLionProjects\\hse\\hybrid_reversed.txt");
    ofstream out_file6("C:\\Users\\79655\\CLionProjects\\hse\\hybrid_nearly_sorted.txt");


    for (int size = 500; size <= 10000; size += 100) {
        vector<int> random_array = generator.GetRandomArray(size);
        vector<int> reversed_array = generator.GetReversedArray(size);
        vector<int> nearly_sorted_array = generator.GetNearlySortedArray(size, size / 20);

        double random_time = tester.MeasureMergeSortTime(random_array);
        double reversed_time = tester.MeasureMergeSortTime(reversed_array);
        double nearly_sorted_time = tester.MeasureMergeSortTime(nearly_sorted_array);

        out_file1 << size << ' ' << random_time << '\n';
        out_file2 << size << ' ' << reversed_time << '\n';
        out_file3 << size << ' ' << nearly_sorted_time << '\n';

        out_file4 << size;
        out_file5 << size;
        out_file6 << size;
        for (int i = 0; i < thresholds.size(); ++i) {
            random_time = tester.MeasureHybridMergeSortTime(random_array, thresholds[i]);
            reversed_time = tester.MeasureHybridMergeSortTime(reversed_array, thresholds[i]);
            nearly_sorted_time = tester.MeasureHybridMergeSortTime(nearly_sorted_array, thresholds[i]);

            out_file4 << ' ' << random_time;
            out_file5 << ' ' << reversed_time;
            out_file6 << ' ' << nearly_sorted_time;
        }
        out_file4 << '\n';
        out_file5 << '\n';
        out_file6 << '\n';
    }
    cout << "OK";
    return 0;
}