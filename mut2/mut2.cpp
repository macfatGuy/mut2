#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip> 

using namespace std;


vector<double> sumVectors(const vector<double>& v1, const vector<double>& v2) {
    vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}


vector<double> sumVectorsParallel(const vector<double>& v1, const vector<double>& v2, int numThreads) {
    vector<double> result(v1.size());
    vector<thread> threads;


    size_t chunkSize = v1.size() / numThreads;
    for (int i = 0; i < numThreads; i++) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? v1.size() : (i + 1) * chunkSize;
        threads.push_back(thread([&v1, &v2, &result, start, end]() {
            for (size_t j = start; j < end; j++) {
                result[j] = v1[j] + v2[j];
            }
            }));
    }

   
    for (auto& thread : threads) {
        thread.join();
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");

    
    cout << "Количество ядер: " << thread::hardware_concurrency() << endl;

    
    vector<size_t> sizes = { 1000, 10000, 100000, 1000000 };

  
    vector<vector<double>> results;

    for (size_t size : sizes) {
      
        vector<double> v1(size, 1.0);
        vector<double> v2(size, 2.0);

        vector<double> times;

        // 1 поток
        auto start = chrono::high_resolution_clock::now();
        sumVectors(v1, v2);
        auto end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration<double>(end - start).count());

        // 2 потока
        start = chrono::high_resolution_clock::now();
        sumVectorsParallel(v1, v2, 2);
        end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration<double>(end - start).count());

        // 4 потока
        start = chrono::high_resolution_clock::now();
        sumVectorsParallel(v1, v2, 4);
        end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration<double>(end - start).count());

        // 8 потоков
        start = chrono::high_resolution_clock::now();
        sumVectorsParallel(v1, v2, 8);
        end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration<double>(end - start).count());

        // 16 потоков
        start = chrono::high_resolution_clock::now();
        sumVectorsParallel(v1, v2, 16);
        end = chrono::high_resolution_clock::now();
        times.push_back(chrono::duration<double>(end - start).count());

        
        results.push_back(times);
    }

  
    cout << endl << "Время выполнения (секунды):" << endl;
    cout << "Размер массива | 1 поток | 2 потока | 4 потока | 8 потоков | 16 потоков" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    for (size_t i = 0; i < sizes.size(); i++) {
        cout << sizes[i] << "             |";
        for (size_t j = 0; j < results[i].size(); j++) {
            cout << fixed << setprecision(3) << results[i][j] << "        |";
        }
        cout << endl;
    }
    return 0;
}
