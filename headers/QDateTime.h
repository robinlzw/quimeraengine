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

#ifndef __QDATETIME__
#define __QDATETIME__

#include "QTimeSpan.h"

using Kinesis::QuimeraEngine::Common::DataTypes::u64_q;
using Kinesis::QuimeraEngine::Common::DataTypes::i32_q;


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Time
{

// Forward declarations
class QTimeZone;

    
/// <summary>
/// Represents a unique instant in the continuous time, using the Gregorian calendar to represent the date and
/// the western time measurement system, based on the Coordinated Universal Time (UTC), to represent the time.
/// </summary>
/// <remarks>
/// Date and time are stored together but this class can also be used to represent either the date only or the time only. When
/// it is used to represent the time only, the first day of the Anno Domini will be used; this is just a convention and
/// has no relevance. When it is used to store only dates, the time will be zero.<br/>
/// The range of time that can be represented is from 29228/11/23 21:11:54.5224193 B.C. to 29228-02-08 02:48:05.4775807 A. D., with
/// a resolution of 100 nanoseconds.<br/>
/// Instances are undefined by default until they are assigned a value.<br/>
/// This class is not completely immutable. Only the assignment operator can modify an instance once it is created.
/// </remarks>
class QE_LAYER_TOOLS_SYMBOLS QDateTime
{
    // CONSTANTS
	// ---------------
private:

    /// <summary>
	/// The central value representable by a 64 bits unsigned integer.
	/// </summary>
    static const u64_q HALF_VALUE;

    /// <summary>
	/// The amount of hundreds of nanoseconds per microsecond.
	/// </summary>
    static const u64_q HNS_PER_MICROSECOND;

    /// <summary>
	/// The amount of hundreds of nanoseconds per millisecond.
	/// </summary>
    static const u64_q HNS_PER_MILLISECOND;

    /// <summary>
	/// The amount of hundreds of nanoseconds per second.
	/// </summary>
    static const u64_q HNS_PER_SECOND;

    /// <summary>
	/// The amount of hundreds of nanoseconds per minute.
	/// </summary>
    static const u64_q HNS_PER_MINUTE;

    /// <summary>
	/// The amount of hundreds of nanoseconds per hour.
	/// </summary>
    static const u64_q HNS_PER_HOUR;

    /// <summary>
	/// The amount of hundreds of nanoseconds per day.
	/// </summary>
    static const u64_q HNS_PER_DAY;

    /// <summary>
	/// The amount of hundreds of nanoseconds per year.
	/// </summary>
    static const u64_q HNS_PER_YEAR;

    /// <summary>
	/// The amount of hundreds of nanoseconds per leap year.
	/// </summary>
    static const u64_q HNS_PER_LEAPYEAR;

    /// <summary>
	/// The amount of hundreds of nanoseconds per four consecutive years, taking into account the leap year.
	/// </summary>
    static const u64_q HNS_PER_4_CONSECUTIVE_YEARS;


	// CONSTRUCTORS
	// ---------------
public:

	/// <summary>
	/// Default constructor. Instances are undefined by default.
	/// </summary>
	QDateTime();

    /// <summary>
    /// Copy constructor. Date and time are copied as well as the time zone information.
    /// </summary>
    /// <param name="dateTime">[IN] The date and time to be copied.</param>
    QDateTime(const QDateTime &dateTime);

    /// <summary>
    /// Constructor that receives the value of every time unit, in local time, plus the time zone information.
    /// </summary>
    /// <remarks>
    /// It is important to know that both the input date and the input time are expected to be local, this means, they are shifted by
    /// the offsets of the time zone and the Daylight Saving Time.<br/>
    /// If the input date is posterior to the maximum positive date, then the maximum positive value will be used instead.<br/>
    /// If the input date is prior to the maximum negative date, then the maximum negative value will be used instead.<br/>
    /// If arguments are not valid, the result is undefined.
    /// </remarks>
    /// <param name="nYear">[IN] The year, which may be positive (A.D.) or negative (B.C). The year must be prior to 29229
    /// and posterior to -29229. Zero is not a valid year.</param>
    /// <param name="uMonth">[IN] The month number, being January the number 1. It must be greater than zero and lower than 13.</param>
    /// <param name="uDay">[IN] The day number, as it appears in the calendar. It must be greater than zero and lower than or equal to
    /// the corresponding last day of the month.</param>
    /// <param name="uHour">[IN] The hour, as it appears in the clock. It must be lower than 24.</param>
    /// <param name="uMinute">[IN] The minute, as it appears in the clock. It must be lower than 60.</param>
    /// <param name="uSecond">[IN] The second, as it appears in the clock. It must be lower than 60.</param>
    /// <param name="uMillisecond">[IN] The millisecond, as it appears in the clock. It must be lower than 1.000.</param>
    /// <param name="uMicrosecond">[IN] The microsecond, as it appears in the clock. It must be lower than 1.000.</param>
    /// <param name="uHundredsOfNanosecond">[IN] The number of hundreds of nanoseconds (tenths of microsecond). It must be lower than 10.</param>
    /// <param name="pTimeZone">[Optional][IN] The information about the time zone on which the time is based. Its default value 
    /// is null, which means no time zone offset (UTC).</param>
    QDateTime(const i32_q nYear, const u64_q uMonth, const u64_q uDay, 
              const u64_q uHour, const u64_q uMinute, const u64_q uSecond, 
              const u64_q uMillisecond, const u64_q uMicrosecond, const u64_q uHundredsOfNanosecond, 
              const QTimeZone* pTimeZone = null_q);

    /// <summary>
    /// Constructor that receives a date, in local time, plus the time zone information. The time will be set to 00:00:00.0.
    /// </summary>
    /// <remarks>
    /// It is important to know that both the input date is expected to be local, this means, it is shifted by
    /// the offsets of the time zone and the Daylight Saving Time.<br/>
    /// If the input date is posterior to the maximum positive date, then the maximum positive value will be used instead.<br/>
    /// If the input date is prior to the maximum negative date, then the maximum negative value will be used instead.<br/>
    /// If arguments are not valid, the result is undefined.<br/>
    /// There is a special case regarding the farthest negative date; it cannot be 29228/11/23 because it is anterior
    /// to the maximum negative date, which starts at 21:11:54.5224193. Therefore, the maximum negative date allowed through this
    /// constructor is 29228/11/24.
    /// </remarks>
    /// <param name="nYear">[IN] The year, which may be positive (A.D.) or negative (B.C). The year must be prior to 29229
    /// and posterior to -29229. Zero is not a valid year.</param>
    /// <param name="uMonth">[IN] The month number, being January the number 1. It must be greater than zero and lower than 13.</param>
    /// <param name="uDay">[IN] The day number, as it appears in the calendar. It must be greater than zero and lower than or equal to
    /// the corresponding last day of the month.</param>
    /// <param name="uHundredsOfNanosecond">[IN] The number of hundreds of nanoseconds (tenths of microsecond).</param>
    /// <param name="pTimeZone">[Optional][IN] The information about the time zone on which the time is based. Its default value 
    /// is null, which means no time zone offset (UTC).</param>
    QDateTime(const i32_q nYear, const u64_q uMonth, const u64_q uDay, const QTimeZone* pTimeZone = null_q);

    /// <summary>
    /// Constructor that receives the time, in local time, plus the time zone information. The date will be 0001/01/01 A. D..
    /// </summary>
    /// <remarks>
    /// It is important to know that the input time is expected to be local, this means, it is shifted by
    /// the offsets of the time zone and the Daylight Saving Time.<br/>
    /// If arguments are not valid, the result is undefined.
    /// </remarks>
    /// <param name="uHour">[IN] The hour, as it appears in the clock. It must be lower than 24.</param>
    /// <param name="uMinute">[IN] The minute, as it appears in the clock. It must be lower than 60.</param>
    /// <param name="uSecond">[IN] The second, as it appears in the clock. It must be lower than 60.</param>
    /// <param name="uMillisecond">[IN] The millisecond, as it appears in the clock. It must be lower than 1.000.</param>
    /// <param name="uMicrosecond">[IN] The microsecond, as it appears in the clock. It must be lower than 1.000.</param>
    /// <param name="uHundredsOfNanosecond">[IN] The number of hundreds of nanoseconds (tenths of microsecond). It must be lower than 10.</param>
    /// <param name="pTimeZone">[Optional][IN] The information about the time zone on which the time is based. Its default value 
    /// is null, which means no time zone offset (UTC).</param>
    QDateTime(const u64_q uHour, const u64_q uMinute, const u64_q uSecond, 
              const u64_q uMillisecond, const u64_q uMicrosecond, const u64_q uHundredsOfNanosecond, 
              const QTimeZone* pTimeZone = null_q);

    /// <summary>
    /// Constructor that receives the time, in local time, plus the time zone information. The date will be 0001/01/01 A. D..
    /// </summary>
    /// <remarks>
    /// It is important to know that the input time is expected to be local, this means, it is shifted by
    /// the offsets of the time zone and the Daylight Saving Time.<br/>
    /// If arguments are not valid, the result is undefined.
    /// </remarks>
    /// <param name="uHour">[IN] The hour, as it appears in the clock. It must be lower than 24.</param>
    /// <param name="uMinute">[IN] The minute, as it appears in the clock. It must be lower than 60.</param>
    /// <param name="uSecond">[IN] The second, as it appears in the clock. It must be lower than 60.</param>
    /// <param name="uMillisecond">[IN] The millisecond, as it appears in the clock. It must be lower than 1.000.</param>
    /// <param name="pTimeZone">[Optional][IN] The information about the time zone on which the time is based. Its default value 
    /// is null, which means no time zone offset (UTC).</param>
    QDateTime(const u64_q uHour, const u64_q uMinute, const u64_q uSecond, const u64_q uMillisecond,
              const QTimeZone* pTimeZone = null_q);


	// METHODS
	// ---------------
public:

    /// <summary>
    /// Assignment operator which copies the date, the time and the time zone information.
    /// </summary>
    /// <param name="dateTime">[IN] The date and time to copy.</param>
    /// <returns>
    /// A reference to the resident value.
    /// </returns>
    QDateTime& operator=(const QDateTime& dateTime);

    /// <summary>
    /// Gets the number of days in a given month, taking into account leap years.
    /// </summary>
    /// <remarks>
    /// If arguments are not valid, the result is undefined.
    /// </remarks>
    /// <param name="uMonth">[IN] The number of the month, being January the number 1. It must be greater than zero and lower than 13.</param>
    /// <param name="nYear">[IN] The year, positive (A.D.) or negative (B.C.), in order to know whether Fabruary has 28 or 29 days. 
    /// Zero is not a valid year.</param>
    /// <returns>
    /// The number of days in the month.
    /// </returns>
    static unsigned int GetDaysInMonth(const unsigned int uMonth, const int nYear);

    /// <summary>
    /// Gets an undefined date and time. Undefined dates/times has no value so it is not possible to operate with them.
    /// </summary>
    /// <returns>
    /// A reference to a constant undefined date/time.
    /// </returns>
    static const QDateTime& GetUndefinedDate();

protected:

    /// <summary>
    /// Calculates whether a year is a leap year or not.
    /// </summary>
    /// <param name="nYear">[IN] The year, positive (A.D.) or negative (B.C.).</param>
    /// <returns>
    /// True if it is a leap year; False otherwise.
    /// </returns>
    static bool IsLeapYear(const int nYear);

    /// <summary>
    /// Applies the offset of the time zone and the DST to the internal time data so it becomes UTC.
    /// </summary>
    void SubtractTimeZoneOffset();


	// PROPERTIES
	// ---------------
public:

    /// <summary>
    /// Calculates whether the year is a leap year or not.
    /// </summary>
    /// <returns>
    /// True if it is a leap year; False otherwise.
    /// </returns>
    bool IsLeapYear() const;

    /// <summary>
    /// Gets the time zone information.
    /// </summary>
    /// <returns>
    /// A time zone instance.
    /// </returns>
    const QTimeZone* GetTimeZone() const;


	// ATTRIBUTES
	// ---------------
private:

    /// <summary>
    /// The internal representation of the unique instant in time.
    /// </summary>
    QTimeSpan m_instant;

    /// <summary>
    /// The time zone to be used along with the internal time to get the local time.
    /// </summary>
    const QTimeZone* m_pTimeZone;

};

} //namespace Time
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QDATETIME__