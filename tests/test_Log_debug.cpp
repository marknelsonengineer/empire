///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Test setting `MIN_LOG_SEVERITY` to #LOG_SEVERITY_DEBUG and test UTF-8
/// compatability in several languages
///
/// @file      test_Log_debug.cpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings
/// @NOLINTBEGIN( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers ): Tests will have magic numbers

#include <boost/test/unit_test.hpp>

#include "../src/lib/LogSeverity.hpp"  // For LOG_SEVERITY #defines

/// The name of the module for logging purposes
/// @NOLINTNEXTLINE( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): A `char[]` array is acceptable here
[[maybe_unused]] alignas(32) static constinit const char LOG_MODULE[32] { "test_Log_debug" };

/// Logs at and above `MIN_LOG_SEVERITY` will be available.  Logs below
/// `MIN_LOG_SEVERITY` will not be compiled into the source file.
#define MIN_LOG_SEVERITY LOG_SEVERITY_DEBUG
#include "../src/lib/Log.hpp"


/* ****************************************************************************
   White Box Test Declarations

   These declarations may contain duplicate code or code that needs to be in
   sync with the code under test.  Because these are white box tests, it's on
   the tester to ensure the code is in sync.                                 */

namespace empire {
extern LogEntry& logPeek();
} // namespace empire

/* ***************************************************************************/


using namespace empire;

BOOST_AUTO_TEST_SUITE( Log )

BOOST_AUTO_TEST_CASE( Log_level_debug ) {
   LOG_TEST( "測試日誌條目 %s", "普通話 中文" );  // Mandarin Chinese
   BOOST_CHECK_NE( logPeek().msg, "測試日誌條目 普通話 中文" );

   LOG_TRACE( "लॉग इन प्रविष्टि का परीक्षण करें %s", "हिंदी" );  // Hindi
   BOOST_CHECK_NE( logPeek().msg, "लॉग इन प्रविष्टि का परीक्षण करें हिंदी" );

   LOG_DEBUG( "Test Log Entrada de depuración en %s", "español" );  // Spanish
   BOOST_CHECK_EQUAL( logPeek().msg, "Test Log Entrada de depuración en español" );

   LOG_INFO( "Entrée d'informations sur le journal de test en %s", "français" );  // French
   BOOST_CHECK_EQUAL( logPeek().msg, "Entrée d'informations sur le journal de test en français" );

   LOG_WARN( "اختبار الدخول إلى التحذير من دخول %s", "عربي" );  // Arabic
   BOOST_CHECK_EQUAL( logPeek().msg, "اختبار الدخول إلى التحذير من دخول عربي" );

   LOG_ERROR( "পরীক্ষা ত্রুটি লগ এন্ট্রি %s", "বাংলা" );  // Bengali
   BOOST_CHECK_EQUAL( logPeek().msg, "পরীক্ষা ত্রুটি লগ এন্ট্রি বাংলা" );

   LOG_FATAL( "Журнал испытаний Фатальная запись в %s", "Русский" );  // Russian
   BOOST_CHECK_EQUAL( logPeek().msg, "Журнал испытаний Фатальная запись в Русский" );
}


BOOST_AUTO_TEST_SUITE_END()
// NOLINTEND( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers )
/// @endcond
