/**
 * @file gradient_zero_visitor_impl.hpp
 * @author Marcus Edel
 *
 * Implementation of the Gradient() function layer abstraction.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_VISITOR_GRADIENT_ZERO_VISITOR_IMPL_HPP
#define MLPACK_METHODS_ANN_VISITOR_GRADIENT_ZERO_VISITOR_IMPL_HPP

// In case it hasn't been included yet.
#include "gradient_zero_visitor.hpp"

namespace mlpack {
namespace ann {

//! GradientZeroVisitor visitor class.
inline GradientZeroVisitor::GradientZeroVisitor()
{
  /* Nothing to do here. */
}

template<typename LayerType>
inline void GradientZeroVisitor::operator()(LayerType* layer) const
{
  LayerGradients(layer, layer->OutputParameter());
}

template<typename T>
inline typename std::enable_if<
    HasGradientCheck<T, arma::mat&(T::*)()>::value, void>::type
GradientZeroVisitor::LayerGradients(T* layer, arma::mat& /* input */) const
{
  layer->Gradient().zeros();
}

template<typename T, typename P>
inline typename std::enable_if<
    !HasGradientCheck<T, P&(T::*)()>::value, void>::type
GradientZeroVisitor::LayerGradients(T* /* layer */, P& /* input */) const
{
  /* Nothing to do here. */
}

} // namespace ann
} // namespace mlpack

#endif
