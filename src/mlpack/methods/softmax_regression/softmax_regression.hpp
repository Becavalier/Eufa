/**
 * @file softmax_regression.hpp
 * @author Siddharth Agrawal
 *
 * An implementation of softmax regression.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_SOFTMAX_REGRESSION_SOFTMAX_REGRESSION_HPP
#define MLPACK_METHODS_SOFTMAX_REGRESSION_SOFTMAX_REGRESSION_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/core/optimizers/lbfgs/lbfgs.hpp>

#include "softmax_regression_function.hpp"

namespace mlpack {
namespace regression {

/**
 * Softmax Regression is a classifier which can be used for classification when
 * the data available can take two or more class values. It is a generalization
 * of Logistic Regression (which is used only for binary classification). The
 * model has a different set of parameters for each class, but can be easily
 * converted into a vectorized implementation as has been done in this module.
 * The model can be used for direct classification of feature data or in
 * conjunction with unsupervised learning methods. More technical details about
 * the model can be found on the following webpage:
 *
 * http://ufldl.stanford.edu/wiki/index.php/Softmax_Regression
 *
 * An example on how to use the interface is shown below:
 *
 * @code
 * arma::mat train_data; // Training data matrix.
 * arma::vec labels; // Labels associated with the data.
 * const size_t inputSize = 784; // Size of input feature vector.
 * const size_t numClasses = 10; // Number of classes.
 *
 * // Train the model using default options.
 * SoftmaxRegression<> regressor1(train_data, labels, inputSize, numClasses);
 *
 * const size_t numBasis = 5; // Parameter required for L-BFGS algorithm.
 * const size_t numIterations = 100; // Maximum number of iterations.
 *
 * // Use an instantiated optimizer for the training.
 * SoftmaxRegressionFunction srf(train_data, labels, inputSize, numClasses);
 * L_BFGS<SoftmaxRegressionFunction> optimizer(srf, numBasis, numIterations);
 * SoftmaxRegression<L_BFGS> regressor2(optimizer);
 *
 * arma::mat test_data; // Test data matrix.
 * arma::vec predictions1, predictions2; // Vectors to store predictions in.
 *
 * // Obtain predictions from both the learned models.
 * regressor1.Classify(test_data, predictions1);
 * regressor2.Classify(test_data, predictions2);
 * @endcode
 */
class SoftmaxRegression
{
 public:
  /**
   * Initialize the SoftmaxRegression without performing training.  Default
   * value of lambda is 0.0001.  Be sure to use Train() before calling
   * Classify() or ComputeAccuracy(), otherwise the results may be meaningless.
   *
   * @param inputSize Size of the input feature vector.
   * @param numClasses Number of classes for classification.
   * @param fitIntercept add intercept term or not.
   */
  SoftmaxRegression(const size_t inputSize = 0,
                    const size_t numClasses = 0,
                    const bool fitIntercept = false);

  /**
   * Construct the SoftmaxRegression class with the provided data and labels.
   * This will train the model. Optionally, the parameter 'lambda' can be
   * passed, which controls the amount of L2-regularization in the objective
   * function. By default, the model takes a small value.
   *
   * @tparam OptimizerType Desired optimizer type.
   * @param data Input training features. Each column associate with one sample
   * @param labels Labels associated with the feature data.
   * @param inputSize Size of the input feature vector.
   * @param numClasses Number of classes for classification.
   * @param optimizer Desired optimizer.
   * @param lambda L2-regularization constant.
   * @param fitIntercept add intercept term or not.
   */
  template<typename OptimizerType = mlpack::optimization::L_BFGS>
  SoftmaxRegression(const arma::mat& data,
                    const arma::Row<size_t>& labels,
                    const size_t numClasses,
                    const double lambda = 0.0001,
                    const bool fitIntercept = false,
                    OptimizerType optimizer = OptimizerType());

  /**
   * Predict the class labels for the provided feature points. The function
   * calculates the probabilities for every class, given a data point. It then
   * chooses the class which has the highest probability among all.
   *
   * This method is deprecated and will be removed in mlpack 3.0.0. You should
   * use Classify() instead.
   *
   * @param testData Matrix of data points for which predictions are to be made.
   * @param predictions Vector to store the predictions in.
   */
  mlpack_deprecated void Predict(const arma::mat& testData,
                                 arma::Row<size_t>& predictions) const;

