/**
 * @file get_param.hpp
 * @author Ryan Curtin
 *
 * Use template metaprogramming to get the right type of parameter.
 */
#ifndef MLPACK_BINDINGS_CLI_GET_PARAM_HPP
#define MLPACK_BINDINGS_CLI_GET_PARAM_HPP

#include <mlpack/prereqs.hpp>
#include "parameter_type.hpp"

namespace mlpack {
namespace bindings {
namespace cli {

/**
 * This overload is called when nothing special needs to happen to the name of
 * the parameter.
 *
 * @param d ParamData object to get parameter value from.
 */
template<typename T>
T& GetParam(
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
 *
 * @param d ParamData object to get parameter value from.
 */
template<typename T>
T& GetParam(
    util::ParamData& d,
    const typename boost::enable_if<arma::is_arma_type<T>>::type* = 0)
{
  // If the matrix is an input matrix, we have to load the matrix.  'value'
  // contains the filename.  It's possible we could load empty matrices many
  // times, but I am not bothered by that---it shouldn't be something that
  // happens.
  typedef std::tuple<T, typename ParameterType<T>::type> TupleType;
  TupleType& tuple = *boost::any_cast<TupleType>(&d.value);
  const std::string& value = std::get<1>(tuple);
  T& matrix = std::get<0>(tuple);
  if (d.input && !d.loaded)
  {
    // Call correct data::Load() function.
    if (arma::is_Row<T>::value || arma::is_Col<T>::value)
      data::Load(value, matrix, true);
    else
      data::Load(value, matrix, true, !d.noTranspose);
    d.loaded = true;
  }

  return matrix;
}

/**
 * Return a matrix/dataset info parameter.
 *
 * @param d ParamData object to get parameter value from.
 */
template<typename T>
T& GetParam(
    util::ParamData& d,
    const typename boost::enable_if<std::is_same<T,
        std::tuple<mlpack::data::DatasetInfo, arma::mat>>>::type* = 0)
{
  // If this is an input parameter, we need to load both the matrix and the
  // dataset info.
  typedef std::tuple<T, std::string> TupleType;
  TupleType* tuple = boost::any_cast<TupleType>(&d.value);
  const std::string& value = std::get<1>(*tuple);
  T& t = std::get<0>(*tuple);
  if (d.input && !d.loaded)
  {
    data::Load(value, std::get<1>(t), std::get<0>(t), true, !d.noTranspose);
    d.loaded = true;
  }

  return t;
}

/**
 * Return a serializable object.
 *
 * @param d ParamData object to get parameter value from.
 */
template<typename T>
T& GetParam(
    util::ParamData& d,
    const typename boost::enable_if<data::HasSerialize<T>>::type* = 0)
{
  // If the model is an input model, we have to load it from file.  'value'
  // contains the filename.
  typedef std::tuple<T, std::string> TupleType;
  TupleType* tuple = boost::any_cast<TupleType>(&d.value);
  const std::string& value = std::get<1>(*tuple);
  T& model = std::get<0>(*tuple);
  if (d.input && !d.loaded)
  {
    data::Load(value, "model", model, true);
    d.loaded = true;
  }

  return model;
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
void GetParam(const util::ParamData& d, const void* /* input */, void* output)
{
  // Cast to the correct type.
  *((T**) output) = &GetParam<T>(const_cast<util::ParamData&>(d));
}

} // namespace cli
} // namespace bindings
} // namespace mlpack

#endif
