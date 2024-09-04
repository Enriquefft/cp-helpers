#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

using ll = intmax_t;

using std::function;
using std::optional;

// Concept to ensure Op is a valid binary operation
template <typename Op, typename T>
concept BinaryOperation = requires(T left, T right, Op operation) {
  { operation(left, right) } -> std::same_as<T>;
};

template <typename InputIt, typename value_type>
concept ValueInputIterator =
    std::input_iterator<InputIt> &&
    std::constructible_from<value_type,
                            typename std::iterator_traits<InputIt>::value_type>;


template <typename T, typename Stored, BinaryOperation<Stored> Op>
class SegmentTree {
public:
  using value_type = T;
  using stored_type = Stored;
  using operation_type = Op;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  SegmentTree(const std::vector<value_type> &data, Op _op,
              function<stored_type(value_type)> _make_data)
      : m_op(std::move(_op)), m_size(data.size()),
        m_make_data(std::move(_make_data)) {
    build_tree(data.begin(), data.end());
  }

  SegmentTree(const std::initializer_list<value_type> &data, Op _op)
      : m_op(_op), m_size(data.size()) {
    build_tree(data.begin(), data.end());
  }

  template <ValueInputIterator<value_type> Iterator>
  SegmentTree(Iterator first, Iterator last, Op _op)
      : m_op(_op), m_size(std::distance(first, last)) {
    build_tree(first, last);
  }

  [[nodiscard]] auto query_range(size_type left, size_type right) const
      -> optional<stored_type> {
    auto response = query_range_helper(1, 0, m_size - 1, left, right);
    if (response.has_value()) {
      return response.value();
    }
    throw std::invalid_argument("Invalid range");
  }

  // Update the element at position pos
  void update_element(size_type pos, const value_type &new_value) {
    update_element_helper(1, 0, m_size - 1, pos, new_value);
  }

private:
  std::vector<stored_type> m_tree;
  Op m_op;
  size_type m_size;
  function<stored_type(value_type)> m_make_data = [](const value_type &value) {
    return value;
  };

  /// Combine function which acts as an identity function handler
  [[nodiscard]] auto combine(const optional<stored_type> &left,
                             const optional<stored_type> &right) const
      -> optional<stored_type> {
    if (left.has_value() && right.has_value()) {
      return m_op(left.value(), right.value());
    }
    if (left.has_value()) {
      // Return left if right is nullopt
      return left.value();
    }
    if (right.has_value()) {
      // Return right if left is nullopt
      return right.value();
    } // Return nullopt if both left and right are nullopt
    return std::nullopt;
  }

  [[nodiscard]] auto
  query_range_helper(size_t index, size_type left,

                     size_type right, size_type query_left, size_type query_right) const -> optional<stored_type> {
    if (query_left > query_right) {
      return std::nullopt;
    }
    if (left == query_left && right == query_right) {
      return m_tree.at(index);
    }
    size_type query_center = (left + right) / 2;
    return combine(
        query_range_helper(index * 2, left, query_center, query_left, std::min(query_right, query_center)),
        query_range_helper(index * 2 + 1, query_center + 1, right, std::max(query_left, query_center + 1),
                           query_right));
  }

  void update_element_helper(size_t index, size_type left, size_type right,
                             size_type pos, const value_type &new_value) {
    if (left == right) {
      m_tree.at(index) = m_make_data(new_value);
    } else {
      size_type query_center = (left + right) / 2;
      if (pos <= query_center) {
        update_element_helper(index * 2, left, query_center, pos, new_value);
      } else {
        update_element_helper(index * 2 + 1, query_center + 1, right, pos, new_value);
      }
      m_tree.at(index) =
          combine(m_tree.at(index * 2), m_tree.at(index * 2 + 1)).value();
    }
  }

  template <typename Iterator> void build_tree(Iterator first, Iterator last) {
    m_tree.resize(m_size * 4);
    auto build_tree_helper = [&first, this](auto &&self, size_t index,
                                            difference_type left,
                                            difference_type right) -> void {
      if (left == right) {
        m_tree.at(index) = m_make_data(*std::next(first, left));
      } else {
        int64_t query_center = (left + right) / 2;
        self(self, index * 2, left, query_center);
        self(self, index * 2 + 1, query_center + 1, right);
        m_tree.at(index) =
            combine(m_tree.at(index * 2), m_tree.at(index * 2 + 1)).value();
      }
    };

    build_tree_helper(build_tree_helper, 1, 0, std::distance(first, last) - 1);
  }
};
