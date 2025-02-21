#include <concepts>
#include <iostream>
#include <utility>
#include <vector>

template <typename T>
concept Sortable = requires(T a, T b) {
  { a < b } -> std::convertible_to<bool>;
};

/////// BUBBLE ///////

template <Sortable T> void bubble_sort(std::vector<T> &arr) {
  bool swapped = false;
  for (size_t i = 0; i < arr.size() - 1; ++i) {
    swapped = false;
    // Traverse the array and bubble the largest element to the end
    for (size_t j = 0; j < arr.size() - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
        swapped = true;
      }
    }
    // If no two elements were swapped in inner loop, array is sorted
    if (!swapped) {
      break;
    }
  }
}

/////// MERGESORT ///////

template <Sortable T>
void merge(std::vector<T> &arr, std::size_t left, std::size_t mid,
           std::size_t right) {
  std::size_t n1 = mid - left + 1;
  std::size_t n2 = right - mid;

  // Create temporary vectors
  std::vector<T> left_arr(n1);
  std::vector<T> right_arr(n2);

  // Copy data into temporary vectors
  std::copy(arr.begin() + left, arr.begin() + mid + 1, left_arr.begin());
  std::copy(arr.begin() + mid + 1, arr.begin() + right + 1, right_arr.begin());

  // Merge the temporary arrays back into arr[left..right]
  std::size_t i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (left_arr[i] <= right_arr[j]) {
      arr[k] = left_arr[i];
      i++;
    } else {
      arr[k] = right_arr[j];
      j++;
    }
    k++;
  }

  // Copy remaining elements of leftArr[], if any
  while (i < n1) {
    arr[k] = left_arr[i];
    i++;
    k++;
  }

  // Copy remaining elements of rightArr[], if any
  while (j < n2) {
    arr[k] = right_arr[j];
    j++;
    k++;
  }
}

// MergeSort function using templates and concepts
template <Sortable T>
void merge_sort(std::vector<T> &arr, std::size_t left, std::size_t right) {
  if (left < right) {
    std::size_t mid = left + (right - left) / 2;

    // Recursively sort both halves
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);

    // Merge the sorted halves
    merge(arr, left, mid, right);
  }
}

// Public interface for merge sort that doesn't require specifying array
// boundaries
template <Sortable T> void merge_sort(std::vector<T> &arr) {
  if (!arr.empty()) {
    merge_sort(arr, 0, arr.size() - 1);
  }
}

/////// QUICKSORT ///////

template <Sortable T>
std::size_t partition(std::vector<T> &arr, std::size_t low, std::size_t high) {
  T pivot = arr[high]; // Choose the pivot as the last element
  std::size_t i = low - 1;

  for (std::size_t j = low; j < high; ++j) {
    if (arr[j] < pivot) {
      ++i;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[high]);
  return i + 1;
}

template <Sortable T>
void quick_sort(std::vector<T> &arr, std::size_t low, std::size_t high) {
  if (low < high) {
    std::size_t pi = partition(arr, low, high); // Partitioning index
    quick_sort(arr, low, pi - 1);  // Recursively sort the left subarray
    quick_sort(arr, pi + 1, high); // Recursively sort the right subarray
  }
}

// Public interface for quicksort that doesn't require specifying array
// boundaries
template <Sortable T> void quick_sort(std::vector<T> &arr) {
  if (!arr.empty()) {
    quick_sort(arr, 0, arr.size() - 1);
  }
}
