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

#include "TestResults/ETATResultSource.h"

namespace Kinesis
{
namespace TestAutomationTool
{
namespace Backend
{

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |  ATTRIBUTES INITIALIZATION |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

ETATResultSource::TNameValuePair ETATResultSource::sm_arValueName[] =
    {
        std::pair<wxString, ETATResultSource::EnumType>(wxT("E_XmlFile"), ETATResultSource::E_XmlFile),
    };

ETATResultSource::TNameValueMap ETATResultSource::sm_mapValueName(
        ETATResultSource::sm_arValueName ,
        &ETATResultSource::sm_arValueName[0] + sizeof(ETATResultSource::sm_arValueName) / sizeof(ETATResultSource::sm_arValueName[0])
    );

} //namespace Backend
} //namespace TestAutomationTool
} //namespace Kinesis
