#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <numeric>
#include <windows.h>
// #include <omp.h>

std::vector<size_t> sort_indexes(const std::vector<float> &v) {
  //initialize original index locations
  std::vector<size_t> idx(v.size());
  iota(idx.begin(), idx.end(), 0);
  //sort indexes based on comparing values in v
  std::stable_sort(idx.begin(), idx.end(),
    [&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });
  return idx;
}

std::vector<size_t> sort_indexes(const std::vector<int> &v) {
  //initialize original index locations
  std::vector<size_t> idx(v.size());
  iota(idx.begin(), idx.end(), 0);
  //sort indexes based on comparing values in v
  std::stable_sort(idx.begin(), idx.end(),
    [&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });
  return idx;
}

void knn_sparse(float* cost_matrix, int n, int m, int k, bool sorted, int* sort_index_matrix) {

  DWORD st, ed;
  st = GetTickCount();

  bool* connected = new bool[n * m];
  memset(connected, false, n * m * sizeof(bool));
  std::vector<size_t>* radish_sort_dist_index = new std::vector<size_t>[m];
  int* hole_count = new int[n];
  memset(hole_count, 0, n * sizeof(int));
  int* radish_toCheck = new int[m];

  if (!sorted) {
// #pragma omp parallel for 
    for (int j = 0; j < m; ++j) {
      std::vector<float> col;
      for (int i = 0; i < n; ++i) {
        col.push_back(cost_matrix[m * i + j]);
      }
      // col_sort = sort_indexes(col);
      radish_sort_dist_index[j] = sort_indexes(col);
      radish_toCheck[j] = k;
    }
    for (int j = 0; j < m; ++j) {
      for (int c = 0; c < k; ++c) {
        hole_count[radish_sort_dist_index[j][c]] += 1;
        connected[radish_sort_dist_index[j][c] * m + j] = true;
      }
    }
  } else {
// #pragma omp parallel for 
    // sort_index_matrix: shape = m * n
    for (int j = 0; j < m; ++j) {
      radish_sort_dist_index[j] = std::vector<size_t>(sort_index_matrix + (j * n), sort_index_matrix + ((j + 1) * n));
      radish_toCheck[j] = k;
    }
    for (int j = 0; j < m; ++j) {
      for (int c = 0; c < k; ++c) {
        hole_count[radish_sort_dist_index[j][c]] += 1;
        connected[radish_sort_dist_index[j][c] * m + j] = true;
      }
    }
  }
  ed = GetTickCount();
  // printf("Sort Time: %dms\n", ed - st);
  std::vector<int> hc(hole_count, hole_count + n);
  std::vector<size_t> hc_sort = sort_indexes(hc);
  for (int i = n - 1; i >= 0; --i) {
    int hole = hc_sort[i];
    if (hole_count[hole] <= k) {
      break;
    }
    bool* hole_connected_start = &connected[m * hole];
    float* hole_cost_start = &cost_matrix[m * hole];
    std::vector<int> rs;
    std::vector<float> distances;
    for (int j = 0; j < m; ++j) {
      if (hole_connected_start[j]) {
        rs.push_back(j);
        distances.push_back(hole_cost_start[j]);
      }
    }
    std::vector<size_t> order = sort_indexes(distances);
    for (int j = order.size() - 1; j >= 0 && hole_count[hole] > k; --j) {
      int radish_idx = rs[order[j]];
      int next_hole_to_assign;
      while (true) {
        if (radish_toCheck[radish_idx] == n) {
          break;
        }
        next_hole_to_assign = radish_sort_dist_index[radish_idx][radish_toCheck[radish_idx]];
        radish_toCheck[radish_idx]++;
        if (hole_count[next_hole_to_assign] < k && !connected[next_hole_to_assign * m + radish_idx]) {
          hole_count[next_hole_to_assign]++;
          connected[next_hole_to_assign * m + radish_idx] = true;
          hole_count[hole]--;
          hole_connected_start[radish_idx] = false;
          break;
        }
      }
    }
  }
  
  for (int i = 0; i < n; ++i) {
    float* tmp = &cost_matrix[m * i];
    for (int j = 0; j < m; ++j) {
      if (!connected[i * m + j]) {
        tmp[j] = 0;
      }
    }
  }
  delete[] connected;
  delete[] hole_count;
  delete[] radish_sort_dist_index;
  delete[] radish_toCheck;
  ed = GetTickCount();
  // printf("Sparse Time: %dms\n", ed - st);
}

int main() {
  std::vector<float> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(1);
  a.push_back(2);
  a.push_back(1);
  a.push_back(2);
  a.push_back(1);
  a.push_back(2);
  std::vector<size_t> b = sort_indexes(a);
  for (int i = 0; i < b.size(); ++i) {
    printf("%d, ", b[i]);
  }
}