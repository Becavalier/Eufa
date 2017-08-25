/**
 * @file print_doc_functions_impl.hpp
 * @author Ryan Curtin
 *
 * This file contains functions useful for printing documentation strings
 * related to Python bindings.
 */
#ifndef MLPACK_BINDINGS_PYTHON_PRINT_DOC_FUNCTIONS_IMPL_HPP
#define MLPACK_BINDINGS_PYTHON_PRINT_DOC_FUNCTIONS_IMPL_HPP

#include <mlpack/core/util/hyphenate_string.hpp>

namespace mlpack {
namespace bindings {
namespace python {

/**
 * Given a parameter type, print the corresponding value.
 */
template<typename T>
inline std::string PrintValue(const T& value, bool quotes)
{
  std::ostringstream oss;
  if (quotes)
    oss << "'";
  oss << value;
  if (quotes)
    oss << "'";
  return oss.str();
}

// Special overload for booleans.
template<>
inline std::string PrintValue(const bool& value, bool quotes)
{
  if (quotes && value)
    return "'True'";
  else if (quotes && !value)
    return "'False'";
  else if (!quotes && value)
    return "True";
  else
    return "False";
}

// Recursion base case.
std::string PrintInputOptions() { return ""; }

/**
 * Print an input option.  This will throw an exception if the parameter does
 * not exist in CLI.  For a parameter 'x' with value '5', this will print
 * something like x=5.
 */
template<typename T, typename... Args>
std::string PrintInputOptions(const std::string& paramName,
                              const T& value,
                              Args... args)
{
  // See if this is part of the program.
  std::string result = "";
  if (CLI::Parameters().count(paramName) > 0)
  {
    const util::ParamData& d = CLI::Parameters()[paramName];
    if (d.input)
    {
      // Print the input option.
      std::ostringstream oss;
      oss << paramName << "=" <<
          PrintValue(value, d.tname == TYPENAME(std::string));
      result = oss.str();
    }
  }
  else
  {
    // Unknown parameter!
    throw std::runtime_error("Unknown parameter '" + paramName + "' " +
        "encountered while assembling documentation!  Check PROGRAM_INFO() " +
        "declaration.");
  }

  // Continue recursion.
  std::string rest = PrintInputOptions(args...);
  if (rest != "" && result != "")
    result += ", " + rest;
  else if (result == "")
    result = rest;

  return result;
}

// Recursion base case.
inline std::string PrintOutputOptions() { return ""; }

template<typename T, typename... Args>
std::string PrintOutputOptions(const std::string& paramName,
                               const T& value,
                               Args... args)
{
  // See if this is part of the program.
  std::string result = "";
  if (CLI::Parameters().count(paramName) > 0)
  {
    const util::ParamData& d = CLI::Parameters()[paramName];
    if (!d.input)
    {
      // Print a new line for the output option.
      std::ostringstream oss;
      oss << ">>> " << value << " = output['" << paramName << "']";
      result = oss.str();
    }
  }
  else
  {
    // Unknown parameter!
    throw std::runtime_error("Unknown parameter '" + paramName + "' " +
        "encountered while assembling documentation!  Check PROGRAM_INFO() " +
        "declaration.");
  }

  // Continue recursion.
  std::string rest = PrintOutputOptions(args...);
  if (rest != "" && result != "")
    result += '\n';
  result += rest;

  return result;
}

/**
 * Given a name of a binding and a variable number of arguments (and their
 * contents), print the corresponding function call.
 */
template<typename... Args>
std::string ProgramCall(const std::string& programName, Args... args)
{
  std::ostringstream oss;
  oss << ">>> " << programName << "(";

  // Now process each input option.
  oss << PrintInputOptions(args...);
  oss << ")";

  std::string call = oss.str();
  oss.str(""); // Reset it.

  // Now process each output option.
  oss << PrintOutputOptions(args...);
  if (oss.str() == "")
    return util::HyphenateString(call, 2);
  else
    return util::HyphenateString(call, 2) + "\n" + oss.str();
}

/**
 * Given the name of a model, print it.  Here we do not need to modify anything.
 */
inline std::string PrintModel(const std::string& modelName)
{
  return "'" + modelName + "'";
}

/**
 * Given the name of a matrix, print it.  Here we do not need to modify
 * anything.
 */
inline std::string PrintDataset(const std::string& datasetName)
{
  return "'" + datasetName + "'";
}

/**
 * Given the name of a binding, print its invocation.
 */
inline std::string ProgramCall(const std::string& programName)
{
  return ">>> " + programName + "(";
}

/**
 * Print any closing call to a program.  For a Python binding this is a closing
 * brace.
 */
inline std::string ProgramCallClose()
{
  return ")";
}

/**
 * Given the parameter name, determine what it would actually be when passed to
 * the command line.
 */
inline std::string ParamString(const std::string& paramName)
{
  // For a Python binding we don't need to know the type.
  return "'" + paramName + "'";
}

/**
 * Given the parameter name and an argument, return what should be written as
 * documentation when referencing that argument.
 */
template<typename T>
inline std::string ParamString(const std::string& paramName, const T& value)
{
  std::ostringstream oss;
  oss << paramName << "=" << value;
  return oss.str();
}

} // namespace python
} // namespace bindings
} // namespace mlpack

#endif
