/**
 * @file param_data.hpp
 * @author Ryan Curtin
 *
 * This defines the structure that holds information for each command-line
 * parameter, as well as utility functions it is used with.
 */
#ifndef MLPACK_CORE_UTIL_PARAM_DATA_HPP
#define MLPACK_CORE_UTIL_PARAM_DATA_HPP

#include <mlpack/prereqs.hpp>
#include <boost/any.hpp>

/**
 * The TYPENAME macro is used internally to convert a type into a string.
 */
#define TYPENAME(x) (std::string(typeid(x).name()))

namespace mlpack {
namespace data {

class IncrementPolicy;

template<typename PolicyType>
class DatasetMapper;

using DatasetInfo = DatasetMapper<IncrementPolicy>;

} // namespace data
} // namespace mlpack

namespace mlpack {
namespace util {

/**
 * This structure holds all of the information about a single parameter,
 * including its value (which is set when ParseCommandLine() is called).  It
 * does not hold any information about whether or not it was passed---that is
 * handled elsewhere.  A ParamData struct is only useful in order to get
 * "static" information about a parameter.  Note that some parameter types have
 * internal types but also different types that are used by
 * boost::program_options (specifically, matrix and model types map to strings).
 *
 * This structure is somewhat unwieldy and is likely to be refactored at some
 * point in the future, but for now it does the job fine.
 */
struct ParamData
{
  //! Name of this parameter.  This is the name used for HasParam() and
  //! GetParam().
  std::string name;
  //! Description of this parameter, if any.
  std::string desc;
  //! Type information of this parameter.  Note that this is TYPENAME() of the
  //! user-visible parameter type, not whatever is given by ParameterType<>.
  std::string tname;
  //! Alias for this parameter.
  char alias;
  //! True if the option was passed to the program.  Note that wasPassed may be
  //! set by either ParseCommandLine() or SetPassed().
  bool wasPassed;
  //! True if this is a matrix that should not be transposed.  Ignored if the
  //! parameter is not a matrix.
  bool noTranspose;
  //! True if this option is required.
  bool required;
  //! True if this option is an input option (otherwise, it is output).
  bool input;
  //! If this is an input parameter that needs extra loading, this indicates
  //! whether or not it has been loaded.
  bool loaded;
  //! If this should be preserved across different settings (i.e. if it should
  //! exist for every binding), this should be set to true.
  bool persistent;
  //! The actual value that is held.  If the user has passed a different type,
  //! this may be a tuple containing multiple values.
  boost::any value;
  //! The true name of the type, as it would be written in C++.
  std::string cppType;
};

} // namespace util
} // namespace mlpack

#endif
