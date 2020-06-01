#pragma once

#include <string>

// Macro to handle erros in VCL
//
// - assert_vcl( condition, error message )
// - assert_vcl_no_msg( condition )
// - error_vcl( error message )
//

namespace vcl
{
#ifndef _WIN32 // no backtrace on Windows
std::string backtrace(int const skip=1);
#endif
[[noreturn]] void call_error(std::string error, std::string message, std::string filename, std::string function_name, int line, std::string const& trace);
}



#ifndef VCL_NO_DEBUG

#ifdef _WIN32  // backtrace and pretty_function not defined on Windows
#define assert_vcl(X, MSG)   {if( (X)==false ) {vcl::call_error(#X,MSG,__FILE__,__func__ ,__LINE__,"No backtrace debug on Windows\n");} }
#define assert_vcl_no_msg(X) {if( (X)==false ) {vcl::call_error(#X,"",__FILE__,__func__ ,__LINE__,"No backtrace debug on Windows\n");} }
#else
#define assert_vcl(X, MSG)   {if( (X)==false ) {vcl::call_error(#X,MSG,__FILE__,__PRETTY_FUNCTION__,__LINE__,vcl::backtrace());} }
#define assert_vcl_no_msg(X) {if( (X)==false ) {vcl::call_error(#X,"",__FILE__,__PRETTY_FUNCTION__,__LINE__,vcl::backtrace());} }
#endif

#else
#define assert_vcl(X, MSG)
#define assert_vcl_no_msg(X)
#endif

#ifdef _WIN32  // backtrace and pretty_function not defined on windows
#define error_vcl(MSG)  {vcl::call_error("",MSG,__FILE__,__func__,__LINE__,"No backtrace debug on Windows\n"); }
#else
#define error_vcl(MSG)  {vcl::call_error("",MSG,__FILE__,__PRETTY_FUNCTION__,__LINE__,vcl::backtrace()); }
#endif




