/**
 * @file default_param.hpp
 * @author Ryan Curtin
 *
 * Return the default value of a parameter, depending on its type.
 */
#ifndef MLPACK_BINDINGS_CLI_DEFAULT_PARAM_HPP
#define MLPACK_BINDINGS_CLI_DEFAULT_PARAM_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/core/util/param_data.hpp>
#include <mlpack/core/util/is_std_vector.hpp>

namespace mlpack {
namespace bindings {
namespace cli {

/**
 * Return the default value of an option.  This is for regular types.
 */
template<typename T>
std::string DefaultParamImpl(
    const util::ParamData& data,
    const typename boost::disable_if<arma::is_arma_type<T>>::type* = 0,
    const typename boost::disable_if<util::IsStdVector<T>>::type* = 0,
    const typename boost::disable_if<data::HasSerialize<T>>::type* = 0,
    const typename boost::disable_if<std::is_same<T, std::string>>::type* = 0,
    const typename boost::disable_if<std::is_same<T,
        std::tuple<mlpack::data::DatasetInfo, arma::mat>>>::type* = 0);

/**
 * Return the default value of a vector option.
 */
template<typename T>
std::string DefaultParamImpl(
    const util::ParamData& data,
    const typename boost::enable_if<util::IsStdVector<T>>::type* = 0);

/**
 * Return the default value of a string option.
 */
template<typename T>
std::string DefaultParamImpl(
    const util::ParamData& data,
    const typename boost::enable_if<std::is_same<T, std::string>>::type* = 0);

/**
 * Return the default value of a matrix option, a tuple option, a
 * serializable option, or a string option (this returns the default filename,
 * or '' if the default is no file).
 */
template<typename T>
std::string DefaultParamImpl(
    const util::ParamData& data,
    const typename boost::enable_if_c<
        arma::is_arma_type<T>::value ||
        data::HasSerialize<T>::value ||
        std::is_same<T, std::tuple<mlpack::data::DatasetInfo,
                                   arma::mat>>::value>::type* /* junk */ = 0);

/**
 * Return the default value of an option.  This is the function that will be
 * placed into the CLI functionMap.
 */
template<typename T>
void DefaultParam(const util::ParamData& data,
                  const void* /* input */,
                  void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = DefaultParamImpl<T>(data);
}

} // namespace cli
} // namespace bindings
} // namespace mlpack

// Include implementation.
#include "default_param_impl.hpp"

#endif
