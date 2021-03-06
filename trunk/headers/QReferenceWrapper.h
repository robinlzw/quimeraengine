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

#ifndef __QREFERENCEWRAPPER__
#define __QREFERENCEWRAPPER__

#include "CommonDefinitions.h"


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Common
{

/// <summary>
/// A wrapper for storing a reference.
/// </summary>
/// <typeparam name="T">The type of the reference.</typeparam>
template<class T>
class QReferenceWrapper
{

    // CONSTRUCTORS
    // ---------------
public:

    /// <summary>
    /// Constructor that receives the instance whose reference will be stored.
    /// </summary>
    /// <param name="value">[IN] The instance whose reference will be stored.</param>
    explicit QReferenceWrapper(T& value) : m_value(value)
    {
    }

    
    // METHODS
    // ---------------
public:

    /// <summary>
    /// Casting operator that gets the stored reference.
    /// </summary>
    /// <returns>
    /// The stored reference.
    /// </returns>
    operator T&() const
    {
        return m_value;
    }
    
    /// <summary>
    /// Gets the stored reference.
    /// </summary>
    /// <returns>
    /// The stored reference.
    /// </returns>
    T& Get() const
    {
        return m_value;
    }
    

    // ATTRIBUTES
    // ---------------
private:

    /// <returns>
    /// The wrapped reference.
    /// </returns>
    T& m_value;

};

} //namespace Common
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QREFERENCEWRAPPER__
