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
#include "DataTypesDefinitions.h"

using Kinesis::QuimeraEngine::Common::DataTypes::u64_q;
using Kinesis::QuimeraEngine::Common::DataTypes::i32_q;
using Kinesis::QuimeraEngine::Common::DataTypes::string_q;

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
/// The range of time that can be represented is from 29228-4-18 21:11:54.5224193 B.C. to 29228-09-14 02:48:05.4775807 A.D., with
/// a resolution of 100 nanoseconds.<br/>
/// Instances are undefined by default until they are assigned a value.<br/>
/// This class is not immutable, although only the assignment operator can modify an instance once it is created.
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
    /// The internal value for the instant stored in an undefined date/time object.
    /// </summary>
    static const QTimeSpan UNDEFINED_VALUE;


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
    /// There is a special case regarding the farthest negative date; it cannot be 29228-04-18 because it is anterior
    /// to the maximum negative date, which starts at 21:11:54.5224193. Therefore, the maximum negative date allowed through this
    /// constructor is 29228-04-19.
    /// </remarks>
    /// <param name="nYear">[IN] The year, which may be positive (A.D.) or negative (B.C). The year must be prior to 29229
    /// and posterior to -29229. Zero is not a valid year.</param>
    /// <param name="uMonth">[IN] The month number, being January the number 1. It must be greater than zero and lower than 13.</param>
    /// <param name="uDay">[IN] The day number, as it appears in the calendar. It must be greater than zero and lower than or equal to
    /// the corresponding last day of the month.</param>
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
    
    /// <summary>
    /// Constructor that copies a date and time, using a different time zone.
    /// </summary>
    /// <param name="dateTime">[IN] The date and time to be copied.</param>
    /// <param name="pTimeZone">[IN] The information about the time zone on which the time is based. Null means no time zone offset (UTC).</param>
    QDateTime(const QDateTime &dateTime, const QTimeZone* pTimeZone);
    
    /// <summary>
    /// Builds a valid date/time instance from a string that contains a timestamp based on the ISO 8601:2004 standard.
    /// </summary>
    /// <remarks>
    /// Only calendar dates or times (including time zone offsets), using any of both basic or extended format, are allowed. Week dates, 
    /// ordinal dates, durations and time intervals are not supported.<br/>
    /// Since this method has no way to distinguish between dates and times (unless they appear combined in the same timestamp), the
    /// following cases will be parsed as times:<br/>
    /// - "YYMMDD" will be treated as "hhmmss"<br/>
    /// - "YYYYMM" will be treated as "hhmmss"<br/>
    /// - "YY-MM" will be treated as "hh-hh"<br/>
    /// - "YYYY-MM" will be treated as "hhmm-hh"<br/>
    /// When using timestamps that combine both dates and times, the separator "T" is mandatory. Besides, both must be complete, without the 
    /// omission of any part.<br/>
    /// No whitespaces are allowed.<br/>
    /// Any letter in the timestamp must be uppercase (for example: "T" or "Z").<br/>
    /// Fractions of second are optional but, when they appear, they must be 7-numbers long at most.<br/>
    /// Remember that, in ISO-8601 timestamps, negative years start at year -0000 unlike positive dates which start at year +0001.<br/>
    /// More information at http://en.wikipedia.org/wiki/ISO_8601 .
    /// </remarks>
    /// <param name="strTimestamp">[IN] A valid timestamp compound of a date, a time or both. It must not be empty nor contain whitespaces.</param>
    QDateTime(const string_q &strTimestamp);
    

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
    /// Moves forward the current instant, adding a time span.
    /// </summary>
    /// <remarks>
    /// It is not possible to operate with undefined dates; the date/time will not be changed.<br/>
    /// If the result is posterior to the maximum, it will be set to the maximum.<br/>
    /// The result will be stored in this instance.
    /// </remarks>
    /// <param name="timeToAdd">[IN] The amount of time to move the current date/time.</param>
    /// <returns>
    /// A reference to the resident value.
    /// </returns>
    QDateTime& operator+=(const QTimeSpan &timeToAdd);

    /// <summary>
    /// Moves backward the current instant, subtracting a time span.
    /// </summary>
    /// <remarks>
    /// It is not possible to operate with undefined dates; the date/time will not be changed.<br/>
    /// If the result is prior to the minimum, it will be set to the minimum.<br/>
    /// The result will be stored in this instance.
    /// </remarks>
    /// <param name="timeToSubtract">[IN] The amount of time to move the current date/time.</param>
    /// <returns>
    /// A reference to the resident value.
    /// </returns>
    QDateTime& operator-=(const QTimeSpan &timeToSubtract);

    /// <summary>
    /// Moves forward the current instant, adding a time span and returning the result.
    /// </summary>
    /// <remarks>
    /// It is not possible to operate with undefined dates; the date/time will not be changed.<br/>
    /// If the result is posterior to the maximum, it will be set to the maximum.
    /// </remarks>
    /// <param name="timeToAdd">[IN] The amount of time to move the current date/time.</param>
    /// <returns>
    /// The displaced date/time.
    /// </returns>
    QDateTime operator+(const QTimeSpan &timeToAdd) const;

    /// <summary>
    /// Moves backward the current instant, subtracting a time span and returning the result.
    /// </summary>
    /// <remarks>
    /// It is not possible to operate with undefined dates; the date/time will not be changed.<br/>
    /// If the result is prior to the minimum, it will be set to the minimum.
    /// </remarks>
    /// <param name="timeToSubtract">[IN] The amount of time to move the current date/time.</param>
    /// <returns>
    /// The displaced date/time.
    /// </returns>
    QDateTime operator-(const QTimeSpan &timeToSubtract) const;

    /// <summary>
    /// Calculates the difference between two dates/times.
    /// </summary>
    /// <remarks>
    /// It is not possible to operate with undefined dates; the result will be zero time.<br/>
    /// The result will be always a positive value, the order of the operands does not matter.
    /// </remarks>
    /// <param name="dateTime">[IN] The other date/time with which to calculate the difference.</param>
    /// <returns>
    /// The difference between both dates/times, as a time span.
    /// </returns>
    QTimeSpan operator-(const QDateTime &dateTime) const;

    /// <summary>
    /// Checks whether two dates/times are equal.
    /// </summary>
    /// <remarks>
    /// The time zone is ignored when comparing the date and the time, it means, comparisons are performed using UTC time.
    /// </remarks>
    /// <param name="dateTime">[IN] The date/time to compare to.</param>
    /// <returns>
    /// True if date/times are equal; False otherwise. If only one of the operands is undefined, the result will be False;
    /// if both operads are undefined, the result will be True.
    /// </returns>
    bool operator==(const QDateTime &dateTime) const;

    /// <summary>
    /// Checks whether two dates/times are different.
    /// </summary>
    /// <remarks>
    /// The time zone is ignored when comparing the date and the time, it means, comparisons are performed using UTC time.
    /// </remarks>
    /// <param name="dateTime">[IN] The date/time to compare to.</param>
    /// <returns>
    /// True if date/times are different; False otherwise. If only one of the operands is undefined, the result will be True;
    /// if both operads are undefined, the result will be False.
    /// </returns>
    bool operator!=(const QDateTime &dateTime) const;

    /// <summary>
    /// Checks whether a date/time is prior to other.
    /// </summary>
    /// <remarks>
    /// The time zone is ignored when comparing the date and the time, it means, comparisons are performed using UTC time.
    /// </remarks>
    /// <param name="dateTime">[IN] The date/time to compare to.</param>
    /// <returns>
    /// True if the resident date/time (left operand) is prior to input date/time (right operand); False otherwise. If any of the operands is 
    /// undefined, the result will be False.
    /// </returns>
    bool operator<(const QDateTime &dateTime) const;

    /// <summary>
    /// Checks whether a date/time is posterior to other.
    /// </summary>
    /// <remarks>
    /// The time zone is ignored when comparing the date and the time, it means, comparisons are performed using UTC time.
    /// </remarks>
    /// <param name="dateTime">[IN] The date/time to compare to.</param>
    /// <returns>
    /// True if the resident date/time (left operand) is posterior to input date/time (right operand); False otherwise. If any of the operands is 
    /// undefined, the result will be False.
    /// </returns>
    bool operator>(const QDateTime &dateTime) const;

    /// <summary>
    /// Checks whether a date/time is prior or equal to other.
    /// </summary>
    /// <remarks>
    /// The time zone is ignored when comparing the date and the time, it means, comparisons are performed using UTC time.
    /// </remarks>
    /// <param name="dateTime">[IN] The date/time to compare to.</param>
    /// <returns>
    /// True if the resident date/time (left operand) is prior or equal to input date/time (right operand); False otherwise.
    /// If only one of the operands is undefined, the result will be False; if both operads are undefined, the result will be True.
    /// </returns>
    bool operator<=(const QDateTime &dateTime) const;

    /// <summary>
    /// Checks whether a date/time is posterior or equal to other.
    /// </summary>
    /// <remarks>
    /// The time zone is ignored when comparing the date and the time, it means, comparisons are performed using UTC time.
    /// </remarks>
    /// <param name="dateTime">[IN] The date/time to compare to.</param>
    /// <returns>
    /// True if the resident date/time (left operand) is posterior or equal to input date/time (right operand); False otherwise.
    /// If only one of the operands is undefined, the result will be False; if both operads are undefined, the result will be True.
    /// </returns>
    bool operator>=(const QDateTime &dateTime) const;

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

    /// <summary>
    /// Creates a string representation of the date/time instance (a timestamp) following the ISO 8601:2004 standard.
    /// </summary>
    /// <remarks>
    /// The resultant timestamp will be composed of the complete date, preceded by either a positive or negative sign, and the complete 
    /// time (including the time zone offset), using the extended format: [+|-][Y]YYYYY-MM-DDThh:mm:ss[.fffffff][[+|-]hh:mm|Z].<br/>
    /// The year (Y) will be formed by 5 numbers only if it is greater than 9999. The fraction of second will not be included if
    /// the current millisecond, microsecond and hundred of nanosecond equal zero; besides, the amount of decimal numbers will depend
    /// on the value of the mentioned time parts.<br/>
    /// Remember that, in ISO-8601 timestamps, negative years start at year -0000 unlike positive dates which start at year +0001.<br/>
    /// Undefined date/times cannot be represented as valid timestamps.
    /// </remarks>
    /// <returns>
    /// A valid timestamp compound of the date and the time, including a time fraction and the time offset, if any.
    /// </returns>
    string_q ToString() const;
    
    /// <summary>
    /// Gets all the components of the date and the time, in local time.
    /// </summary>
    /// <remarks>
    /// If the date/time is undefined, then all the values will be undefined too.
    /// </remarks>
    /// <param name="uYear">[OUT] The absolute value of the year. To know whether it is a negative or positive year, call IsPositive or IsNegative.</param>
    /// <param name="uMonth">[OUT] The value of the month, being 1 the value that represents January and 12 for December.</param>
    /// <param name="uDay">[OUT] The value of the day, from 1 to 31.</param>
    /// <param name="uHour">[OUT] The value of the hour, from 0 to 23.</param>
    /// <param name="uMinute">[OUT] The value of the minute, from 0 to 59.</param>
    /// <param name="uSecond">[OUT] The value of the second, from 0 to 59.</param>
    /// <param name="uMillisecond">[OUT] The value of the millisecond, from 0 to 999.</param>
    /// <param name="uMicrosecond">[OUT] The value of the microsecond, from 0 to 999.</param>
    /// <param name="uHundredOfNanosecond">[OUT] The number of hundreds of nanoseconds, from 0 to 9.</param>
    void Decompose(unsigned int &uYear, unsigned int &uMonth,  unsigned int &uDay, 
                   unsigned int &uHour, unsigned int &uMinute, unsigned int &uSecond, 
                   unsigned int &uMillisecond, unsigned int &uMicrosecond, unsigned int &uHundredOfNanosecond) const;
    
    /// <summary>
    /// Gets all the components of the date, in local time.
    /// </summary>
    /// <remarks>
    /// If the date/time is undefined, then all the values will be undefined too.
    /// </remarks>
    /// <param name="uYear">[OUT] The absolute value of the year. To know whether it is a negative or positive year, call IsPositive or IsNegative.</param>
    /// <param name="uMonth">[OUT] The value of the month, being 1 the value that represents January and 12 for December.</param>
    /// <param name="uDay">[OUT] The value of the day, from 1 to 31.</param>
    void DecomposeDate(unsigned int &uYear, unsigned int &uMonth, unsigned int &uDay) const;
    
    /// <summary>
    /// Gets the hour, the minute and the second, in local time.
    /// </summary>
    /// <remarks>
    /// If the date/time is undefined, then all the values will be undefined too.
    /// </remarks>
    /// <param name="uHour">[OUT] The value of the hour, from 0 to 23.</param>
    /// <param name="uMinute">[OUT] The value of the minute, from 0 to 59.</param>
    /// <param name="uSecond">[OUT] The value of the second, from 0 to 59.</param>
    void DecomposeTime(unsigned int &uHour, unsigned int &uMinute, unsigned int &uSecond) const;
    
    /// <summary>
    /// Gets all the components of the time, in local time.
    /// </summary>
    /// <remarks>
    /// If the date/time is undefined, then all the values will be undefined too.
    /// </remarks>
    /// <param name="uHour">[OUT] The value of the hour, from 0 to 23.</param>
    /// <param name="uMinute">[OUT] The value of the minute, from 0 to 59.</param>
    /// <param name="uSecond">[OUT] The value of the second, from 0 to 59.</param>
    /// <param name="uMillisecond">[OUT] The value of the millisecond, from 0 to 999.</param>
    /// <param name="uMicrosecond">[OUT] The value of the microsecond, from 0 to 999.</param>
    /// <param name="uHundredOfNanosecond">[OUT] The number of hundreds of nanoseconds, from 0 to 9.</param>
    void DecomposeTime(unsigned int &uHour, unsigned int &uMinute, unsigned int &uSecond, 
                       unsigned int &uMillisecond, unsigned int &uMicrosecond, unsigned int &uHundredOfNanosecond) const;

