/**
 * @file all_dimension_select.hpp
 * @author Ryan Curtin
 *
 * Selects all dimensions for a split.
 */
#ifndef MLPACK_METHODS_DECISION_TREE_ALL_DIMENSION_SELECT_HPP
#define MLPACK_METHODS_DECISION_TREE_ALL_DIMENSION_SELECT_HPP

namespace mlpack {
namespace tree {

/**
 * This dimension selection policy allows any dimension to be selected for
 * splitting.
 */
class AllDimensionSelect
{
 public:
  /**
   * Construct the AllDimensionSelect object for the given number of dimensions.
   */
  AllDimensionSelect(const size_t dimensions) : i(0), dimensions(dimensions) { }

  /**
   * Get the first dimension to select from.
   */
  size_t Begin()
  {
    i = 0;
    return 0;
  }

  /**
   * Get the last dimension to select from.
   */
  size_t End() const { return dimensions; }

  /**
   * Get the next dimension.
   */
  size_t Next() { return ++i; }

 private:
  //! The current dimension we are looking at.
  size_t i;
  //! The number of dimensions to select from.
  const size_t dimensions;
};

} // namespace tree
} // namespace mlpack

#endif
