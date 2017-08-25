/**
 * @file leaky_relu.hpp
 * @author Dhawal Arora
 *
 * Definition of LeakyReLU layer first introduced in the acoustic model,
 * Andrew L. Maas, Awni Y. Hannun, Andrew Y. Ng,
 * "Rectifier Nonlinearities Improve Neural Network Acoustic Models", 2014
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_LEAKYRELU_HPP
#define MLPACK_METHODS_ANN_LAYER_LEAKYRELU_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * The LeakyReLU activation function, defined by
 *
 * @f{eqnarray*}{
 * f(x) &=& \max(x, alpha*x) \\
 * f'(x) &=& \left\{
 *   \begin{array}{lr}
 *     1 & : x > 0 \\
 *     alpha & : x \le 0
 *   \end{array}
 * \right.
 * @f}
 *
 * @tparam InputDataType Type of the input data (arma::colvec, arma::mat,
 *         arma::sp_mat or arma::cube).
 * @tparam OutputDataType Type of the output data (arma::colvec, arma::mat,
 *         arma::sp_mat or arma::cube).
 */
template <
    typename InputDataType = arma::mat,
    typename OutputDataType = arma::mat
>
class LeakyReLU
{
 public:
  /**
   * Create the LeakyReLU object using the specified parameters.
   * The non zero gradient can be adjusted by specifying tha parameter
   * alpha in the range 0 to 1. Default (alpha = 0.03)
   *
   * @param alpha Non zero gradient
   */
  LeakyReLU(const double alpha = 0.03);

  /**
   * Ordinary feed forward pass of a neural network, evaluating the function
   * f(x) by propagating the activity forward through f.
   *
   * @param input Input data used for evaluating the specified function.
   * @param output Resulting output activation.
   */
  template<typename InputType, typename OutputType>
  void Forward(const InputType&& input, OutputType&& output);

  /**
   * Ordinary feed backward pass of a neural network, calculating the function
   * f(x) by propagating x backwards through f. Using the results from the feed
   * forward pass.
   *
   * @param input The propagated input activation.
   * @param gy The backpropagated error.
   * @param g The calculated gradient.
   */
  template<typename DataType>
  void Backward(const DataType&& input, DataType&& gy, DataType&& g);

  //! Get the input parameter.
  InputDataType const& InputParameter() const { return inputParameter; }
  //! Modify the input parameter.
  InputDataType& InputParameter() { return inputParameter; }

  //! Get the output parameter.
  OutputDataType const& OutputParameter() const { return outputParameter; }
  //! Modify the output parameter.
  OutputDataType& OutputParameter() { return outputParameter; }

  //! Get the delta.
  OutputDataType const& Delta() const { return delta; }
  //! Modify the delta.
  OutputDataType& Delta() { return delta; }

  //! Get the non zero gradient.
  double const& Alpha() const { return alpha; }
  //! Modify the non zero gradient.
  double& Alpha() { return alpha; }

  /**
   * Serialize the layer.
   */
  template<typename Archive>
  void Serialize(Archive& ar, const unsigned int /* version */);

 private:
  /**
   * Computes the LeakReLU function
   *
   * @param x Input data.
   * @return f(x).
   */
  double Fn(const double x)
  {
    return std::max(x, alpha * x);
  }

  /**
   * Computes the Leaky ReLU function using a dense matrix as input.
   *
   * @param x Input data.
   * @param y The resulting output activation.
   */
  template<typename eT>
  void Fn(const arma::Mat<eT>& x, arma::Mat<eT>& y)
  {
    y = arma::max(x, alpha * x);
  }

  /**
   * Computes the first derivative of the LeakyReLU function.
   *
   * @param x Input data.
   * @return f'(x)
   */
  double Deriv(const double x)
  {
    return (x >= 0) ? 1 : alpha;
  }

  /**
   * Computes the first derivative of the LeakyReLU function.
   *
   * @param y Input activations.
   * @param x The resulting derivatives.
   */

  template<typename InputType, typename OutputType>
  void Deriv(const InputType& x, OutputType& y)
  {
    y = x;

    for (size_t i = 0; i < x.n_elem; i++)
    {
      y(i) = Deriv(x(i));
    }
  }

  //! Locally-stored delta object.
  OutputDataType delta;

  //! Locally-stored input parameter object.
  InputDataType inputParameter;

  //! Locally-stored output parameter object.
  OutputDataType outputParameter;

  //! Leakyness Parameter in the range 0 <alpha< 1
  double alpha;
}; // class LeakyReLU

} // namespace ann
} // namespace mlpack

// Include implementation.
#include "leaky_relu_impl.hpp"

#endif
