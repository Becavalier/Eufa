/**
 * @file get_printable_param_value.hpp
 * @author Ryan Curtin
 *
 * Given a parameter value, print what the user might actually specify on the
 * command line.  Basically this adds ".csv" to types where data must be loaded.
 */
#ifndef MLPACK_BINDINGS_CLI_GET_PRINTABLE_PARAM_VALUE_HPP
#define MLPACK_BINDINGS_CLI_GET_PRINTABLE_PARAM_VALUE_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/core/util/param_data.hpp>

namespace mlpack {
namespace bindings {
namespace cli {

/**
 * Get the parameter name for a type that has no special handling.
 */
template<typename T>
std::string GetPrintableParamValue(
    const util::ParamData& data,
    const std::string& value,
    const typename boost::disable_if<arma::is_arma_type<T>>::type* = 0,
    const typename boost::disable_if<data::HasSerialize<T>>::type* = 0,
    const typename boost::disable_if<std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>>::type* = 0);

/**
 * Get the parameter name for a matrix type (where the user has to pass the file
 * that holds the matrix).
 */
template<typename T>
std::string GetPrintableParamValue(
    const util::ParamData& data,
    const std::string& value,
    const typename boost::enable_if<arma::is_arma_type<T>>::type* = 0);

/**
 * Get the parameter name for a serializable model type (where the user has to
 * pass the file that holds the matrix).
 */
template<typename T>
std::string GetPrintableParamValue(
    const util::ParamData& data,
    const std::string& value,
    const typename boost::enable_if<data::HasSerialize<T>>::type* = 0);

/**
 * Get the parameter name for a mapped matrix type (where the user has to pass
 * the file that holds the matrix).
 */
template<typename T>
std::string GetPrintableParamValue(
    const util::ParamData& data,
    const std::string& value,
    const typename boost::enable_if<std::is_same<T,
        std::tuple<data::DatasetInfo, arma::mat>>>::type* = 0);

/**
 * Get the parameter's name as seen by the user.
 */
template<typename T>
void GetPrintableParamValue(
    const util::ParamData& d,
    const void* input,
    void* output)
{
  *((std::string*) output) = GetPrintableParamValue<T>(d,
      *((std::string*) input));
}

} // namespace cli
} // namespace bindings
} // namespace mlpack

// Include implementation.
#include "get_printable_param_value_impl.hpp"

#endif
