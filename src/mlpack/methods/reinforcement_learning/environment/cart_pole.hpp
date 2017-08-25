/**
 * @file cart_pole.hpp
 * @author Shangtong Zhang
 *
 * This file is an implementation of Cart Pole task:
 * https://gym.openai.com/envs/CartPole-v0
 *
 * TODO: provide an option to use dynamics directly from OpenAI gym.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_RL_ENVIRONMENT_CART_POLE_HPP
#define MLPACK_METHODS_RL_ENVIRONMENT_CART_POLE_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace rl {

/**
 * Implementation of Cart Pole task.
 */
class CartPole
{
 public:
  /**
   * Implementation of the state of Cart Pole. Each state is a tuple vector
   * (position, velocity, angle, angular velocity).
   */
  class State
  {
   public:
    /**
     * Construct a state instance.
     */
    State() : data(dimension)
    { /* Nothing to do here. */ }

    /**
     * Construct a state instance from given data.
     *
     * @param data Data for the position, velocity, angle and angular velocity.
     */
    State(const arma::colvec& data) : data(data)
    { /* Nothing to do here */ }

    //! Modify the internal representation of the state.
    arma::colvec& Data() { return data; }

    //! Get the position.
    double Position() const { return data[0]; }
    //! Modify the position.
    double& Position() { return data[0]; }

    //! Get the velocity.
    double Velocity() const { return data[1]; }
    //! Modify the velocity.
    double& Velocity() { return data[1]; }

    //! Get the angle.
    double Angle() const { return data[2]; }
    //! Modify the angle.
    double& Angle() { return data[2]; }

    //! Get the angular velocity.
    double AngularVelocity() const { return data[3]; }
    //! Modify the angular velocity.
    double& AngularVelocity() { return data[3]; }

    //! Encode the state to a column vector.
    const arma::colvec& Encode() const { return data; }

    //! Dimension of the encoded state.
    static constexpr size_t dimension = 4;

   private:
    //! Locally-stored (position, velocity, angle, angular velocity).
    arma::colvec data;
  };

  /**
   * Implementation of action of Cart Pole.
   */
  enum Action
  {
    backward,
    forward,

    // Track the size of the action space.
    size
  };

  /**
   * Construct a Cart Pole instance using the given constants.
   *
   * @param gravity The gravity constant.
   * @param massCart The mass of the cart.
   * @param massPole The mass of the pole.
   * @param length The length of the pole.
   * @param forceMag The magnitude of the applied force.
   * @param tau The time interval.
   * @param thetaThresholdRadians The maximum angle.
   * @param xThreshold The maximum position.
   */
  CartPole(const double gravity = 9.8,
           const double massCart = 1.0,
           const double massPole = 0.1,
           const double length = 0.5,
           const double forceMag = 10.0,
           const double tau = 0.02,
           const double thetaThresholdRadians = 12 * 2 * 3.1416 / 360,
           const double xThreshold = 2.4) :
      gravity(gravity),
      massCart(massCart),
      massPole(massPole),
      totalMass(massCart + massPole),
      length(length),
      poleMassLength(massPole * length),
      forceMag(forceMag),
      tau(tau),
      thetaThresholdRadians(thetaThresholdRadians),
      xThreshold(xThreshold)
  { /* Nothing to do here */ }

  /**
   * Dynamics of Cart Pole instance. Get reward and next state based on current
   * state and current action.
   *
   * @param state The current state.
   * @param action The current action.
   * @param nextState The next state.
   * @return reward, it's always 1.0.
   */
  double Sample(const State& state,
                const Action& action,
                State& nextState) const
  {
    // Calculate acceleration.
    double force = action ? forceMag : -forceMag;
    double cosTheta = std::cos(state.Angle());
    double sinTheta = std::sin(state.Angle());
    double temp = (force + poleMassLength * state.AngularVelocity() *
        state.AngularVelocity() * sinTheta) / totalMass;
    double thetaAcc = (gravity * sinTheta - cosTheta * temp) /
        (length * (4.0 / 3.0 - massPole * cosTheta * cosTheta / totalMass));
    double xAcc = temp - poleMassLength * thetaAcc * cosTheta / totalMass;

    // Update states.
    nextState.Position() = state.Position() + tau * state.Velocity();
    nextState.Velocity() = state.Velocity() + tau * xAcc;
    nextState.Angle() = state.Angle() + tau * state.AngularVelocity();
    nextState.AngularVelocity() = state.AngularVelocity() + tau * thetaAcc;

    return 1.0;
  }

  /**
   * Dynamics of Cart Pole. Get reward based on current state and current
   * action.
   *
   * @param state The current state.
   * @param action The current action.
   * @return reward, it's always 1.0.
   */
  double Sample(const State& state, const Action& action) const
  {
    State nextState;
    return Sample(state, action, nextState);
  }

  /**
   * Initial state representation is randomly generated within [-0.05, 0.05].
   *
   * @return Initial state for each episode.
   */
  State InitialSample() const
  {
    return State((arma::randu<arma::colvec>(4) - 0.5) / 10.0);
  }

  /**
   * Whether given state is a terminal state.
   *
   * @param state The desired state.
   * @return true if state is a terminal state, otherwise false.
   */
  bool IsTerminal(const State& state) const
  {
    return std::abs(state.Position()) > xThreshold ||
        std::abs(state.Angle()) > thetaThresholdRadians;
  }

 private:
  //! Locally-stored gravity.
  double gravity;

  //! Locally-stored mass of the cart.
  double massCart;

  //! Locally-stored mass of the pole.
  double massPole;

  //! Locally-stored total mass.
  double totalMass;

  //! Locally-stored length of the pole.
  double length;

  //! Locally-stored moment of pole.
  double poleMassLength;

  //! Locally-stored magnitude of the applied force.
  double forceMag;

  //! Locally-stored time interval.
  double tau;

  //! Locally-stored maximum angle.
  double thetaThresholdRadians;

  //! Locally-stored maximum position.
  double xThreshold;
};

} // namespace rl
} // namespace mlpack

#endif
