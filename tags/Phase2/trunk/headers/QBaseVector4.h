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

#ifndef __QBASEVECTOR4__
#define __QBASEVECTOR4__

#include "DataTypesDefinitions.h"
#include "ToolsDefinitions.h"

using Kinesis::QuimeraEngine::Common::DataTypes::float_q;
using Kinesis::QuimeraEngine::Common::DataTypes::vf32_q;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{

/// <summary>
/// A vector with four components: X, Y, Z and W.
/// </summary>
/// <remarks>
///    A vector is a geometric object that has both a magnitude (or length) and a direction.
/// The fourth component may equal 0, if the vector represents a normal in 3D space, or 1, if it represents a point.
/// </remarks>
class QE_LAYER_TOOLS_SYMBOLS QBaseVector4
{
    // CONSTRUCTORS
    // ---------------
public:

    /// <summary>
    /// Default constructor.
    /// </summary>
    /// <remarks>
    ///    By default, all the components are set to zero.
    /// </remarks>
    QBaseVector4();

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="vVector">[IN] The vector whose components are to be copied.</param>
    QBaseVector4(const QBaseVector4 &vVector);

    /// <summary>
    /// Constructor that receives the value of every vector's component.
    /// </summary>
    /// <param name="fValueX">[IN] The value for X component.</param>
    /// <param name="fValueY">[IN] The value for Y component.</param>
    /// <param name="fValueZ">[IN] The value for Z component.</param>
    /// <param name="fValueW">[IN] The value for W component.</param>
    QBaseVector4(const float_q &fValueX, const float_q &fValueY, const float_q &fValueZ, const float_q &fValueW);

    /// <summary>
    /// Constructor from a single value for all the vector's components.
    /// </summary>
    /// <param name="fValueAll">[IN] The value for all components.</param>
    explicit QBaseVector4(const float_q &fValueAll);

    /// <summary>
    /// Constructor that receives an array of scalars.
    /// </summary>
    /// <remarks>
    /// The array must contain, at least, four elements. Only the first four elements will be considered; the rest will be ignored.
    /// </remarks>
    /// <param name="arValues">[IN] An array of scalars. It must contain, at least, four elements. If the pointer is null, the behavior  
    /// is undefined.</param>
    explicit QBaseVector4(const float_q* arValues);

    /// <summary>
    /// Constructor that receives a pack of four scalars.
    /// </summary>
    /// <param name="value">[IN] 4x32 packed floating point value containing the three components.</param>
    explicit QBaseVector4(const vf32_q &value);


    // METHODS
    // ---------------
public:

    /// <summary>
    /// Checks if two vectors are equal.
    /// </summary>
    /// <param name="vVector">[IN] The vector to which to compare.</param>
    /// <returns>
    /// True if vectors are the same; False otherwise.
    /// </returns>
    bool operator==(const QBaseVector4 &vVector) const;

    /// <summary>
    /// Checks if two vectors are not equal.
    /// </summary>
    /// <param name="vVector">[IN] The vector to which to compare.</param>
    /// <returns>
    /// True if vectors are not the same; False otherwise.
    /// </returns>
    bool operator!=(const QBaseVector4 &vVector) const;


    // ATTRIBUTES
    // ---------------
public:

    /// <summary>
    /// Vector's X coordinate.
    /// </summary>
    float_q x;

    /// <summary>
    /// Vector's Y coordinate.
    /// </summary>
    float_q y;

    /// <summary>
    /// Vector's Z coordinate.
    /// </summary>
    float_q z;

    /// <summary>
    /// Vector's W coordinate.
    /// </summary>
    float_q w;
};

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QBASEVECTOR4__
