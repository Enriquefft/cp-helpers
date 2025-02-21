#include <stdexcept>
#include <unordered_map>
#include <vector>

/**
 * @brief A generic Disjoint Set Union (Union-Find) data structure.
 *
 * This structure supports elements of any type and provides efficient
 * union and find operations with path compression and union by rank
 * optimizations.
 *
 * @tparam T The type of the elements.
 */
template <typename T> class DSU {
public:
  /**
   * @brief Constructs a DSU with the given elements.
   *
   * Initializes each element to be its own set.
   *
   * @param elements A container of elements to be managed by the DSU.
   * @throws std::invalid_argument if duplicate elements are provided.
   */
  template <typename Container> explicit DSU(const Container &elements) {
    int index = 0;
    for (const auto &elem : elements) {
      if (m_element_to_index.find(elem) != m_element_to_index.end()) {
        throw std::invalid_argument(
            "Duplicate elements are not allowed in DSU.");
      }
      m_element_to_index[elem] = index;
      m_index_to_element.push_back(elem);
      m_parent.push_back(index);
      m_rank.push_back(0);
      index++;
    }
  }

  /**
   * @brief Finds the representative (root) of the set containing the element.
   *
   * Applies path compression for optimization.
   *
   * @param element The element whose set representative is to be found.
   * @return The representative element of the set containing the given element.
   * @throws std::out_of_range if the element is not present in the DSU.
   */
  T find_set(const T &element) {
    int index = find_set_index(element);
    return m_index_to_element[m_parent[index]];
  }

  /**
   * @brief Unions the sets containing the two given elements.
   *
   * Uses union by rank to attach the smaller tree under the root of the deeper
   * tree.
   *
   * @param element1 The first element.
   * @param element2 The second element.
   * @throws std::out_of_range if either element is not present in the DSU.
   */
  void union_set(const T &element1, const T &element2) {
    int root1 = find_set_index(element1);
    int root2 = find_set_index(element2);

    if (root1 == root2) {
      return; // Already in the same set
    }

    // Union by rank
    if (m_rank[root1] < m_rank[root2]) {
      m_parent[root1] = root2;
    } else if (m_rank[root1] > m_rank[root2]) {
      m_parent[root2] = root1;
    } else {
      m_parent[root2] = root1;
      m_rank[root1]++;
    }
  }

  /**
   * @brief Checks if two elements are in the same set.
   *
   * @param element1 The first element.
   * @param element2 The second element.
   * @return true if both elements are in the same set, false otherwise.
   * @throws std::out_of_range if either element is not present in the DSU.
   */
  bool connected(const T &element1, const T &element2) const {
    return find_set(element1) == find_set(element2);
  }

private:
  std::vector<int> m_parent; ///< Parent pointers for each element
  std::vector<int> m_rank;   ///< Rank (approximate tree depth) for each set
  std::unordered_map<T, int>
      m_element_to_index;            ///< Mapping from element to internal index
  std::vector<T> m_index_to_element; ///< Mapping from internal index to element

  /**
   * @brief Finds the set representative's internal index for the given element.
   *
   * Applies path compression.
   *
   * @param element The element whose set representative's index is to be found.
   * @return The internal index of the set representative.
   * @throws std::out_of_range if the element is not present in the DSU.
   */
  int find_set_index(const T &element) const {
    auto it = m_element_to_index.find(element);
    if (it == m_element_to_index.end()) {
      throw std::out_of_range("Element not found in DSU.");
    }
    int index = it->second;
    if (m_parent[index] != index) {
      m_parent[index] = find_set_index(m_index_to_element[m_parent[index]]);
    }
    return m_parent[index];
  }
};
