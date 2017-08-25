/**
 * @file parametric_relu.hpp
 * @author Prasanna Patil
 *
 * Definition of PReLU layer first introduced in the,
 * Kaiming He, Xiangyu Zhang, Shaoqing, Ren Jian Sun,
 * "Delving Deep into Rectifiers:
 * Surpassing Human-Level Performance on ImageNet Classification", 2014
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_PReLU_HPP
#define MLPACK_METHODS_ANN_LAYER_PReLU_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * The PReLU activation function, defined by (where alpha is trainable)
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
class PReLU
{
 public:
  /**
   * Create the PReLU object using the specified parameters.
   * The non zero gradient can be adjusted by specifying tha parameter
   * alpha in the range 0 to 1. Default (alpha = 0.03). This parameter
   * is trainable.
   *
   * @param alpha Non zero gradient
   */
  PReLU(const double user_alpha = 0.03);

  /*
   * Reset the layer parameter.
   */
  void Reset();

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

  /**
   * Calculate the gradient using the output delta and the input activation.
   *
   * @param input The input parameter used for calculating the gradient.
   * @param error The calculated error.
   * @param gradient The calculated gradient.
   */
  template<typename eT>
  void Gradient(const arma::Mat<eT>&& input,
                arma::Mat<eT>&& error,
                arma::Mat<eT>&& gradient);

  //! Get the parameters.
  OutputDataType const& Parameters() const { return alpha; }
  //! Modify the parameters.
  OutputDataType& Parameters() { return alpha; }

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

  //! Get the gradient.
  OutputDataType const& Gradient() const { return gradient; }
  //! Modify the gradient.
  OutputDataType& Gradient() { return gradient; }

  //! Get the non zero gradient.
  double const& Alpha() const { return alpha(0); }
  //! Modify the non zero gradient.
  double& Alpha() { return alpha(0); }

  /**
   * Serialize the layer.
   */
  template<typename Archive>
  void Serialize(Archive& ar, const unsigned int /* version */);

 private:
  /**
   * Computes the parametric ReLU function.
   *
   * @param x Input data.
   * @return f(x).
   */
  double Fn(const double x)
  {
    return std::max(x, alpha(0) * x);
  }

  /**
   * Computes the parametric ReLU function using a dense matrix as input.
   *
   * @param x Input data.
   * @param y The resulting output activation.
   */
  template<typename eT>
  void Fn(const arma::Mat<eT>& x, arma::Mat<eT>& y)
  {
    y = x;
    arma::uvec negative = arma::find(x < 0);
    y(negative) = x(negative) * alpha(0);
  }

  /**
   * Computes the first derivative of the parametric ReLU function.
   *
   * @param x Input data.
   * @return f'(x)
   */
  double Deriv(const double x)
  {
    return (x >= 0) ? 1 : alpha(0);
  }

  /**
   * Computes the first derivative of the PReLU function.
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

  //! Leakyness Parameter object.
  OutputDataType alpha;

  //! Locally-stored gradient object.
  OutputDataType gradient;

  //! Leakyness Parameter given by user in the range 0 < alpha < 1.
  double user_alpha;
}; // class PReLU

} // namespace ann
} // namespace mlpack

// Include implementation.
#include "parametric_relu_impl.hpp"

#endif
