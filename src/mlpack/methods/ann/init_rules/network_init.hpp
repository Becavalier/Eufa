/**
 * @file network_init.hpp
 * @author Marcus Edel
 *
 * Intialization of a given network with a given initialization rule
 * e.g. GaussianInitialization.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_INIT_RULES_NETWORK_INIT_HPP
#define MLPACK_METHODS_ANN_INIT_RULES_NETWORK_INIT_HPP

#include <mlpack/prereqs.hpp>

#include "../visitor/reset_visitor.hpp"
#include "../visitor/weight_size_visitor.hpp"
#include "../visitor/weight_set_visitor.hpp"
#include "init_rules_traits.hpp"

#include <mlpack/methods/ann/layer/layer_types.hpp>

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * This class is used to initialize the network with the given initialization
 * rule.
 */
template<typename InitializationRuleType>
class NetworkInitialization
{
 public:
  /**
   * Use the given initialization rule to initialize the specified network.
   *
   * @param initializeRule Rule to initialize the given network.
   */
  NetworkInitialization(
      const InitializationRuleType& initializeRule = InitializationRuleType()) :
      initializeRule(initializeRule)
  {
    // Nothing to do here.
  }

  /**
   * Initialize the specified network and store the results in the given
   * parameter.
   *
   * @param network Network that should be initialized.
   * @param parameter The network parameter.
   */
  void Initialize(const std::vector<LayerTypes>& network, arma::mat& parameter)
  {
    // Determine the number of parameter/weights of the given network.
    size_t weights = 0;
    for (size_t i = 0; i < network.size(); ++i)
      weights += boost::apply_visitor(weightSizeVisitor, network[i]);
    parameter.set_size(weights, 1);

    // Initialize the network layer by layer or the complete network.
    if (ann::InitTraits<InitializationRuleType>::UseLayer)
    {
      for (size_t i = 0, offset = 0; i < network.size(); ++i)
      {
        // Initialize the layer with the specified parameter/weight
        // initialization rule.
        const size_t weight = boost::apply_visitor(weightSizeVisitor,
            network[i]);
        arma::mat tmp = arma::mat(parameter.memptr() + offset,
            weight, 1, false, false);
        initializeRule.Initialize(tmp, tmp.n_elem, 1);

        // Increase the parameter/weight offset for the next layer.
        offset += weight;
      }
    }
    else
    {
      initializeRule.Initialize(parameter, parameter.n_elem, 1);
    }

    // Note: We can't merge the for loop into the for loop above because
    // WeightSetVisitor also sets the parameter/weights of the inner modules.
    // Inner Modules are held by the parent module e.g. the concat module can
    // hold various other modules.
    for (size_t i = 0, offset = 0; i < network.size(); ++i)
    {
      offset += boost::apply_visitor(WeightSetVisitor(std::move(parameter),
          offset), network[i]);

      boost::apply_visitor(resetVisitor, network[i]);
    }
  }

 private:
  //! Instantiated InitializationRule object for initializing the network
  //! parameter.
  InitializationRuleType initializeRule;

  //! Locally-stored reset visitor.
  ResetVisitor resetVisitor;

  //! Locally-stored weight size visitor.
  WeightSizeVisitor weightSizeVisitor;
}; // class NetworkInitialization

} // namespace ann
} // namespace mlpack

#endif
