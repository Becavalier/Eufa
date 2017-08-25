/**
 * @file get_raw_param.hpp
 * @author Ryan Curtin
 *
 * Use template metaprogramming to get the right type of parameter, but without
 * any processing.
 */
#ifndef MLPACK_BINDINGS_CLI_GET_RAW_PARAM_HPP
#define MLPACK_BINDINGS_CLI_GET_RAW_PARAM_HPP

#include <mlpack/prereqs.hpp>
#include "parameter_type.hpp"

namespace mlpack {
namespace bindings {
namespace cli {

/**
 * This overload is called when nothing special needs to happen to the name of
 * the parameter.
 */
template<typename T>
T& GetRawParam(
    util::ParamData& d,
    const typename boost::disable_if<arma::is_arma_type<T>>::type* = 0,
    const typename boost::disable_if<data::HasSerialize<T>>::type* = 0,
    const typename boost::disable_if<std::is_same<T,
        std::tuple<mlpack::data::DatasetInfo, arma::mat>>>::type* = 0)
{
  // No mapping is needed, so just cast it directly.
  return *boost::any_cast<T>(&d.value);
}

/**
 * Return a matrix parameter.
 */
template<typename T>
T& GetRawParam(
    util::ParamData& d,
    const typename boost::enable_if_c<
        arma::is_arma_type<T>::value ||
        std::is_same<T, std::tuple<mlpack::data::DatasetInfo,
                                   arma::mat>>::value ||
        data::HasSerialize<T>::value>::type* = 0)
{
  // Don't load the matrix/model.
  typedef std::tuple<T, std::string> TupleType;
  T& value = std::get<0>(*boost::any_cast<TupleType>(&d.value));
  return value;
}

/**
 * Return a parameter casted to the given type.  Type checking does not happen
 * here!
 *
 * @param d Parameter information.
 * @param input Unused parameter.
 * @param output Place to store pointer to value.
 */
template<typename T>
void GetRawParam(const util::ParamData& d,
                 const void* /* input */,
                 void* output)
{
  // Cast to the correct type.
  *((T**) output) = &GetRawParam<T>(const_cast<util::ParamData&>(d));
}

} // namespace cli
} // namespace bindings
} // namespace mlpack

#endif