private:

    /// <summary>
    /// Calculates whether a year is a leap year or not.
    /// </summary>
    /// <param name="nYear">[IN] The year, positive (A.D.) or negative (B.C.).</param>
    /// <returns>
    /// True if it is a leap year; False otherwise.
    /// </returns>
    static bool IsLeapYear(const int nYear);

    /// <summary>
    /// Subtracts the offset of the time zone and the DST to the time data so it becomes UTC.
    /// </summary>
    /// <param name="instant">[IN] A time instant, with time zone offset.</param>
    /// <param name="pTimeZone">[IN] The time zone whose offset is to be subtracted. It cannot be null.</param>
    /// <returns>
    /// The instant without time zone offset applied, if any.
    /// </returns>
    QTimeSpan GetInstantWithSubtractedTimeZoneOffset(const QTimeSpan &instant, const QTimeZone* pTimeZone) const;
    
    /// <summary>
    /// Adds the offset of the time zone and the DST to the time data so it becomes Local Time.
    /// </summary>
    /// <param name="instant">[IN] A time instant, without time zone offset.</param>
    /// <param name="pTimeZone">[IN] The time zone whose offset is to be added. It cannot be null.</param>
    /// <returns>
    /// The instant with time zone offset applied, if any.
    /// </returns>
    QTimeSpan GetInstantWithAddedTimeZoneOffset(const QTimeSpan &instant, const QTimeZone* pTimeZone) const;
    
    /// <summary>
    /// Parses a complete date from a timestamp.
    /// </summary>
    /// <param name="strTimestamp">[IN] The original timestamp, which is a combination of date and time.</param>
    /// <param name="uTPosition">[IN] The position of the date/time separator in the timestamp.</param>
    /// <param name="nYear">[OUT] The year read from the timestamp.</param>
    /// <param name="uMonth">[OUT] The month read from the timestamp.</param>
    /// <param name="uDay">[OUT] The day read from the timestamp.</param>
    void ParseTimestampCompleteDate(const string_q &strTimestamp, const u32_q uTPosition, i32_q &nYear, u32_q &uMonth, u32_q &uDay) const;

    /// <summary>
    /// Parses a complete time from a timestamp.
    /// </summary>
    /// <param name="strTimestamp">[IN] The original timestamp, which is a combination of date and time.</param>
    /// <param name="uTPosition">[IN] The position of the date/time separator in the timestamp.</param>
    /// <param name="uHour">[OUT] The hour read from the timestamp.</param>
    /// <param name="uMinute">[OUT] The minute read from the timestamp.</param>
    /// <param name="uSecond">[OUT] The second read from the timestamp.</param>
    /// <param name="uMillisecond">[OUT] The millisecond read from the timestamp.</param>
    /// <param name="uMicrosecond">[OUT] The microsecond read from the timestamp.</param>
    /// <param name="uHundredOfNanosecond">[OUT] The hundred of nanosecond read from the timestamp.</param>
    /// <param name="nOffsetHours">[OUT] The offset hour read from the timestamp.</param>
    /// <param name="uOffsetMinutes">[OUT] The offset minute read from the timestamp.</param>
    void ParseTimestampCompleteTime(const string_q &strTimestamp, const u32_q uTPosition, u32_q &uHour, u32_q &uMinute, u32_q &uSecond, u32_q &uMillisecond, 
                                    u32_q &uMicrosecond, u32_q &uHundredOfNanosecond, i32_q &nOffsetHours, u32_q& uOffsetMinutes) const;

    /// <summary>
    /// Parses a complete time from the time part of a timestamp, without fraction or time offset.
    /// </summary>
    /// <param name="strTimeWithoutFraction">[IN] The time part of a timestamp, without fraction or time offset.</param>
    /// <param name="uHour">[OUT] The hour read from the timestamp.</param>
    /// <param name="uMinute">[OUT] The minute read from the timestamp.</param>
    /// <param name="uSecond">[OUT] The second read from the timestamp.</param>
    void ParseTimestampCompleteTimePart(const string_q& strTimeWithoutFraction, u32_q& uHour, u32_q& uMinute, u32_q& uSecond) const;

    /// <summary>
    /// Parses a time offset of a timestamp, including the sign. It may be incomplete and use either basic or extended formats.
    /// </summary>
    /// <param name="strTimeOffset">[IN] The time offset of a timestamp.</param>
    /// <param name="nOffsetHours">[OUT] The offset hours read from the timestamp.</param>
    /// <param name="uOffsetMinutes">[OUT] The offset minutes read from the timestamp.</param>
    void ParseTimestampTimeOffset(const string_q& strTimeOffset, i32_q& nOffsetHours, u32_q& uOffsetMinutes) const;

    /// <summary>
    /// Parses a fraction of second of a timestamp, without the initial separator.
    /// </summary>
    /// <param name="strTimeFraction">[IN] The time fraction of a timestamp, without the initial separator.</param>
    /// <param name="uMillisecond">[OUT] The milliseconds in the fraction.</param>
    /// <param name="uMicrosecond">[OUT] The microseconds in the fraction.</param>
    /// <param name="uHundredOfNanosecond">[OUT] The hundreds of nanoseconds in the fraction.</param>
    void ParseTimestampTimeFraction(const string_q& strTimeFraction, u32_q& uMillisecond, u32_q& uMicrosecond, u32_q& uHundredOfNanosecond) const;

    /// <summary>
    /// Parses a date from a timestamp that may be incomplete and uses separators (extended format).
    /// </summary>
    /// <param name="strTimestamp">[IN] The original timestamp.</param>
    /// <param name="uFirstSeparatorPosition">[IN] The position of the first date separator in the timestamp.</param>
    /// <param name="nYear">[OUT] The year read from the timestamp.</param>
    /// <param name="uMonth">[OUT] The month read from the timestamp.</param>
    /// <param name="uDay">[OUT] The day read from the timestamp. It will be equal to 1 if the day was not provided.</param>
    void ParseTimestampIncompleteDateWithSeparators(const string_q &strTimestamp, const u32_q uFirstSeparatorPosition, i32_q &nYear, u32_q &uMonth, u32_q &uDay) const;

    /// <summary>
    /// Parses a date from a timestamp that may be incomplete and does not use separators (basic format).
    /// </summary>
    /// <param name="strTimestamp">[IN] The original timestamp.</param>
    /// <param name="nYear">[OUT] The year read from the timestamp.</param>
    /// <param name="uMonth">[OUT] The month read from the timestamp. It will be equal to 1 if the month was not provided.</param>
    /// <param name="uDay">[OUT] The day read from the timestamp. It will be equal to 1 if the day was not provided.</param>
    void ParseTimestampIncompleteDateWithoutSeparators(const string_q &strTimestamp, i32_q &nYear, u32_q &uMonth, u32_q &uDay) const;

    /// <summary>
    /// Parses a time from a timestamp which may be incomplete.
    /// </summary>
    /// <param name="strTimestamp">[IN] The original timestamp, which is a time that may be incomplete, and that may not have fraction nor time offset.</param>
    /// <param name="uHour">[OUT] The hour read from the timestamp.</param>
    /// <param name="uMinute">[OUT] The minute read from the timestamp. It will be equal to 0 if the minute was not provided.</param>
    /// <param name="uSecond">[OUT] The second read from the timestamp. It will be equal to 0 if the second was not provided.</param>
    /// <param name="uMillisecond">[OUT] The millisecond read from the timestamp. It will be equal to 0 if the millisecond was not provided.</param>
    /// <param name="uMicrosecond">[OUT] The microsecond read from the timestamp. It will be equal to 0 if the microsecond was not provided.</param>
    /// <param name="uHundredOfNanosecond">[OUT] The hundred of nanosecond read from the timestamp. It will be equal to 0 if the hundred of nanosecond was not provided.</param>
    /// <param name="nOffsetHours">[OUT] The offset hour read from the timestamp. It will be equal to 0 if the offset hours were not provided.</param>
    /// <param name="uOffsetMinutes">[OUT] The offset minute read from the timestamp. It will be equal to 0 if the offset minutes were not provided.</param>
    void ParseTimestampIncompleteTime(const string_q &strTimestamp, u32_q &uHour, u32_q &uMinute, u32_q &uSecond, u32_q &uMillisecond, u32_q &uMicrosecond, 
                                      u32_q &uHundredOfNanosecond, i32_q &nOffsetHours, u32_q& uOffsetMinutes) const;

    /// <summary>
    /// Parses a time from a timestamp which may be incomplete, without time offset or fraction.
    /// </summary>
    /// <param name="strTimeWithoutFraction">[IN] The time part, which is a time that may be incomplete, and that may not have fraction nor time offset.</param>
    /// <param name="uHour">[OUT] The hour read from the timestamp.</param>
    /// <param name="uMinute">[OUT] The minute read from the timestamp. It will be equal to 0 if the minute was not provided.</param>
    /// <param name="uSecond">[OUT] The second read from the timestamp. It will be equal to 0 if the second was not provided.</param>
    void ParseTimestampIncompleteTimePart(const string_q& strTimeWithoutFraction, u32_q& uHour, u32_q& uMinute, u32_q& uSecond) const;

    /// <summary>
    /// Parses a time from a timestamp which may be incomplete, without time offset or fraction.
    /// </summary>
    /// <param name="nOffsetHours">[IN] The offset hours to apply.</param>
    /// <param name="uOffsetMinutes">[IN] The offset minutes to apply.</param>
    /// <param name="uHour">[IN] The current hour read from the timestamp to which apply the offset. It is provided not to extract it back again from the date/time instance.</param>
    /// <param name="uMinute">[IN] The current minute read from the timestamp to which apply the offset. It is provided not to extract it back again from the date/time instance.</param>
    /// <param name="dateTime">[OUT] The current date/time instance to which apply the offset.</param>
    void ApplyOffsetToTimestamp(const i32_q nOffsetHours, const u32_q uOffsetMinutes, const u32_q uHour, const u32_q uMinute, QDateTime &dateTime);

    /// <summary>
    /// Parses a time from a timestamp which may be incomplete, without time offset or fraction. If the final time overflows a day, it will be clamped to the original day of the date.
    /// </summary>
    /// <param name="nOffsetHours">[IN] The offset hours to apply.</param>
    /// <param name="uOffsetMinutes">[IN] The offset minutes to apply.</param>
    /// <param name="uHour">[IN] The current hour read from the timestamp to which apply the offset. It is provided not to extract it back again from the date/time instance.</param>
    /// <param name="uMinute">[IN] The current minute read from the timestamp to which apply the offset. It is provided not to extract it back again from the date/time instance.</param>
    /// <param name="dateTime">[OUT] The current date/time instance to which apply the offset.</param>
    void ApplyOffsetToTimestampWithoutAffectingDate(const i32_q nOffsetHours, const u32_q uOffsetMinutes, const u32_q uHour, const u32_q uMinute, QDateTime &dateTime);

    /// <summary>
    /// Transforms milliseconds, microseconds and hundreds of seconds to the fraction part of a timestamp, appending the result to the output timestamp.
    /// If all the input values equal zero, no fraction will be appended.
    /// </summary>
    /// <param name="uMillisecond">[IN] The millisecond to transform.</param>
    /// <param name="uMicrosecond">[IN] The microsecond to transform.</param>
    /// <param name="uHundredOfNanosecond">[IN] The hundred of nanosecond to transform.</param>
    /// <param name="strTimestamp">[OUT] The timestamp to which concatenate the fraction value, including the separator.</param>
    void SecondFractionToString(const unsigned int uMillisecond, const unsigned int uMicrosecond, const unsigned int uHundredOfNanosecond, string_q &strTimestamp) const;


    // PROPERTIES
    // ---------------
