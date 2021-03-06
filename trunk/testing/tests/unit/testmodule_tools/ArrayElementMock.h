//-------------------------------------------------------------------------------//
//                         QUIMERA ENGINE : LICENSE                              //
//-------------------------------------------------------------------------------//
// This file is part of Quimera Engine.                                          //
// Quimera Engine is free software: you can redistribute it and/or modify        //
// it under the terms of the Lesser GNU General Public License as published by   //
// the Free Software Foundation, either version 3 of the License, or             //
// (at your option) any later version.                                           //
//                                                                               //
// Quimera Engine is distributed in the hope that it will be useful,             //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// Lesser GNU General Public License for more details.                           //
//                                                                               //
// You should have received a copy of the Lesser GNU General Public License      //
// along with Quimera Engine. If not, see <http://www.gnu.org/licenses/>.        //
//                                                                               //
// This license doesn't force you to put any kind of banner or logo telling      //
// that you are using Quimera Engine in your project but we would appreciate     //
// if you do so or, at least, if you let us know about that.                     //
//                                                                               //
// Enjoy!                                                                        //
//                                                                               //
// Kinesis Team                                                                  //
//-------------------------------------------------------------------------------//

#ifndef __ARRAYELEMENTMOCK__
#define __ARRAYELEMENTMOCK__

using Kinesis::QuimeraEngine::Common::DataTypes::u64_q;
using Kinesis::QuimeraEngine::Common::DataTypes::i16_q;

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools    
{
namespace Containers
{
namespace Test
{

/// <summary>
/// Class to be used to ensure that the copy constructor, assignment operator and destructor are called
/// properly from QArrayFixed.
/// </summary>
class ArrayElementMock
{
	// CONSTRUCTORS
	// ---------------
public:
	// Necessary for testing
    ArrayElementMock(u64_q uValue) : 
            m_uValue(uValue),
            m_bCallToCopyConstructor(false),
            m_bCallToOperatorAssignment(false)

    {
        ArrayElementMock::m_nNumberOfAllocatedObjects++;
    }
    
    // Copy constructor necessary for testing
    ArrayElementMock(const ArrayElementMock& arrayElementMock)
    {
        this->m_uValue = arrayElementMock.m_uValue;
        this->m_bCallToCopyConstructor = true;
        this->m_bCallToOperatorAssignment = false;
        ArrayElementMock::m_nNumberOfAllocatedObjects++;
    }
    
    // Destructor necessary for testing
    ~ArrayElementMock()
    {
        ArrayElementMock::m_nNumberOfAllocatedObjects--;
    }

	// METHODS
	// ---------------
public:

    // Assignment operator necessary for testing
    ArrayElementMock& operator= (const ArrayElementMock& arrayElementMock)
    {
        this->m_uValue = arrayElementMock.m_uValue;
        this->m_bCallToOperatorAssignment = true;
        return *this;
    }

    // Exposed member.
    bool GetCallToCopyConstructor() const
    {
        return m_bCallToCopyConstructor;
    }

    // Exposed member.
    bool GetCallToOperatorAssignment() const
    {
        return m_bCallToOperatorAssignment;
    }

    // Exposed member.
    static i16_q GetNumberOfAllocatedObjects() 
    {
        return m_nNumberOfAllocatedObjects;
    }
    
    // ATTRIBUTES
	// ---------------

private:

    // Number of allocated objects
    static i16_q m_nNumberOfAllocatedObjects;

    // Stored value.
    u64_q m_uValue;

    // True if a call to the copy constructor has been done.
    bool m_bCallToCopyConstructor;

    // True if a call to the assignment operator has been done.
    bool m_bCallToOperatorAssignment; 

};

} //namespace Test
} //namespace Containers
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __ARRAYELEMENTMOCK__
