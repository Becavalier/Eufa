/**
 * @file negative_log_likelihood_impl.hpp
 * @author Marcus Edel
 *
 * Implementation of the NegativeLogLikelihood class.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_NEGATIVE_LOG_LIKELIHOOD_IMPL_HPP
#define MLPACK_METHODS_ANN_LAYER_NEGATIVE_LOG_LIKELIHOOD_IMPL_HPP

// In case it hasn't yet been included.
#include "negative_log_likelihood.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

template<typename InputDataType, typename OutputDataType>
NegativeLogLikelihood<InputDataType, OutputDataType>::NegativeLogLikelihood()
{
  // Nothing to do here.
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
double NegativeLogLikelihood<InputDataType, OutputDataType>::Forward(
    const arma::Mat<eT>&& input, arma::Mat<eT>&& target)
{
  double output = 0;

  for (size_t i = 0; i < input.n_cols; ++i)
  {
    size_t currentTarget = target(i) - 1;
    Log::Assert(currentTarget >= 0 && currentTarget < input.n_rows,
        "Target class out of range.");

    output -= input(currentTarget, i);
  }

  return output;
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
void NegativeLogLikelihood<InputDataType, OutputDataType>::Backward(
      const arma::Mat<eT>&& input,
      const arma::Mat<eT>&& target,
      arma::Mat<eT>&& output)
{
  output = arma::zeros<arma::Mat<eT> >(input.n_rows, input.n_cols);
  for (size_t i = 0; i < input.n_cols; ++i)
  {
    size_t currentTarget = target(i) - 1;
    Log::Assert(currentTarget >= 0 && currentTarget < input.n_rows,
        "Target class out of range.");

    output(currentTarget, i) = -1;
  }
}

template<typename InputDataType, typename OutputDataType>
template<typename Archive>
void NegativeLogLikelihood<InputDataType, OutputDataType>::Serialize(
    Archive& /* ar */,
    const unsigned int /* version */)
{
  // Nothing to do here.
}

} // namespace ann
} // namespace mlpack

#endif
