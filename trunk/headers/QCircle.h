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

#ifndef __QCIRCLE__
#define __QCIRCLE__

#include "QOrb.h"
#include "QVector2.h"
#include "EQIntersections.h"

using Kinesis::QuimeraEngine::Common::DataTypes::SQFloat;
using Kinesis::QuimeraEngine::Common::DataTypes::float_q;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{
    
/// <summary>
/// Class which represents a circle in bidimensional space.
/// </summary>
/// <remarks>
/// A circle is the collection of all points that are equidistant from other point known as "Center".
/// </remarks>
class QE_LAYER_TOOLS_SYMBOLS QCircle : public QOrb<QVector2>
{

    // BASE CLASS USINGS
    // -------------------
public:

    using QOrb<QVector2>::Center;
    using QOrb<QVector2>::Radius;


    // CONSTRUCTORS
    // ---------------
public:

    /// <summary>
    /// Default constructor.
    /// </summary>
    QCircle();

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="circle">[IN] The circle from which we want to create a copy in the resident circle.</param>
    QCircle(const QCircle &circle);

    /// <summary>
    /// Base type constructor.
    /// </summary>
    /// <param name="orb">[IN] The orb in which we want resident circle to be based.</param>
    QCircle(const QBaseOrb<QVector2> &orb);

    /// <summary>
    /// Constructor from a vector which defines the center point and a floating point value which
    /// defines the radius for the circle.
    /// </summary>
    /// <param name="vCenter">[IN] Vector to define the center of the cirle.</param>
    /// <param name="fRadius">[IN] A floating point value to define the radius.</param>
    QCircle(const QVector2 &vCenter, const float_q fRadius);


    // PROPERTIES
    // ---------------
public:

    /// <summary>
    /// Gets a "unit circle" placed in the center of coordinates and whose radius equals 1.
    /// </summary>
    /// <returns>
    /// A unit circle.
    /// </returns>
    static const QCircle& GetUnitCircle();


    // METHODS
    // ---------------
public:

    /// <summary>
    /// Assignation operator.
    /// </summary>
    /// <param name="orb">[IN] The orb to be copied from.</param>
    /// <returns>
    /// A reference to the modified orb.
    /// </returns>
    QCircle& operator=(const QBaseOrb<QVector2> &orb);

    /// <summary>
    /// Translates the circle.
    /// </summary>
    /// <param name="vTranslation">[IN] 2D vector that contains the translation to be applied.</param>
    /// <returns>
    /// The translated circle.
    /// </returns>
    QCircle Translate(const QBaseVector2 &vTranslation) const;

    /// <summary>
    /// Translates the circle.
    /// </summary>
    /// <param name="fTranslationX">[IN] Scalar that contains the translation on X axis.</param>
    /// <param name="fTranslationY">[IN] Scalar that contains the translation on Y axis.</param>
    /// <returns>
    /// The translated circle.
    /// </returns>
    QCircle Translate(const float_q fTranslationX, const float_q fTranslationY) const;

    /// <summary>
    /// Rotates the circle.
    /// </summary>
    /// <param name="fRotationAngle">[IN] Scalar that contains the angle of rotation.</param>
    /// <returns>
    /// The rotated circle.
    /// </returns>
    QCircle Rotate(const float_q fRotationAngle) const;

    /// <summary>
    /// Rotates the circle using a pivot.
    /// </summary>
    /// <param name="fRotationAngle">[IN] Scalar that contains the angle of rotation.</param>
    /// <param name="vPivot">[IN] Vector used as pivot for the rotation.</param>
    /// <returns>
    /// The rotated circle.
    /// </returns>
    QCircle RotateWithPivot(const float_q fRotationAngle, const QBaseVector2 &vPivot) const;

     /// <summary>
    /// Scales the circle.
    /// </summary>
    /// <param name="vScale">[IN] 2D vector that contains the scale to be applied.</param>
    /// <param name="fRadiusScale">[IN] Multiplying factor to scale the circle's radius.</param>
    /// <returns>
    /// The scaled circle.
    /// </returns>
    QCircle Scale(const QBaseVector2 &vScale, const float_q fRadiusScale) const;

     /// <summary>
    /// Scales the circle.
    /// </summary>
    /// <param name="fScaleX">[IN] Scalar that contains the scale on X axis.</param>
    /// <param name="fScaleY">[IN] Scalar that contains the scale on Y axis.</param>
    /// <param name="fRadiusScale">[IN] Multiplying factor to scale the circle's radius.</param>
    /// <returns>
    /// The scaled circle.
    /// </returns>
    QCircle Scale(const float_q fScaleX, const float_q fScaleY, const float_q fRadiusScale) const;

     /// <summary>
    /// Scales the circle using a pivot.
    /// </summary>
    /// <param name="vScale">[IN] 2D vector that contains the scale to be applied.</param>
    /// <param name="fRadiusScale">[IN] Multiplying factor to scale the circle's radius.</param>
    /// <param name="vPivot">[IN] Vector used as pivot for the scale.</param>
    /// <returns>
    /// The scaled circle.
    /// </returns>
    QCircle ScaleWithPivot(const QBaseVector2 &vScale, const float_q fRadiusScale, const QBaseVector2 &vPivot) const;

     /// <summary>
    /// Scales the circle using a pivot.
    /// </summary>
    /// <param name="fScaleX">[IN] Scalar that contains the scale on X axis.</param>
    /// <param name="fScaleY">[IN] Scalar that contains the scale on Y axis.</param>
    /// <param name="fRadiusScale">[IN] Multiplying factor to scale the circle's radius.</param>
    /// <param name="vPivot">[IN] Vector used as pivot for the scale.</param>
    /// <returns>
    /// The scaled circle.
    /// </returns>
    QCircle ScaleWithPivot(const float_q fScaleX, const float_q fScaleY, const float_q fRadiusScale, const QBaseVector2 &vPivot) const;

     /// <summary>
    /// Transforms the circle.
    /// </summary>
    /// <param name="transformation">[IN] Matrix that contains the transformations to apply.</param>
    /// <param name="fRadiusScale">[IN] Multiplying factor to scale the circle's radius.</param>
    /// <returns>
    /// The transformed circle.
    /// </returns>
    QCircle Transform(const QTransformationMatrix3x3 &transformation, const float_q fRadiusScale) const;

     /// <summary>
    /// Transforms the circle using a pivot.
    /// </summary>
    /// <param name="transformation">[IN] Matrix that contains the transformations to apply.</param>
    /// <param name="fRadiusScale">[IN] Multiplying factor to scale the circle's radius.</param>
    /// <param name="vPivot">[IN] Vector used as pivot for the transformation.</param>
    /// <returns>
    /// The transformed circle.
    /// </returns>
    QCircle TransformWithPivot(const QTransformationMatrix3x3 &transformation, const float_q fRadiusScale, const QBaseVector2 &vPivot) const;

     /// <summary>
    /// Calculates possible intersections between resident circle and a circle received as parameter.
    /// </summary>
    /// <remarks>
    /// If any of both circles' radius equals zero, the result is undefined.<br/>
    /// If there are no intersections or it there are infinite, the output parameters will not be modified.
    /// </remarks>
    /// <param name="circle">[IN] A circle that is supposed to intersect with resident one.</param>
    /// <param name="vIntersection1">[OUT] An intersection point. If there is only one intersection, it will be stored in this parameter.</param>
    /// <param name="vIntersection2">[OUT] An intersection point.</param>
    /// <returns>
    /// An enumerated value that indicates how many intersections were found:<br/>
    /// <br/>
    /// <b>None</b><br/>
    /// There are no intersections.<br/>
    ///
    /// <b>One</b><br/>
    /// There is one intersection.<br/>
    /// - Circles are tangent.
    ///
    /// <b>Two</b><br/>
    /// There are two intersections.<br/>
    /// - Circles intersect but not coincide and are not contained in each other.
    ///
    /// <b>Infinite</b><br/>
    /// There are infinite intersections.<br/>
    /// - Circles are the same.
    /// - One circle is contained in the other.
    /// </returns>
    EQIntersections IntersectionPoint(const QBaseOrb<QVector2> &circle, QBaseVector2 &vIntersection1, QBaseVector2 &vIntersection2) const;
    
};

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QCIRCLE__
