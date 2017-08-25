/**
 * @file logistic_regression_function.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of the LogisticRegressionFunction class.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_LOGISTIC_REGRESSION_FUNCTION_IMPL_HPP
#define MLPACK_METHODS_LOGISTIC_REGRESSION_FUNCTION_IMPL_HPP

// In case it hasn't been included yet.
#include "logistic_regression_function.hpp"

namespace mlpack {
namespace regression {

template<typename MatType>
LogisticRegressionFunction<MatType>::LogisticRegressionFunction(
    const MatType& predictors,
    const arma::Row<size_t>& responses,
    const double lambda) :
    predictors(predictors),
    responses(responses),
    lambda(lambda)
{
  initialPoint = arma::zeros<arma::mat>(predictors.n_rows + 1, 1);

  // Sanity check.
  if (responses.n_elem != predictors.n_cols)
    Log::Fatal << "LogisticRegressionFunction::LogisticRegressionFunction(): "
        << "predictors matrix has " << predictors.n_cols << " points, but "
        << "responses vector has " << responses.n_elem << " elements (should be"
        << " " << predictors.n_cols << ")!" << std::endl;
}

template<typename MatType>
LogisticRegressionFunction<MatType>::LogisticRegressionFunction(
    const MatType& predictors,
    const arma::Row<size_t>& responses,
    const arma::vec& initialPoint,
    const double lambda) :
    initialPoint(initialPoint),
    predictors(predictors),
    responses(responses),
    lambda(lambda)
{
  // To check if initialPoint is compatible with predictors.
  if (initialPoint.n_rows != (predictors.n_rows + 1) ||
      initialPoint.n_cols != 1)
    this->initialPoint = arma::zeros<arma::mat>(predictors.n_rows + 1, 1);
}

/**
 * Evaluate the logistic regression objective function given the estimated
 * parameters.
 */
template<typename MatType>
double LogisticRegressionFunction<MatType>::Evaluate(
    const arma::mat& parameters) const
{
  // The objective function is the log-likelihood function (w is the parameters
  // vector for the model; y is the responses; x is the predictors; sig() is the
  // sigmoid function):
  //   f(w) = sum(y log(sig(w'x)) + (1 - y) log(sig(1 - w'x))).
  // We want to minimize this function.  L2-regularization is just lambda
  // multiplied by the squared l2-norm of the parameters then divided by two.

  // For the regularization, we ignore the first term, which is the intercept
  // term.
  const double regularization = 0.5 * lambda *
      arma::dot(parameters.col(0).subvec(1, parameters.n_elem - 1),
                parameters.col(0).subvec(1, parameters.n_elem - 1));

  // Calculate vectors of sigmoids.  The intercept term is parameters(0, 0) and
  // does not need to be multiplied by any of the predictors.
  const arma::vec exponents = parameters(0, 0) + predictors.t() *
      parameters.col(0).subvec(1, parameters.n_elem - 1);
  const arma::vec sigmoid = 1.0 / (1.0 + arma::exp(-exponents));

  // Assemble full objective function.  Often the objective function and the
  // regularization as given are divided by the number of features, but this
  // doesn't actually affect the optimization result, so we'll just ignore those
  // terms for computational efficiency.
  double result = 0.0;
  for (size_t i = 0; i < responses.n_elem; ++i)
  {
    if (responses[i] == 1)
      result += log(sigmoid[i]);
    else
      result += log(1.0 - sigmoid[i]);
  }

  // Invert the result, because it's a minimization.
  return -result + regularization;
}

/**
 * Evaluate the logistic regression objective function, but with only one point.
 * This is useful for optimizers that use a separable objective function, such
 * as SGD.
 */
template<typename MatType>
double LogisticRegressionFunction<MatType>::Evaluate(
    const arma::mat& parameters,
    const size_t i) const
{
  // Calculate the regularization term.  We must divide by the number of points,
  // so that sum(Evaluate(parameters, [1:points])) == Evaluate(parameters).
  const double regularization = lambda * (1.0 / (2.0 * predictors.n_cols)) *
      arma::dot(parameters.col(0).subvec(1, parameters.n_elem - 1),
                parameters.col(0).subvec(1, parameters.n_elem - 1));

  // Calculate sigmoid.
  const double exponent = parameters(0, 0) + arma::dot(predictors.col(i),
      parameters.col(0).subvec(1, parameters.n_elem - 1));
  const double sigmoid = 1.0 / (1.0 + std::exp(-exponent));

  if (responses[i] == 1)
    return -log(sigmoid) + regularization;
  else
    return -log(1.0 - sigmoid) + regularization;
}

//! Evaluate the gradient of the logistic regression objective function.
template<typename MatType>
void LogisticRegressionFunction<MatType>::Gradient(
    const arma::mat& parameters,
    arma::mat& gradient) const
{
  // Regularization term.
  arma::mat regularization;
  regularization = lambda * parameters.col(0).subvec(1, parameters.n_elem - 1);

  const arma::rowvec sigmoids = (1 / (1 + arma::exp(-parameters(0, 0)
      - parameters.col(0).subvec(1, parameters.n_elem - 1).t() * predictors)));

  gradient.set_size(parameters.n_elem);
  gradient[0] = -arma::accu(responses - sigmoids);
  gradient.col(0).subvec(1, parameters.n_elem - 1) = -predictors * (responses -
      sigmoids).t() + regularization;
}

/**
 * Evaluate the individual gradients of the logistic regression objective
 * function with respect to individual points.  This is useful for optimizers
 * that use a separable objective function, such as SGD.
 */
template <typename MatType>
template <typename GradType>
void LogisticRegressionFunction<MatType>::Gradient(
    const arma::mat& parameters,
    const size_t i,
    GradType& gradient) const
{
  // Calculate the regularization term.
  GradType regularization;
  regularization = lambda * parameters.col(0).subvec(1, parameters.n_elem - 1)
      / predictors.n_cols;

  const double sigmoid = 1.0 / (1.0 + std::exp(-parameters(0, 0)
      - arma::dot(predictors.col(i), parameters.col(0).subvec(1,
      parameters.n_elem - 1))));

  gradient.set_size(parameters.n_elem);
  gradient[0] = -(responses[i] - sigmoid);
  gradient.col(0).subvec(1, parameters.n_elem - 1) = -predictors.col(i)
      * (responses[i] - sigmoid) + regularization;
}

} // namespace regression
} // namespace mlpack

#endif
