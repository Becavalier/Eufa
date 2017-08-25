/**
 * @file mountain_car.hpp
 * @author Shangtong Zhang
 *
 * This file is an implementation of Mountain Car task:
 * https://gym.openai.com/envs/MountainCar-v0
 *
 * TODO: provide an option to use dynamics directly from OpenAI gym.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#ifndef MLPACK_METHODS_RL_ENVIRONMENT_MOUNTAIN_CAR_HPP
#define MLPACK_METHODS_RL_ENVIRONMENT_MOUNTAIN_CAR_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace rl {

/**
 * Implementation of Mountain Car task.
 */
class MountainCar
{
 public:
  /**
   * Implementation of state of Mountain Car. Each state is a
   * (velocity, position) vector.
   */
  class State
  {
   public:
    /**
     * Construct a state instance.
     */
    State(): data(dimension, arma::fill::zeros)
    { /* Nothing to do here. */ }

    /**
     * Construct a state based on the given data.
     *
     * @param data Data for the velocityand position.
     */
    State(const arma::colvec& data): data(data)
    { /* Nothing to do here. */ }

    //! Modify the internal representation of the state.
    arma::colvec& Data() { return data; }

    //! Get the velocity.
    double Velocity() const { return data[0]; }
    //! Modify the velocity.
    double& Velocity() { return data[0]; }

    //! Get the position.
    double Position() const { return data[1]; }
    //! Modify the position.
    double& Position() { return data[1]; }

    //! Encode the state to a column vector.
    const arma::colvec& Encode() const { return data; }

    //! Dimension of the encoded state.
    static constexpr size_t dimension = 2;

   private:
    //! Locally-stored velocity and position vector.
    arma::colvec data;
  };

  /**
   * Implementation of action of Mountain Car.
   */
  enum Action
  {
    backward,
    stop,
    forward,

    //! Track the size of the action space.
    size
  };

  /**
   * Construct a Mountain Car instance using the given constant.
   *
   * @param positionMin Minimum legal position.
   * @param positionMax Maximum legal position.
   * @param velocityMin Minimum legal velocity.
   * @param velocityMax Maximum legal velocity.
   */
  MountainCar(const double positionMin = -1.2,
              const double positionMax = 0.5,
              const double velocityMin = -0.07,
              const double velocityMax = 0.07) :
      positionMin(positionMin),
      positionMax(positionMax),
      velocityMin(velocityMin),
      velocityMax(velocityMax)
  { /* Nothing to do here */ }

  /**
   * Dynamics of Mountain Car. Get reward and next state based on current state
   * and current action.
   *
   * @param state The current state.
   * @param action The current action.
   * @param nextState The next state.
   * @return reward, it's always -1.0.
   */
  double Sample(const State& state,
                const Action& action,
                State& nextState) const
  {
    // Calculate acceleration.
    int direction = action - 1;
    nextState.Velocity() = state.Velocity() + 0.001 * direction - 0.0025 *
        std::cos(3 * state.Position());
    nextState.Velocity() = std::min(
        std::max(nextState.Velocity(), velocityMin), velocityMax);

    // Update states.
    nextState.Position() = state.Position() + nextState.Velocity();
    nextState.Position() = std::min(
        std::max(nextState.Position(), positionMin), positionMax);

    if (std::abs(nextState.Position() - positionMin) <= 1e-5)
    {
      nextState.Velocity() = 0.0;
    }

    return -1.0;
  }

  /**
   * Dynamics of Mountain Car. Get reward based on current state and current
   * action.
   *
   * @param state The current state.
   * @param action The current action.
   * @return reward, it's always -1.0.
   */
  double Sample(const State& state, const Action& action) const
  {
    State nextState;
    return Sample(state, action, nextState);
  }

  /**
   * Initial position is randomly generated within [-0.6, -0.4].
   * Initial velocity is 0.
   *
   * @return Initial state for each episode.
   */
  State InitialSample() const
  {
    State state;
    state.Velocity() = 0.0;
    state.Position() = arma::as_scalar(arma::randu(1)) * 0.2 - 0.6;
    return state;
  }

  /**
   * Whether given state is a terminal state.
   *
   * @param state desired state.
   * @return true if state is a terminal state, otherwise false.
   */
  bool IsTerminal(const State& state) const
  {
    return std::abs(state.Position() - positionMax) <= 1e-5;
  }

 private:
  //! Locally-stored minimum legal position.
  double positionMin;

  //! Locally-stored maximum legal position.
  double positionMax;

  //! Locally-stored minimum legal velocity.
  double velocityMin;

  //! Locally-stored maximum legal velocity.
  double velocityMax;
};

} // namespace rl
} // namespace mlpack

#endif
