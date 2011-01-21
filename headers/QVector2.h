// [TERMS&CONDITIONS]

#ifndef __QVECTOR2__
#define __QVECTOR2__

#include "QBaseVector2.h"

using namespace Kinesis::QuimeraEngine::Tools::DataTypes;

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Math
{

/// <summary>
/// This class implements two components vector functionality. It inherits from QBaseVector2.
///	A vector is a geometric object that has both a magnitude (or length) and direction. 
///	It is frequently represented by a line segment with a definite direction, or graphically as an arrow, 
///	connecting an initial point A with a terminal point B. The vector is then denoted by AB with a arrow hat.
/// </summary>
class QDllExport QVector2 : public QBaseVector2
{
   
public:

	/// <summary>
	/// Default constructor.
	/// </summary>
	inline QVector2() : QBaseVector2() { }

	/// <summary>
	/// Constructor from a QBaseVector2.
	/// </summary>
	/// <param name="v">[IN] The QBaseVector2 in which we want QVector2 to be based.</param>
	inline explicit QVector2(const QBaseVector2 &v) : QBaseVector2(v) { }

	/// <summary>
	/// Constructor from two floating point values for each component x and y.
	/// </summary>
	/// <param name="fValueX">[IN] Floating point value for x component.</param>
	/// <param name="fValueY">[IN] Floating point value for y component.</param>
	inline QVector2(const float_q &fValueX, const float_q &fValueY) : QBaseVector2(fValueX, fValueY) { }

	/// <summary>
	/// Constructor from a single floating point value for all components.
	/// </summary>
	/// <param name="fValue">[IN] Floating point value for all components.</param>
	inline explicit QVector2(const float_q &fValue) : QBaseVector2(fValue) { }
	
	/// <summary>
	/// Constructor from a two component array of floating point values.
	/// </summary>
	/// <param name="pValue">[IN] Pointer to array of floating point values. It must have at least two elements.</param>
	inline explicit QVector2(const float_q *pValue) : QBaseVector2(pValue) { }

	// METHODS
	// ---------------
public:
	// unary operator

	/// <summary>
	/// Vectors negation: it's performed component by component.
	/// </summary>
	/// <returns>
	/// A vector that is the result of the negation.
	/// </returns>
	QVector2 operator - () const;

	// binary operators

	/// <summary>
	/// Vectors addition: it's performed component by component.
	/// </summary>
	/// <param name="v">[IN] Adding vector.</param>
	/// <returns>
	/// A vector that is the result of the addition.
	/// </returns>
	QVector2 operator + (const QVector2 &v) const;

	/// <summary>
	/// Vectors subtraction: it's performed component by component.
	/// </summary>
	/// <param name="v">[IN] Subtracting vector.</param>
	/// <returns>
	/// A vector that is the result of the subtraction.
	/// </returns>
	QVector2 operator - (const QVector2 &v) const;

	/// <summary>
	/// Product by a scalar: all components are multiplied by the floating point value provided.
	/// </summary>
	/// <param name="fValue">[IN] Multiplying floating point value.</param>
	/// <returns>
	/// A vector that is the result of the product.
	/// </returns>
	QVector2 operator * (const float_q &fValue) const;

	/// <summary>
	/// Product by a vector: it's performed component by component. It's not dot product or cross product!
	/// </summary>
	/// <param name="v">[IN] Multiplying vector.</param>
	/// <returns>
	/// A vector that is the result of the product.
	/// </returns>
	QVector2 operator * (const QBaseVector2 &v) const;
	
	/// <summary>
	/// Division by a scalar: all components are divided by the floating point value provided.
	/// </summary>
	/// <param name="fValue">[IN] Dividing floating point value.</param>
	/// <returns>
	/// A vector that is the result of the division.
	/// </returns>
	QVector2 operator / (const float_q &fValue) const;
		
	/// <summary>
	/// Division by a vector: it's performed component by component.
	/// </summary>
	/// <param name="v">[IN] Dividing vector.</param>
	/// <returns>
	/// A vector that is the result of the division.
	/// </returns>
	QVector2 operator / (const QBaseVector2 &v) const;
	
	/// <summary>
	/// Product by a scalar: all components are multiplied by the floating point value provided.
	/// Shorcut to multiply on the left.
	/// </summary>
	/// <param name="fValue">[IN] Multiplying floating point value.</param>
	/// <param name="v">[IN] Vector to be multiplied.</param>
	/// <returns>
	/// A vector that is the result of the product.
	/// </returns>
	friend QVector2 operator * (const float_q &fValue, const QVector2 &v);

	/// <summary>
	/// Compares two vectors.
	/// </summary>
	/// <param name="v">[IN] Vector with which to compare.</param>
	/// <returns>
	/// True if vectors are the same, false otherwise.
	/// </returns>
	inline bool operator == (const QBaseVector2 &v) const
	{
        return ( QFloat::AreEquals(v.x, this->x) && QFloat::AreEquals(v.y, this->y) );
	}

	/// <summary>
	/// Compares two vectors.
	/// </summary>
	/// <param name="v">[IN] Vector with which to compare.</param>
	/// <returns>
	/// True if vectors are not the same, false otherwise.
	/// </returns>
	inline bool operator != (const QBaseVector2 &v) const
	{
        return QFloat::AreNotEquals(v.x, this->x) || QFloat::AreNotEquals(v.y, this->y);
	}

	// assignment operators

	/// <summary>
	/// Adds a vector provided to current vector.
	/// </summary>
	/// <param name="v">[IN] Adding vector.</param>
	/// <returns>
	/// A reference to vector result of the addition.
	/// </returns>
	inline QVector2& operator += (const QVector2 &v) 
	{ 
		this->x += v.x;
		this->y += v.y;
	
		return *this; 
	}

	/// <summary>
	/// Subtracts a vector provided to current vector.
	/// </summary>
	/// <param name="v">[IN] Subtracting vector.</param>
	/// <returns>
	/// A reference to vector result of the subtraction.
	/// </returns>
	inline QVector2& operator -= (const QVector2 &v) 
	{ 
		this->x -= v.x; 
		this->y -= v.y;  
		
		return *this; 
	}

	/// <summary>
	/// Multiplies current vector by a floating point value provided.
	/// </summary>
	/// <param name="fValue">[IN] Multiplying floating point value.</param>
	/// <returns>
	/// A reference to vector result of the product.
	/// </returns>
	inline QVector2& operator *= (const float_q &fValue) 
	{ 
		this->x *= fValue; 
		this->y *= fValue; 
	
		return *this;
	}

	/// <summary>
	/// Multiplies current vector by a vector provided. It's performed component by component. It's not dot product or cross product!
	/// </summary>
	/// <param name="v">[IN] Multiplying vector.</param>
	/// <returns>
	/// A reference to vector result of the product.
	/// </returns>
	inline QVector2& operator *= (const QBaseVector2 &v) 
	{ 
		this->x *= v.x; 
		this->y *= v.y; 
		
		return *this; 
	}

	/// <summary>
	/// Divides current vector by a floating point value provided.
	/// </summary>
	/// <param name="fValue">[IN] Dividing floating point value.</param>
	/// <returns>
	/// A reference to vector result of the division.
	/// </returns>
	inline QVector2& operator /= (const float_q &fValue) 
	{ 
		// Checkout to avoid division by 0
		QE_ASSERT(fValue);

		this->x /= fValue; 
		this->y /= fValue; 
		
		return *this; 
	}
	
	/// <summary>
	/// Divides current vector by a vector provided. It's performed component by component.
	/// </summary>
	/// <param name="v">[IN] Dividing vector.</param>
	/// <returns>
	/// A reference to vector result of the division.
	/// </returns>
	inline QVector2& operator /= (const QBaseVector2 &v)
	{
		// Checkout to avoid division by 0
		QE_ASSERT (v.x && v.y);

		this->x /= v.x; 
		this->y /= v.y; 
 
		
		return *this;
	}
	
	/// <summary>
	/// Calculates the length of current vector.
	/// </summary>
	/// <returns>
	/// A floating point value which is the length of the vector.
	/// </returns>
	inline float_q GetLength() const
	{ 
		return sqrt(this->x*this->x + this->y*this->y); 
	}

	/// <summary>
	/// Calculates the vector which is perpendicular to the current vector.
	/// </summary>
	/// <returns>
	/// A Vector which is perpendicular of the vector.
	/// </returns>
	QVector2 GetPerpendicular() const;

	/// <summary>
	/// Makes current vector unitary
	/// </summary>
	inline void Normalize() 
	{
		// Gets vector length
		float_q fLength = this->GetLength(); 

		// Checkout to avoid division by 0
		QE_ASSERT(fLength);

		//Normalize
		this->x /= fLength; 
		this->y /= fLength;	
	}

	/// <summary>
	/// Convert current vector in its opposite vector.
	/// </summary>
	inline void Reverse() 
	{ 
		this->x = -this->x; 
		this->y = -this->y;  
	}

	/// <summary>
	/// Resets all components of current vector to 1.
	/// </summary>
	inline void ResetToOne() 
	{ 
		this->x = QFloat::_1; 
		this->y = QFloat::_1;  
	}

	/// <summary>
	/// Resets all components of current vector to 0.
	/// </summary>
	inline void ResetToZero() 
	{ 
		this->x = QFloat::_0; 
		this->y = QFloat::_0; 
	}

	/// <summary>
	/// Check if all components of current vector are 0.
	/// </summary>
	/// <returns>
	/// True if all components are 0. False otherwise.
	/// </returns>
	inline bool IsZero() 
	{ 
		return QFloat::IsZero(this->x) && QFloat::IsZero(this->y); 
	}

	/// <summary>
	/// Check if all components of current vector are 1.
	/// </summary>
	/// <returns>
	/// True if all components are 1. False otherwise.
	/// </returns>
	inline bool IsVectorOfOnes() 
	{ 
		return QFloat::AreEquals(this->x, QFloat::_1) && QFloat::AreEquals(this->y, QFloat::_1); 
	}

	/// <summary>
	/// Calculates Dot Product by a vector provided.
	/// </summary>
	/// <param name="v">[IN] Multiplying vector.</param>
	/// <returns>
	/// A floating point value which is the result of Dot Product.
	/// </returns>
	inline float_q DotProduct(const QVector2 &v) const 
	{ 
		return(this->x*v.x + this->y*v.y); 
	}


	/// <summary>
	/// Makes a Linear Interpolation between current vector and other vector provided. It stores result in current vector.
	/// </summary>
	/// <param name="fFactor">[IN] A floating point value which represents how close is the result vector from the current vector (per one).</param>
	/// <param name="v">[IN] Vector with which to interpolate.</param>
	inline void Lerp(const float_q &fFactor, const QBaseVector2 &v)
	{ 
		this->x = this->x*fFactor + v.x*(QFloat::_1 - fFactor);
		this->y = this->y*fFactor + v.y*(QFloat::_1 - fFactor); 
	}


	/// <summary>
	/// Calculates the distance between two vector heads (or two points). It computes the difference of two vectors and returns its length.
	/// </summary>
	/// <param name="v">[IN] Vector we want to calculate the distance from current vector.</param>
	/// <returns>
	/// A positive floating point value containing the distance between both vectors.
	/// </returns>
	inline float_q Distance(const QVector2 &v) const
	{
		return sqrt( (this->x-v.x)*(this->x-v.x) + (this->y-v.y)*(this->y-v.y) );
	}

	/// <summary>
	/// Converts vector into a string with the following format:
	/// �V3(x, y, z)"
	/// </summary>
	/// <returns>The string with the format specified.</returns>
	string_q ToString();
	
	// ATTRIBUTES
	// ---------------
public:

	/// <summary>
	/// Stores a vector with all components set to 0
	/// </summary>
	static const QVector2 ZeroVector;
	/// <summary>
	/// Stores a vector with all components set to 1
	/// </summary>
	static const QVector2 VectorOfOnes;
	/// <summary>
	/// Stores a unit vector in X positive direction
	/// </summary>
	static const QVector2 UnitVectorX;
	/// <summary>
	/// Stores a unit vector in Y positive direction
	/// </summary>
	static const QVector2 UnitVectorY;
	/// <summary>
	/// Stores a unit vector in X negative direction
	/// </summary>
	static const QVector2 UnitVectorInvX;
	/// <summary>
	/// Stores a unit vector in Y negative direction
	/// </summary>
	static const QVector2 UnitVectorInvY;

};

} //namespace Math
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QVECTOR2__