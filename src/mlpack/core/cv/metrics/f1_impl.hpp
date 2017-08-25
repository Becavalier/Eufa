/**
 * @file f1_impl.hpp
 * @author Kirill Mishchenko
 *
 * Implementation of the class F1.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_CV_METRICS_F1_IMPL_HPP
#define MLPACK_CORE_CV_METRICS_F1_IMPL_HPP

#include <mlpack/core/cv/metrics/accuracy.hpp>
#include <mlpack/core/cv/metrics/facilities.hpp>

namespace mlpack {
namespace cv {

template<AverageStrategy AS, size_t PC /* PositiveClass */>
template<typename MLAlgorithm, typename DataType>
double F1<AS, PC>::Evaluate(MLAlgorithm& model,
                            const DataType& data,
                            const arma::Row<size_t>& labels)
{
  return Evaluate<AS>(model, data, labels);
}

template<AverageStrategy AS, size_t PC /* PositiveClass */>
template<AverageStrategy _AS, typename MLAlgorithm, typename DataType, typename>
double F1<AS, PC>::Evaluate(MLAlgorithm& model,
                            const DataType& data,
                            const arma::Row<size_t>& labels)
{
  AssertSizes(data, labels, "F1<Binary>::Evaluate()");

  arma::Row<size_t> predictedLabels;
  model.Classify(data, predictedLabels);

  size_t tp = arma::sum((labels == PC) % (predictedLabels == PC));
  size_t numberOfPositivePredictions = arma::sum(predictedLabels == PC);
  size_t numberOfPositiveClassInstances = arma::sum(labels == PC);

  double precision = double(tp) / numberOfPositivePredictions;
  double recall = double(tp) / numberOfPositiveClassInstances;

  return (precision + recall == 0.0) ? 0.0 :
      2.0 * precision * recall / (precision + recall);
}

template<AverageStrategy AS, size_t PC /* PositiveClass */>
template<AverageStrategy _AS, typename MLAlgorithm, typename DataType, typename,
    typename>
double F1<AS, PC>::Evaluate(MLAlgorithm& model,
                            const DataType& data,
                            const arma::Row<size_t>& labels)
{
  AssertSizes(data, labels, "F1<Micro>::Evaluate()");

  // Microaveraged F1 is really the same as microaveraged precision and
  // microaveraged recall, which are in turn the same as accuracy.
  return Accuracy::Evaluate(model, data, labels);
}

template<AverageStrategy AS, size_t PC /* PositiveClass */>
template<AverageStrategy _AS, typename MLAlgorithm, typename DataType, typename,
    typename, typename>
double F1<AS, PC>::Evaluate(MLAlgorithm& model,
                            const DataType& data,
                            const arma::Row<size_t>& labels)
{
  AssertSizes(data, labels, "F1<Macro>::Evaluate()");

  arma::Row<size_t> predictedLabels;
  model.Classify(data, predictedLabels);

  size_t numClasses = arma::max(labels) + 1;

  arma::vec f1s = arma::vec(numClasses);
  for (size_t c = 0; c < numClasses; ++c)
  {
    size_t tp = arma::sum((labels == c) % (predictedLabels == c));
    size_t numberOfPositivePredictions = arma::sum(predictedLabels == c);
    size_t numberOfClassInstances = arma::sum(labels == c);

    double precision = double(tp) / numberOfPositivePredictions;
    double recall = double(tp) / numberOfClassInstances;
    f1s(c) = (precision + recall == 0.0) ? 0.0 :
        2.0 * precision * recall / (precision + recall);
  }

  return arma::mean(f1s);
}

} // namespace cv
} // namespace mlpack

#endif
