/**
 * @file string_type_param_impl.hpp
 * @author Ryan Curtin
 *
 * Implementations of StringTypeParam().
 */
#ifndef MLPACK_BINDINGS_CLI_STRING_TYPE_PARAM_IMPL_HPP
#define MLPACK_BINDINGS_CLI_STRING_TYPE_PARAM_IMPL_HPP

#include "string_type_param.hpp"

namespace mlpack {
namespace bindings {
namespace cli {

/**
 * Return a string containing the type of the parameter.
 */
template<typename T>
std::string StringTypeParamImpl(
    const typename boost::disable_if<arma::is_arma_type<T>>::type* /* junk */,
    const typename boost::disable_if<util::IsStdVector<T>>::type* /* junk */,
    const typename boost::disable_if<data::HasSerialize<T>>::type* /* junk */)
{
  // Don't know what type this is.
  return "unknown";
}

/**
 * Return a string containing the type of the parameter, for vector options.
 */
template<typename T>
std::string StringTypeParamImpl(
    const typename boost::enable_if<util::IsStdVector<T>>::type* /* junk */)
{
  return "vector";
}

/**
 * Return a string containing the type of the parameter, for matrix options.
 */
template<typename T>
std::string StringTypeParamImpl(
    const typename boost::enable_if<arma::is_arma_type<T>>::type* /* junk */)
{
  return "string";
}

/**
 * Return a string containing the type of the parameter,
 */
template<typename T>
std::string StringTypeParamImpl(
    const typename boost::enable_if<data::HasSerialize<T>>::type* /* junk */)
{
  return "string";
}

//! Return "int".
template<>
inline void StringTypeParam<int>(const util::ParamData& /* data */,
                                 const void* /* input */,
                                 void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = "int";
}

//! Return "bool".
template<>
inline void StringTypeParam<bool>(const util::ParamData& /* data */,
                                  const void* /* input */,
                                  void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = "bool";
}

//! Return "string".
template<>
inline void StringTypeParam<std::string>(const util::ParamData& /* data */,
                                         const void* /* input */,
                                         void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = "string";
}

//! Return "float".
template<>
inline void StringTypeParam<float>(const util::ParamData& /* data */,
                                   const void* /* input */,
                                   void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = "float";
}

//! Return "double".
template<>
inline void StringTypeParam<double>(const util::ParamData& /* data */,
                                    const void* /* input */,
                                    void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = "double";
}

//! Return "string";
template<>
inline void StringTypeParam<std::tuple<mlpack::data::DatasetInfo, arma::mat>>(
    const util::ParamData& /* data */,
    const void* /* input */,
    void* output)
{
  std::string* outstr = (std::string*) output;
  *outstr = "string";
}

} // namespace cli
} // namespace bindings
} // namespace mlpack

#endif
