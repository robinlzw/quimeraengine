//-------------------------------------------------------------------------------//
//                   TEST AUTOMATION TOOL : LICENSE                              //
//-------------------------------------------------------------------------------//
// This file is part of Test Automation Tool.                                    //
// Test Automation Tool is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation, either version 3 of the License, or             //
// (at your option) any later version.                                           //
//                                                                               //
// Test Automation Tool is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License for more details.                                  //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with Test Automation Tool. If not, see <http://www.gnu.org/licenses/>.  //
//                                                                               //
// This license doesn't force you to put any kind of banner or logo telling      //
// that you are using Test Automation Tool in your project but we would          //
// appreciate if you do so or, at least, if you let us know about that.          //
//                                                                               //
// Enjoy!                                                                        //
//                                                                               //
// Kinesis Team                                                                  //
//-------------------------------------------------------------------------------//

#include "TATCommonTestConfig.h"
#include "CommonConfigDefinitions.h"

#include <boost/date_time.hpp>

#include <iostream>

namespace Kinesis
{
namespace TestAutomationTool
{
namespace Test
{

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |  ATTRIBUTES INITIALIZATION |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

const std::string TATCommonTestConfig::TestConfigFileName("TestConfig.txt");
std::string TATCommonTestConfig::s_strLogFilePath;
std::ofstream TATCommonTestConfig::s_resultsFileStream;


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |       CONSTRUCTORS		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

TATCommonTestConfig::TATCommonTestConfig()
{
}

TATCommonTestConfig::TATCommonTestConfig(const std::string &strTestModuleName, const ETATTestType &testType)
{
    // Loads configuration values from disk
    // -------------------------------------

    char* szCurrentWorkingDirectory = getcwd_t(null_t, 0);
    std::string strCurrentWorkingDirectory(szCurrentWorkingDirectory);
    delete[] szCurrentWorkingDirectory;

    Kinesis::QuimeraEngine::Test::QSimpleConfigLoader config(strCurrentWorkingDirectory + "/" + TATCommonTestConfig::TestConfigFileName);

    if(config.LoadEntries())
    {
        // Entries order in the configuration file
        const int FILE_UNITTEST_RESULTSPATH_ENTRY_POSITION = 0;
        const int FILE_PERFORMANCETEST_RESULTSPATH_ENTRY_POSITION = 1;

        // Depending on the test type, a path is selected to store the results file
        int nEntryToReadResultsPathFrom = 0;

        switch(testType)
        {
        case ETATTestType::E_UnitTest:
            {
                nEntryToReadResultsPathFrom = FILE_UNITTEST_RESULTSPATH_ENTRY_POSITION;
                break;
            }
        case ETATTestType::E_PerformanceTest:
            {
                nEntryToReadResultsPathFrom = FILE_PERFORMANCETEST_RESULTSPATH_ENTRY_POSITION;
                break;
            }
        }

        TATCommonTestConfig::s_strLogFilePath = (config[nEntryToReadResultsPathFrom] + strTestModuleName + "_" + TAT_TEST_CONFIG_NAME + "_" + this->GetCurrentTime() + ".xml");
        TATCommonTestConfig::s_resultsFileStream.open(TATCommonTestConfig::s_strLogFilePath.c_str(), std::ofstream::out);

        // Log configuration
        // -------------------
        // Output file format
        unit_test_log.set_format(XML);
        // Output stream
        unit_test_log.set_stream(TATCommonTestConfig::s_resultsFileStream);
        // Threshold level
        unit_test_log.set_threshold_level(log_successful_tests);
    }
    else
    {
        std::cout << "An error occured when loading the test configuration file (" << TATCommonTestConfig::TestConfigFileName << ").";
    }
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |		  DESTRUCTOR		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

TATCommonTestConfig::~TATCommonTestConfig()
{
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |		    METHODS	    	 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

std::string TATCommonTestConfig::GetCurrentTime()
{
    // Read from http://stackoverflow.com/questions/2612938/simplest-way-to-get-current-time-in-current-timezone-using-boostdate-time

    std::string strResult;

    std::ostringstream stringStream;
    const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_facet *const facet = new boost::posix_time::time_facet("%Y-%m-%d-%H-%M-%S");
    stringStream.imbue(std::locale(stringStream.getloc(), facet));
    stringStream << now;

    strResult = stringStream.str();

    return strResult;
}

} //namespace Test
} //namespace QuimeraEngine
} //namespace Kinesis