public:

    /// <summary>
    /// Calculates whether the year is a leap year or not.
    /// </summary>
    /// <remarks>
    /// Leap years are those divisible by 4 but not by 100, unless they are divisible by 400.<br/>
    /// The year is calculated using the local time.
    /// </remarks>
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

    /// <summary>
    /// Gets the year, in local time.
    /// </summary>
    /// <remarks>
    /// To know whether it is a negative or positive year, call IsPositive or IsNegative.<br/>
    /// Call this method if you only need to know the year; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </remarks>
    /// <returns>
    /// The absolute value of the year. If the date is undefined, the result is undefined too.
    /// </returns>
    unsigned int GetYear() const;

    /// <summary>
    /// Gets the month, in local time.
    /// </summary>
    /// <returns>
    /// The value of the month, being 1 the value that represents January and 12 for December. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the month; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetMonth() const;

    /// <summary>
    /// Gets the day, in local time.
    /// </summary>
    /// <returns>
    /// The value of the day, from 1 to 31, taking into account leap years. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the day; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetDay() const;

    /// <summary>
    /// Gets the hour, in local time.
    /// </summary>
    /// <returns>
    /// The value of the hour, from 0 to 23. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the hour; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetHour() const;

    /// <summary>
    /// Gets the minute, in local time.
    /// </summary>
    /// <returns>
    /// The value of the minute, from 0 to 59. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the minute; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetMinute() const;

    /// <summary>
    /// Gets the second, in local time.
    /// </summary>
    /// <returns>
    /// The value of the second, from 0 to 59. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the second; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetSecond() const;
    
    /// <summary>
    /// Gets the millisecond.
    /// </summary>
    /// <returns>
    /// The value of the millisecond, from 0 to 999. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the millisecond; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetMillisecond() const;
    
    /// <summary>
    /// Gets the microsecond.
    /// </summary>
    /// <returns>
    /// The value of the microsecond, from 0 to 999. If the date is undefined, the result is undefined too.<br/>
    /// Call this method if you only need to know the microsecond; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetMicrosecond() const;
    
    /// <summary>
    /// Gets the nanosecond, in hundreds.
    /// </summary>
    /// <returns>
    /// The number of hundreds of nanoseconds, from 0 to 9. If the date is undefined, 
    /// the result is undefined too.<br/>
    /// Call this method if you only need to know the hundreds of nanosecond; otherwise, consider calling Decompose method or its variations since the cost is similar.
    /// </returns>
    unsigned int GetHundredOfNanosecond() const;

    /// <summary>
    /// Gets the date and time in UTC.
    /// </summary>
    /// <returns>
    /// A copy whose time zone is null. If the date is undefined, the result is undefined too.
    /// </returns>
    QDateTime GetUtc() const;
    
    /// <summary>
    /// Gets the maximum positive date and time that can be represented (29228-09-14 02:48:05.4775807 A.D.), in UTC.
    /// </summary>
    /// <returns>
    /// The maximum positive date and time.
    /// </returns>
    static const QDateTime& GetMaxDateTime();
    
    /// <summary>
    /// Gets the maximum negative date and time that can be represented (29228-04-18 21:11:54.5224193 B.C.), in UTC.
    /// </summary>
    /// <returns>
    /// The maximum negative date and time.
    /// </returns>
    static const QDateTime& GetMinDateTime();

    /// <summary>
    /// Indicates whether the date is positive (A.D.) or not.
    /// </summary>
    /// <summary>
    /// It is calculated using the local time.
    /// </summary>
    /// <returns>
    /// True if the date is positive; False otherwise. If the date is undefined, it will return False.
    /// </returns>
    bool IsPositive() const;

    /// <summary>
    /// Indicates whether the date is negative (B.C.) or not.
    /// </summary>
    /// <summary>
    /// It is calculated using the local time.
    /// </summary>
    /// <returns>
    /// True if the date is negative; False otherwise. If the date is undefined, it will return False.
    /// </returns>
    bool IsNegative() const;

    /// <summary>
    /// Checks whether the date / time instance is undefined.
    /// </summary>
    /// <returns>
    /// True if it is undefined; False otherwise.
    /// </returns>
    bool IsUndefined() const;


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