  /**
   * Classify the given points, returning the predicted labels for each point.
   * The function calculates the probabilities for every class, given a data
   * point. It then chooses the class which has the highest probability among
   * all.
   *
   * @param dataset Set of points to classify.
   * @param labels Predicted labels for each point.
   */
  void Classify(const arma::mat& dataset, arma::Row<size_t>& labels) const;

  /**
   * Classify the given point. The predicted class label is returned.
   * The function calculates the probabilites for every class, given the point.
   * It then chooses the class which has the highest probability among all.
   *
   * @param point Point to be classified.
   * @return Predicted class label of the point.
   */
  template<typename VecType>
  size_t Classify(const VecType& point) const;

  /**
   * Classify the given points, returning class probabilities and predicted
   * class label for each point.
   * The function calculates the probabilities for every class, given a data
   * point. It then chooses the class which has the highest probability among
   * all.
   *
   * @param dataset Matrix of data points to be classified.
   * @param labels Predicted labels for each point.
   * @param probabilities Class probabilities for each point.
   */
  void Classify(const arma::mat& dataset,
                arma::Row<size_t>& labels,
                arma::mat& probabilites) const;

  /**
   * Classify the given points, returning class probabilities for each point.
   *
   * @param dataset Matrix of data points to be classified.
   * @param probabilities Class probabilities for each point.
   */
  void Classify(const arma::mat& dataset,
                arma::mat& probabilities) const;

  /**
   * Computes accuracy of the learned model given the feature data and the
   * labels associated with each data point. Predictions are made using the
   * provided data and are compared with the actual labels.
   *
   * @param testData Matrix of data points using which predictions are made.
   * @param labels Vector of labels associated with the data.
   */
  double ComputeAccuracy(const arma::mat& testData,
                         const arma::Row<size_t>& labels) const;

  /**
   * Train the softmax regression with the given training data.
   *
   * @tparam OptimizerType Desired optimizer type.
   * @param data Input data with each column as one example.
   * @param labels Labels associated with the feature data.
   * @param numClasses Number of classes for classification.
   * @param optimizer Desired optimizer.
   * @return Objective value of the final point.
   */
  template<typename OptimizerType = mlpack::optimization::L_BFGS>
  double Train(const arma::mat& data,
               const arma::Row<size_t>& labels,
               const size_t numClasses,
               OptimizerType optimizer = OptimizerType());

  //! Sets the number of classes.
  size_t& NumClasses() { return numClasses; }
  //! Gets the number of classes.
  size_t NumClasses() const { return numClasses; }

  //! Sets the regularization parameter.
  double& Lambda() { return lambda; }
  //! Gets the regularization parameter.
  double Lambda() const { return lambda; }

  //! Gets the intercept term flag.  We can't change this after training.
  bool FitIntercept() const { return fitIntercept; }

  //! Get the model parameters.
  arma::mat& Parameters() { return parameters; }
  //! Get the model parameters.
  const arma::mat& Parameters() const { return parameters; }

  //! Gets the features size of the training data
  size_t FeatureSize() const
  { return fitIntercept ? parameters.n_cols - 1 :
                          parameters.n_cols; }

  /**
   * Serialize the SoftmaxRegression model.
   */
  template<typename Archive>
  void Serialize(Archive& ar, const unsigned int /* version */)
  {
    using mlpack::data::CreateNVP;

    ar & CreateNVP(parameters, "parameters");
    ar & CreateNVP(numClasses, "numClasses");
    ar & CreateNVP(lambda, "lambda");
    ar & CreateNVP(fitIntercept, "fitIntercept");
  }

 private:
  //! Parameters after optimization.
  arma::mat parameters;
  //! Number of classes.
  size_t numClasses;
  //! L2-regularization constant.
  double lambda;
  //! Intercept term flag.
  bool fitIntercept;
};

} // namespace regression
} // namespace mlpack

// Include implementation.
#include "softmax_regression_impl.hpp"

#endif
