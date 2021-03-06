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

#include "QDateTime.h"

#include "Assertions.h"
#include "SQInteger.h"
#include "QTimeZone.h"
#include "EQComparisonType.h"

using Kinesis::QuimeraEngine::Common::DataTypes::SQInteger;
using Kinesis::QuimeraEngine::Common::DataTypes::EQComparisonType;
using Kinesis::QuimeraEngine::Common::DataTypes::char_q;

namespace Kinesis
{
namespace QuimeraEngine
{
namespace Tools
{
namespace Time
{

//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |  ATTRIBUTES INITIALIZATION |             ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

const u64_q QDateTime::HALF_VALUE = 9223372036854775808ULL;
const u64_q QDateTime::HNS_PER_MICROSECOND = 10ULL;
const u64_q QDateTime::HNS_PER_MILLISECOND = 10000ULL;
const u64_q QDateTime::HNS_PER_SECOND   = 10000000ULL;
const u64_q QDateTime::HNS_PER_MINUTE   = 600000000ULL;
const u64_q QDateTime::HNS_PER_HOUR     = 36000000000ULL;
const u64_q QDateTime::HNS_PER_DAY      = 864000000000ULL;
const u64_q QDateTime::HNS_PER_YEAR     = 315360000000000ULL;
const u64_q QDateTime::HNS_PER_LEAPYEAR = 316224000000000ULL;
const QTimeSpan QDateTime::UNDEFINED_VALUE(0ULL);


//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |       CONSTRUCTORS         |             ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

QDateTime::QDateTime() : m_instant(QTimeSpan(0)),
                         m_pTimeZone(null_q)
{
}

QDateTime::QDateTime(const QDateTime &dateTime) : m_instant(dateTime.m_instant),
                                                  m_pTimeZone(dateTime.m_pTimeZone)
{
}

QDateTime::QDateTime(const i32_q nYear, const u64_q uMonth, const u64_q uDay,
                     const u64_q uHour, const u64_q uMinute, const u64_q uSecond,
                     const u64_q uMillisecond, const u64_q uMicrosecond, const u64_q uHundredsOfNanosecond,
                     const QTimeZone* pTimeZone) : m_pTimeZone(pTimeZone)
{
    // Adjustment required to store the day 1 (Gregorian calendar) as day zero (decimal system)
    static const u64_q DAY_ZERO_ADJUSTMENT = 1ULL;
    
    // These constants represent the maximum date (+29228-09-14 02:48:05.477.580.7) and the minimum date (-29228-04-18 21:11:54.522.419.3)
    static const i32_q MAX_DATE_YEAR        = 29228;
    static const u64_q MAX_DATE_MONTH       = 9ULL;
    static const u64_q MAX_DATE_DAY         = 14ULL;
    static const u64_q MAX_DATE_HOUR        = 2ULL;
    static const u64_q MAX_DATE_MINUTE      = 48ULL;
    static const u64_q MAX_DATE_SECOND      = 5ULL;
    static const u64_q MAX_DATE_MILLISECOND = 477ULL;
    static const u64_q MAX_DATE_MICROSECOND = 580ULL;
    static const u64_q MAX_DATE_HNS         = 7ULL;
    static const i32_q MIN_DATE_YEAR        = -29228;
    static const u64_q MIN_DATE_MONTH       = 4ULL;
    static const u64_q MIN_DATE_DAY         = 18ULL;
    static const u64_q MIN_DATE_HOUR        = 21ULL;
    static const u64_q MIN_DATE_MINUTE      = 11ULL;
    static const u64_q MIN_DATE_SECOND      = 54ULL;
    static const u64_q MIN_DATE_MILLISECOND = 522ULL;
    static const u64_q MIN_DATE_MICROSECOND = 419ULL;
    static const u64_q MIN_DATE_HNS         = 3ULL;

    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_MONTH       = 12ULL;
    static const u64_q MAX_DAY         = 31ULL;
    static const u64_q MAX_HOUR        = 23ULL;
    static const u64_q MAX_MINUTE      = 59ULL;
    static const u64_q MAX_SECOND      = 59ULL;
    static const u64_q MAX_MILLISECOND = 999ULL;
    static const u64_q MAX_MICROSECOND = 999ULL;
    static const u64_q MAX_HNS         = 9ULL;

    // Checks for invalid inputs
    QE_ASSERT_ERROR(nYear != 0, "The year zero does not exist");
    QE_ASSERT_ERROR(uMonth <= MAX_MONTH, "The value of the month is greater than the maximum allowed, it must be in the range [1, 12]");
    QE_ASSERT_ERROR(uMonth != 0, "The value of the month lower than the minimum allowed, it must be in the range [1, 12]");
    QE_ASSERT_ERROR(uDay <= MAX_DAY, "The value of the day is greater than the maximum allowed, it must be in the range [1, 31]");
    QE_ASSERT_ERROR(uDay != 0, "The value of the day is lower than the minimum allowed, it must be in the range [1, 31]");
    QE_ASSERT_ERROR(uDay <= QDateTime::GetDaysInMonth(scast_q(uMonth, unsigned int), nYear), "The value of the day is not valid for the provided month in the provided year");
    QE_ASSERT_ERROR(uHour <= MAX_HOUR, "The value of the hour is not valid, it must be in the range [0, 23]");
    QE_ASSERT_ERROR(uMinute <= MAX_MINUTE, "The value of the minute is not valid, it must be in the range [0, 59]");
    QE_ASSERT_ERROR(uSecond <= MAX_SECOND, "The value of the second is not valid, it must be in the range [0, 59]");
    QE_ASSERT_ERROR(uMillisecond <= MAX_MILLISECOND, "The value of the milliseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT_ERROR(uMicrosecond <= MAX_MICROSECOND, "The value of the microseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT_ERROR(uHundredsOfNanosecond <= MAX_HNS, "The value of the hundreds of nanoseconds is not valid, it must be in the range [0, 9]");

    // Checks for representable limits
    const bool INPUT_IS_OUT_OF_POSITIVE_LIMIT = nYear > MAX_DATE_YEAR                                                         ||
                                                (nYear   ==      MAX_DATE_YEAR   && (uMonth       > MAX_DATE_MONTH            ||
                                                (uMonth  ==      MAX_DATE_MONTH  && (uDay         > MAX_DATE_DAY              ||
                                                (uDay    ==      MAX_DATE_DAY    && (uHour        > MAX_DATE_HOUR             ||
                                                (uHour   ==      MAX_DATE_HOUR   && (uMinute      > MAX_DATE_MINUTE           ||
                                                (uMinute ==      MAX_DATE_MINUTE && (uSecond      > MAX_DATE_SECOND           ||
                                                (uSecond ==      MAX_DATE_SECOND && (uMillisecond > MAX_DATE_MILLISECOND      ||
                                                (uMillisecond == MAX_DATE_MILLISECOND && (uMicrosecond > MAX_DATE_MICROSECOND ||
                                                (uMicrosecond == MAX_DATE_MICROSECOND && (uHundredsOfNanosecond > MAX_DATE_HNS))))))))))))))));

    const bool INPUT_IS_OUT_OF_NEGATIVE_LIMIT = nYear < MIN_DATE_YEAR                                                         ||
                                                (nYear   ==      MIN_DATE_YEAR   && (uMonth       < MIN_DATE_MONTH            ||
                                                (uMonth  ==      MIN_DATE_MONTH  && (uDay         < MIN_DATE_DAY              ||
                                                (uDay    ==      MIN_DATE_DAY    && (uHour        < MIN_DATE_HOUR             ||
                                                (uHour   ==      MIN_DATE_HOUR   && (uMinute      < MIN_DATE_MINUTE           ||
                                                (uMinute ==      MIN_DATE_MINUTE && (uSecond      < MIN_DATE_SECOND           ||
                                                (uSecond ==      MIN_DATE_SECOND && (uMillisecond < MIN_DATE_MILLISECOND      ||
                                                (uMillisecond == MIN_DATE_MILLISECOND && (uMicrosecond < MIN_DATE_MICROSECOND ||
                                                (uMicrosecond == MIN_DATE_MICROSECOND && (uHundredsOfNanosecond < MIN_DATE_HNS))))))))))))))));

    QE_ASSERT_WARNING(!INPUT_IS_OUT_OF_POSITIVE_LIMIT, "The input date and time is posterior to the maximum value that can be represented by this type");
    QE_ASSERT_WARNING(!INPUT_IS_OUT_OF_NEGATIVE_LIMIT, "The input date and time is anterior to the minimum value that can be represented by this type");

    if(INPUT_IS_OUT_OF_POSITIVE_LIMIT)
    {
        // If the input date and time exceed the highest limit, the result will be the maximum value
        m_instant = QTimeSpan(SQInteger::MaxU64_Q);
    }
    else if(INPUT_IS_OUT_OF_NEGATIVE_LIMIT)
    {
        // If the input date and time exceed the lowest limit, the result will be the minimum value
        m_instant = QTimeSpan(1ULL);
    }
    else
    {
        // All the input values are converted to hundreds of nanoseconds (hns)
        const u64_q MICROSECOND_AS_HNS = uMicrosecond * QDateTime::HNS_PER_MICROSECOND;
        const u64_q MILLISECOND_AS_HNS = uMillisecond * QDateTime::HNS_PER_MILLISECOND;
        const u64_q SECOND_AS_HNS = uSecond * QDateTime::HNS_PER_SECOND;
        const u64_q MINUTE_AS_HNS = uMinute * QDateTime::HNS_PER_MINUTE;
        const u64_q HOUR_AS_HNS = uHour * QDateTime::HNS_PER_HOUR;
        const u64_q DAY_AS_HNS = (uDay - DAY_ZERO_ADJUSTMENT) * QDateTime::HNS_PER_DAY;

        // Sums the days of all completed months
        u64_q uDaysInCompleteMonths = 0;

        for(unsigned int i = 1U; i < uMonth; ++i)
            uDaysInCompleteMonths += QDateTime::GetDaysInMonth(i, nYear);

        const u64_q MONTH_AS_HNS = uDaysInCompleteMonths * QDateTime::HNS_PER_DAY;

        const u64_q ABS_YEAR = scast_q(SQInteger::Abs(nYear), u64_q);

        // Number of extra days accumulated due to leap years: abs(T.YEAR - 1) / 4 - abs(T.YEAR - 1) / 100 + abs(T.YEAR - 1) / 400
        const u64_q PASSED_COMPLETE_YEARS = ABS_YEAR - 1ULL;
        const u64_q LEAP_DAYS = (PASSED_COMPLETE_YEARS / 4ULL - PASSED_COMPLETE_YEARS / 100ULL + PASSED_COMPLETE_YEARS / 400ULL);

        // Hundreds of nanoseconds in passed years: abs(T.YEAR) * nsh in 1 year + leap days * nsh in 1 day
        u64_q uYearsAsHns = ABS_YEAR * QDateTime::HNS_PER_YEAR + LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Offset applied to positive dates in order to make the year zero-based
        if(nYear > 0)
            uYearsAsHns -= QDateTime::HNS_PER_YEAR;

        // First, the last year's remaining time is summed
        const u64_q REMAINING_TIME = MONTH_AS_HNS + DAY_AS_HNS + HOUR_AS_HNS + MINUTE_AS_HNS + SECOND_AS_HNS +
                                     MILLISECOND_AS_HNS + MICROSECOND_AS_HNS + uHundredsOfNanosecond;

        // If the year is a leap year and is negative, a leap day is added.
        // In absence of leap days, all years are considered equal. Since the remaining time will be substracted to the last year, it may occur that,
        // in case that it is a leap year, the remaining time is greater than 1 common year, which would affect the next year too.
        // In order to keep the remaining time inside the last year, it must be moved 1 day to the negative side.
        // This way, the last year has 366 days and the remaining time is enclosed into it.
        //
        //         -6     -5     -4     -3     -2     -1     +1     +2
        //                              [<<<<<<<<<<<<<<<<<]
        //      |______|______|______|______|______|______|______|______| (365 days-length parts)
        //                    [>>>>>>>>]     ( <=366 days )
        //                           | |
        //
        //     -6     -5       -4       -3     -2     -1     +1     +2
        //                           |++| 1 day
        //                           [<<<<<<<<<<<<<<<<<<<<]
        //  |______|______|__________|______|______|______|______|______|
        //                [>>>>>>>>]     ( <=366 days )
        //                         | |
        //                        |++| 1 day
        //                |<-- +1 day
        if(nYear < 0 && QDateTime::IsLeapYear(nYear))
            uYearsAsHns += QDateTime::HNS_PER_DAY;

        QTimeSpan instant;

        if(nYear < 0)
            // If the year is negative, years are subtracted to the offset and days and time are added
            instant = QTimeSpan(QDateTime::HALF_VALUE - uYearsAsHns + REMAINING_TIME);
        else
            // If the year is positive, years, days and time are added to the offset
            instant = QTimeSpan(QDateTime::HALF_VALUE + uYearsAsHns + REMAINING_TIME);

        // Removes the time zone offset
        m_instant = pTimeZone == null_q ? instant :
                                          this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
    }
}

QDateTime::QDateTime(const i32_q nYear, const u64_q uMonth, const u64_q uDay, const QTimeZone* pTimeZone) :
                m_pTimeZone(pTimeZone)
{
    // Adjustment required to store the day 1 (Gregorian calendar) as day zero (decimal system)
    static const u64_q DAY_ZERO_ADJUSTMENT = 1ULL;

    // These constants represent the maximum date (+29228-09-14 02:48:05.477.580.7) and the minimum date (-29228-04-18 21:11:54.522.419.3)
    static const i32_q MAX_DATE_YEAR        = 29228;
    static const u64_q MAX_DATE_MONTH       = 9ULL;
    static const u64_q MAX_DATE_DAY         = 14ULL;
    static const i32_q MIN_DATE_YEAR        = -29228;
    static const u64_q MIN_DATE_MONTH       = 4ULL;
    static const u64_q MIN_DATE_DAY         = 19ULL; // IMPORTANT: It cannot be 18 because the time would be 00:00:00, which is anterior to 21:11:54

    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_MONTH       = 12ULL;
    static const u64_q MAX_DAY         = 31ULL;

    // Checks for invalid inputs
    QE_ASSERT_ERROR(nYear != 0, "The year zero does not exist");
    QE_ASSERT_ERROR(uMonth <= MAX_MONTH, "The value of the month is greater than the maximum allowed, it must be in the range [1, 12]");
    QE_ASSERT_ERROR(uMonth != 0, "The value of the month lower than the minimum allowed, it must be in the range [1, 12]");
    QE_ASSERT_ERROR(uDay <= MAX_DAY, "The value of the day is greater than the maximum allowed, it must be in the range [1, 31]");
    QE_ASSERT_ERROR(uDay != 0, "The value of the day is lower than the minimum allowed, it must be in the range [1, 31]");
    QE_ASSERT_ERROR(uDay <= QDateTime::GetDaysInMonth(scast_q(uMonth, unsigned int), nYear), "The value of the day is not valid for the provided month in the provided year");

    // Checks for representable limits
    const bool INPUT_IS_OUT_OF_POSITIVE_LIMIT = nYear > MAX_DATE_YEAR                                                         ||
                                                (nYear   ==      MAX_DATE_YEAR   && (uMonth       > MAX_DATE_MONTH            ||
                                                (uMonth  ==      MAX_DATE_MONTH  && (uDay         > MAX_DATE_DAY))));

    const bool INPUT_IS_OUT_OF_NEGATIVE_LIMIT = nYear < MIN_DATE_YEAR                                                         ||
                                                (nYear   ==      MIN_DATE_YEAR   && (uMonth       < MIN_DATE_MONTH            ||
                                                (uMonth  ==      MIN_DATE_MONTH  && (uDay         < MIN_DATE_DAY))));

    QE_ASSERT_WARNING(!INPUT_IS_OUT_OF_POSITIVE_LIMIT, "The input date and time is posterior to the maximum value that can be represented by this type");
    QE_ASSERT_WARNING(!INPUT_IS_OUT_OF_NEGATIVE_LIMIT, "The input date and time is anterior to the minimum value that can be represented by this type");

    if(INPUT_IS_OUT_OF_POSITIVE_LIMIT)
    {
        // If the input date and time exceed the highest limit, the result will be the maximum value
        m_instant = QTimeSpan(SQInteger::MaxU64_Q);
    }
    else if(INPUT_IS_OUT_OF_NEGATIVE_LIMIT)
    {
        // If the input date and time exceed the lowest limit, the result will be the minimum value
        m_instant = QTimeSpan(1ULL);
    }
    else
    {
        // All the input values are converted to hundreds of nanoseconds (hns)
        const u64_q DAY_AS_HNS = (uDay - DAY_ZERO_ADJUSTMENT) * QDateTime::HNS_PER_DAY;

        // Sums the days of all completed months
        u64_q uDaysInCompleteMonths = 0;

        for(unsigned int i = 1U; i < uMonth; ++i)
            uDaysInCompleteMonths += QDateTime::GetDaysInMonth(i, nYear);

        const u64_q MONTH_AS_HNS = uDaysInCompleteMonths * QDateTime::HNS_PER_DAY;

        const u64_q ABS_YEAR = scast_q(SQInteger::Abs(nYear), u64_q);

        // Number of extra days accumulated due to leap years: abs(T.YEAR - 1) / 4 - abs(T.YEAR - 1) / 100 + abs(T.YEAR - 1) / 400
        const u64_q PASSED_COMPLETE_YEARS = ABS_YEAR - 1ULL;
        const u64_q LEAP_DAYS = (PASSED_COMPLETE_YEARS / 4ULL - PASSED_COMPLETE_YEARS / 100ULL + PASSED_COMPLETE_YEARS / 400ULL);

        // Hundreds of nanoseconds in passed years: abs(T.YEAR) * nsh in 1 year + leap days * nsh in 1 day
        u64_q uYearsAsHns = ABS_YEAR * QDateTime::HNS_PER_YEAR + LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Offset applied to positive dates in order to make the year zero-based
        if(nYear > 0)
            uYearsAsHns -= QDateTime::HNS_PER_YEAR;

        // First, the last year's remaining time is summed
        const u64_q REMAINING_TIME = MONTH_AS_HNS + DAY_AS_HNS;
        
        // If the year is a leap year and is negative, a leap day is added.
        // In absence of leap days, all years are considered equal. Since the remaining time will be substracted to the last year, it may occur that,
        // in case that it is a leap year, the remaining time is greater than 1 common year, which would affect the next year too.
        // In order to keep the remaining time inside the last year, it must be moved 1 day to the negative side.
        // This way, the last year has 366 days and the remaining time is enclosed into it.
        //
        //         -6     -5     -4     -3     -2     -1     +1     +2
        //                              [<<<<<<<<<<<<<<<<<]
        //      |______|______|______|______|______|______|______|______| (365 days-length parts)
        //                    [>>>>>>>>]     ( <=366 days )
        //                           | |
        //
        //     -6     -5       -4       -3     -2     -1     +1     +2
        //                           |++| 1 day
        //                           [<<<<<<<<<<<<<<<<<<<<]
        //  |______|______|__________|______|______|______|______|______|
        //                [>>>>>>>>]     ( <=366 days )
        //                         | |
        //                        |++| 1 day
        //                |<-- +1 day
        if(nYear < 0 && QDateTime::IsLeapYear(nYear))
            uYearsAsHns += QDateTime::HNS_PER_DAY;

        QTimeSpan instant;

        if(nYear < 0)
            // If the year is negative, years are subtracted to the offset and days and time are added
            instant = QTimeSpan(QDateTime::HALF_VALUE - uYearsAsHns + REMAINING_TIME);
        else
            // If the year is positive, years, days and time are added to the offset
            instant = QTimeSpan(QDateTime::HALF_VALUE + uYearsAsHns + REMAINING_TIME);

        // Removes the time zone offset
        m_instant = pTimeZone == null_q ? instant :
                                          this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
    }
}

QDateTime::QDateTime(const u64_q uHour, const u64_q uMinute, const u64_q uSecond,
                     const u64_q uMillisecond, const u64_q uMicrosecond, const u64_q uHundredsOfNanosecond,
                     const QTimeZone* pTimeZone) : m_pTimeZone(pTimeZone)
{
    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_HOUR        = 23ULL;
    static const u64_q MAX_MINUTE      = 59ULL;
    static const u64_q MAX_SECOND      = 59ULL;
    static const u64_q MAX_MILLISECOND = 999ULL;
    static const u64_q MAX_MICROSECOND = 999ULL;
    static const u64_q MAX_HNS         = 9ULL;

    // Checks for invalid inputs
    QE_ASSERT_ERROR(uHour <= MAX_HOUR, "The value of the hour is not valid, it must be in the range [0, 23]");
    QE_ASSERT_ERROR(uMinute <= MAX_MINUTE, "The value of the minute is not valid, it must be in the range [0, 59]");
    QE_ASSERT_ERROR(uSecond <= MAX_SECOND, "The value of the second is not valid, it must be in the range [0, 59]");
    QE_ASSERT_ERROR(uMillisecond <= MAX_MILLISECOND, "The value of the milliseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT_ERROR(uMicrosecond <= MAX_MICROSECOND, "The value of the microseconds is not valid, it must be in the range [0, 999]");
    QE_ASSERT_ERROR(uHundredsOfNanosecond <= MAX_HNS, "The value of the hundreds of nanoseconds is not valid, it must be in the range [0, 9]");


    // All the input values are converted to hundreds of nanoseconds (hns)
    const u64_q MICROSECOND_AS_HNS = uMicrosecond * QDateTime::HNS_PER_MICROSECOND;
    const u64_q MILLISECOND_AS_HNS = uMillisecond * QDateTime::HNS_PER_MILLISECOND;
    const u64_q SECOND_AS_HNS = uSecond * QDateTime::HNS_PER_SECOND;
    const u64_q MINUTE_AS_HNS = uMinute * QDateTime::HNS_PER_MINUTE;
    const u64_q HOUR_AS_HNS = uHour * QDateTime::HNS_PER_HOUR;

    // First, the last year's remaining time is summed
    const u64_q REMAINING_TIME = HOUR_AS_HNS + MINUTE_AS_HNS + SECOND_AS_HNS +
                                 MILLISECOND_AS_HNS + MICROSECOND_AS_HNS + uHundredsOfNanosecond;

    QTimeSpan instant = QTimeSpan(QDateTime::HALF_VALUE + REMAINING_TIME);

    // Removes the time zone offset
    m_instant = pTimeZone == null_q ? instant :
                                      this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
}

QDateTime::QDateTime(const u64_q uHour, const u64_q uMinute, const u64_q uSecond, const u64_q uMillisecond, const QTimeZone* pTimeZone) :
                m_pTimeZone(pTimeZone)
{
    // These constants represent the maximum values for every part of the date and time
    static const u64_q MAX_HOUR        = 23ULL;
    static const u64_q MAX_MINUTE      = 59ULL;
    static const u64_q MAX_SECOND      = 59ULL;
    static const u64_q MAX_MILLISECOND = 999ULL;

    // Checks for invalid inputs
    QE_ASSERT_ERROR(uHour <= MAX_HOUR, "The value of the hour is not valid, it must be in the range [0, 23]");
    QE_ASSERT_ERROR(uMinute <= MAX_MINUTE, "The value of the minute is not valid, it must be in the range [0, 59]");
    QE_ASSERT_ERROR(uSecond <= MAX_SECOND, "The value of the second is not valid, it must be in the range [0, 59]");
    QE_ASSERT_ERROR(uMillisecond <= MAX_MILLISECOND, "The value of the milliseconds is not valid, it must be in the range [0, 999]");

    // All the input values are converted to hundreds of nanoseconds (hns)
    const u64_q MILLISECOND_AS_HNS = uMillisecond * QDateTime::HNS_PER_MILLISECOND;
    const u64_q SECOND_AS_HNS = uSecond * QDateTime::HNS_PER_SECOND;
    const u64_q MINUTE_AS_HNS = uMinute * QDateTime::HNS_PER_MINUTE;
    const u64_q HOUR_AS_HNS = uHour * QDateTime::HNS_PER_HOUR;

    // First, the last year's remaining time is summed
    const u64_q REMAINING_TIME = HOUR_AS_HNS + MINUTE_AS_HNS + SECOND_AS_HNS + MILLISECOND_AS_HNS;

    // If the year is positive, years, days and time are added to the offset
    QTimeSpan instant = QTimeSpan(QDateTime::HALF_VALUE + REMAINING_TIME);

    // Removes the time zone offset
    m_instant = pTimeZone == null_q ? instant :
                                      this->GetInstantWithSubtractedTimeZoneOffset(instant, pTimeZone);
}

QDateTime::QDateTime(const QDateTime &dateTime, const QTimeZone* pTimeZone) : m_instant(dateTime.m_instant),
                                                                              m_pTimeZone(pTimeZone)
{
}

QDateTime::QDateTime(const string_q &strTimestamp)
{
    static const char_q DATE_SEPARATOR = '-';
    static const char_q TIME_START_SEPARATOR = 'T';
    static const char_q POSITIVE_SIGN = '+';
    static const char_q NEGATIVE_SIGN = '-';

    QE_ASSERT_ERROR(!strTimestamp.IsEmpty(), "The input timestamp must not be empty.");
    QE_ASSERT_ERROR(strTimestamp.IndexOf(" ", EQComparisonType::E_BinaryCaseSensitive) == string_q::PATTERN_NOT_FOUND, "The input timestamp must not contain whitespaces.");

    i32_q nYear, nOffsetHours;
    u32_q uMonth, uDay, uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond, uOffsetMinutes;

    int nTimePosition = strTimestamp.IndexOf(TIME_START_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive);

    if(nTimePosition != string_q::PATTERN_NOT_FOUND)
    {
        // The timestamp combines Date and Time

        this->ParseTimestampCompleteDate(strTimestamp, scast_q(nTimePosition, u32_q), nYear, uMonth, uDay);
        this->ParseTimestampCompleteTime(strTimestamp, scast_q(nTimePosition, u32_q), uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond, nOffsetHours, uOffsetMinutes);

        // The date and time is constructed
        *this = QDateTime(nYear, uMonth, uDay, uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond);

        this->ApplyOffsetToTimestamp(nOffsetHours, uOffsetMinutes, uHour, uMinute, *this);
    }
    else
    {
        // The timestamp contains either Date or Time information only

        if(strTimestamp[0] == POSITIVE_SIGN || strTimestamp[0] == NEGATIVE_SIGN)
        {
            // It is preceeded by a sign, so it must be a date

            const int SEPARATOR_POSITION = strTimestamp.IndexOf(DATE_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive, 1); // Starts from 1 to skip the sign

            if(SEPARATOR_POSITION != string_q::PATTERN_NOT_FOUND)
                this->ParseTimestampIncompleteDateWithSeparators(strTimestamp, SEPARATOR_POSITION, nYear, uMonth, uDay);
            else
                this->ParseTimestampIncompleteDateWithoutSeparators(strTimestamp, nYear, uMonth, uDay);

            *this = QDateTime(nYear, uMonth, uDay);
        }
        else
        {
            // It is not preceded by a sign, so it may be either a date or a time

            const int FIRST_SEPARATOR_POSITION = strTimestamp.IndexOf(DATE_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive);

            if(FIRST_SEPARATOR_POSITION != string_q::PATTERN_NOT_FOUND)
            {
                const int SECOND_SEPARATOR_POSITION = strTimestamp.IndexOf(DATE_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive, FIRST_SEPARATOR_POSITION + 1U);

                if(SECOND_SEPARATOR_POSITION != string_q::PATTERN_NOT_FOUND)
                {
                    // It contains 2 hyphens, it must be a date
                    this->ParseTimestampIncompleteDateWithSeparators(strTimestamp, FIRST_SEPARATOR_POSITION, nYear, uMonth, uDay);
                    *this = QDateTime(nYear, uMonth, uDay);
                }
                else
                {
                    // It is interpreted as time by default, even if it does not contain colons (the hyphen is considered a minus sign of the time offset instead of a date seprator)

                    this->ParseTimestampIncompleteTime(strTimestamp, uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond, nOffsetHours, uOffsetMinutes);
                    *this = QDateTime(uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond);
                    this->ApplyOffsetToTimestampWithoutAffectingDate(nOffsetHours, uOffsetMinutes, uHour, uMinute, *this);
                }
            }
            else
            {
                // It is interpreted as time by default, even if it does not contain colons
                this->ParseTimestampIncompleteTime(strTimestamp, uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond, nOffsetHours, uOffsetMinutes);
                *this = QDateTime(uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond);
                this->ApplyOffsetToTimestampWithoutAffectingDate(nOffsetHours, uOffsetMinutes, uHour, uMinute, *this);
            }
        }
    }
}


//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |           METHODS          |             ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

QDateTime& QDateTime::operator=(const QDateTime& dateTime)
{
    if(this != &dateTime)
    {
        this->m_instant = dateTime.m_instant;
        this->m_pTimeZone = dateTime.m_pTimeZone;
    }

    return *this;
}

QDateTime& QDateTime::operator+=(const QTimeSpan &timeToAdd)
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "The date/time is undefined, the time span cannot be added");

    QE_ASSERT_WARNING(QDateTime::GetMaxDateTime().m_instant - m_instant >= timeToAdd, "The result of adding that time span exceeds the maximum date, the result will be set to the maximum date allowed");

    // Note: It assumes that the QTimeSpan class already prevents from overflow
    if(*this != QDateTime::GetUndefinedDate())
        m_instant += timeToAdd;

    return *this;
}

QDateTime& QDateTime::operator-=(const QTimeSpan &timeToSubtract)
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "The date/time is undefined, the time span cannot be subtracted");

    if(*this != QDateTime::GetUndefinedDate())
    {
        QE_ASSERT_WARNING(m_instant > timeToSubtract, "The result of subtracting that time span exceeds the minimum date, the result will be set to the minimum date allowed");

        if(m_instant <= timeToSubtract)
            m_instant = QDateTime::GetMinDateTime().m_instant;
        else
            m_instant -= timeToSubtract;
    }

    return *this;
}

QDateTime QDateTime::operator+(const QTimeSpan &timeToAdd) const
{
    QDateTime result(*this);
    result += timeToAdd;

    return result;
}

QDateTime QDateTime::operator-(const QTimeSpan &timeToSubtract) const
{
    QDateTime result(*this);
    result -= timeToSubtract;

    return result;
}

QTimeSpan QDateTime::operator-(const QDateTime &dateTime) const
{
    QE_ASSERT_ERROR(!this->IsUndefined() && !dateTime.IsUndefined(), "It is not possible to calculate the difference between undefined dates");

    if(this->IsUndefined() || dateTime.IsUndefined())
    {
        return QTimeSpan(0);
    }
    else
    {
        if(dateTime.m_instant < m_instant)
            return m_instant - dateTime.m_instant;
        else
            return dateTime.m_instant - m_instant;
    }
}

bool QDateTime::operator==(const QDateTime &dateTime) const
{
    // Other members are not checked
    return dateTime.m_instant == m_instant;
}

bool QDateTime::operator!=(const QDateTime &dateTime) const
{
    // Other members are not checked
    return dateTime.m_instant != m_instant;
}

bool QDateTime::operator<(const QDateTime &dateTime) const
{
    QE_ASSERT_ERROR(!this->IsUndefined() && !dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return this->IsUndefined() || dateTime.IsUndefined() ?
                false :
                m_instant < dateTime.m_instant;
}

bool QDateTime::operator>(const QDateTime &dateTime) const
{
    QE_ASSERT_ERROR(!this->IsUndefined() && !dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return this->IsUndefined() || dateTime.IsUndefined() ?
                false :
                m_instant > dateTime.m_instant;
}

bool QDateTime::operator<=(const QDateTime &dateTime) const
{
    QE_ASSERT_ERROR(this->IsUndefined() == dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return (!this->IsUndefined() && dateTime.IsUndefined()) || (this->IsUndefined() && !dateTime.IsUndefined()) ? // If only one operand is undefined
                false :
                m_instant <= dateTime.m_instant;
}

bool QDateTime::operator>=(const QDateTime &dateTime) const
{
    QE_ASSERT_ERROR(this->IsUndefined() == dateTime.IsUndefined(), "It is not possible to compare undefined dates");

    // Other members are not checked
    return (!this->IsUndefined() && dateTime.IsUndefined()) || (this->IsUndefined() && !dateTime.IsUndefined()) ? // If only one operand is undefined
               false :
               m_instant >= dateTime.m_instant;
}

unsigned int QDateTime::GetDaysInMonth(const unsigned int uMonth, const int nYear)
{
    static const unsigned int FEBRUARY_INDEX = 2;
    static const unsigned int MAXIMUM_MONTH_INDEX = 12;
    static const unsigned int DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    QE_ASSERT_ERROR(uMonth != 0, "The month index must be greater than 0");
    QE_ASSERT_ERROR(uMonth <= MAXIMUM_MONTH_INDEX, "The month index must be lower than 13");

    unsigned int uDays = 0;

    if(uMonth <= MAXIMUM_MONTH_INDEX)
        uDays = DAYS_IN_MONTH[uMonth - 1];

    // If the year is a leap year, add 1 to the number of days in February
    if(uMonth == FEBRUARY_INDEX && QDateTime::IsLeapYear(nYear))
        ++uDays;

    return uDays;
}

const QDateTime& QDateTime::GetUndefinedDate()
{
    static const QDateTime UNDEFINED_DATETIME;
    return UNDEFINED_DATETIME;
}

string_q QDateTime::ToString() const
{
    // Output format samples: +YYYYY-MM-DDThh:mm:ss.uuuuuuu+hh:mm
    //                        -YYYY-MM-DDThh:mm:ss.uuuuuuuZ
    //                        +YYYY-MM-DDThh:mm:ss-hh:mm

    static const string_q TIME_SEPARATOR = ":";
    static const string_q DATE_SEPARATOR = "-";
    static const string_q TIME_START_SEPARATOR = "T";
    static const string_q POSITIVE_SIGN = "+";
    static const string_q NEGATIVE_SIGN = "-";
    static const string_q ZULU_TIME = "Z";
    static const string_q ZERO_STRING = "0";

    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined date/times cannot be represented as string");

    string_q strTimestamp;
    if(this->IsNegative())
        strTimestamp.Append(NEGATIVE_SIGN);
    else
        strTimestamp.Append(POSITIVE_SIGN);

    unsigned int uYear;
    unsigned int uMonth;
    unsigned int uDay;
    unsigned int uHour;
    unsigned int uMinute;
    unsigned int uSecond;
    unsigned int uMillisecond;
    unsigned int uMicrosecond;
    unsigned int uHundredOfNanosecond;

    this->Decompose(uYear, uMonth, uDay, uHour, uMinute, uSecond, uMillisecond, uMicrosecond, uHundredOfNanosecond);

    // Negative years are adjusted so the year 1 BC, or -1, is represented by "-0000" in timestamps
    uYear = this->IsNegative() ? uYear - 1U : uYear;

    const string_q& YEAR_STRING = SQInteger::ToString(uYear);

    // Padding with zeroes
    const unsigned int YEAR_STRING_LENGTH = YEAR_STRING.GetLength();

    if(YEAR_STRING_LENGTH < 4U) // YYYYY is allowed, but padding is applied only when width is lower than 4 cyphers
    {
        for(unsigned int i = 0; i < 4U - YEAR_STRING_LENGTH; ++i)
            strTimestamp.Append(ZERO_STRING);
    }

    strTimestamp.Append(YEAR_STRING);
    strTimestamp.Append(DATE_SEPARATOR);

    // Padding with zeroes
    const unsigned int FIRST_NUMBER_WITH_2_CYPHERS = 10U;

    if(uMonth < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
        strTimestamp.Append(ZERO_STRING);

    strTimestamp.Append(SQInteger::ToString(uMonth));
    strTimestamp.Append(DATE_SEPARATOR);

    // Padding with zeroes
    if(uDay < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
        strTimestamp.Append(ZERO_STRING);

    strTimestamp.Append(SQInteger::ToString(uDay));
    strTimestamp.Append(TIME_START_SEPARATOR);

    // Padding with zeroes
    if(uHour < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
        strTimestamp.Append(ZERO_STRING);

    strTimestamp.Append(SQInteger::ToString(uHour));
    strTimestamp.Append(TIME_SEPARATOR);

    // Padding with zeroes
    if(uMinute < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
        strTimestamp.Append(ZERO_STRING);

    strTimestamp.Append(SQInteger::ToString(uMinute));
    strTimestamp.Append(TIME_SEPARATOR);

    // Padding with zeroes
    if(uSecond < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
        strTimestamp.Append(ZERO_STRING);

    strTimestamp.Append(SQInteger::ToString(uSecond));
    
    // Converts milliseconds, microseconds and nanoseconds to a fraction of second
    this->SecondFractionToString(uMillisecond, uMicrosecond, uHundredOfNanosecond, strTimestamp);

    // Adds the time offset
    if(m_pTimeZone == null_q)
    {
        strTimestamp.Append(ZULU_TIME);
    }
    else
    {
        QTimeSpan offset(0);
        bool bIsNegative = false;
        m_pTimeZone->CalculateOffset(*this, offset, bIsNegative);

        if(bIsNegative)
            strTimestamp.Append(NEGATIVE_SIGN);
        else
            strTimestamp.Append(POSITIVE_SIGN);

        static const unsigned int MINUTES_PER_HOUR = 60;
        const unsigned int OFFSET_HOURS = offset.GetHours();
        const unsigned int OFFSET_MINUTES = offset.GetMinutes() % MINUTES_PER_HOUR;

        // Padding with zeroes
        if(OFFSET_HOURS < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
            strTimestamp.Append(ZERO_STRING);

        strTimestamp.Append(SQInteger::ToString(OFFSET_HOURS));
        strTimestamp.Append(TIME_SEPARATOR);

        // Padding with zeroes
        if(OFFSET_MINUTES < FIRST_NUMBER_WITH_2_CYPHERS) // Less than 2 cyphers
            strTimestamp.Append(ZERO_STRING);

        strTimestamp.Append(SQInteger::ToString(OFFSET_MINUTES));
    }

    return strTimestamp;
}

void QDateTime::SecondFractionToString(const unsigned int uMillisecond, const unsigned int uMicrosecond, const unsigned int uHundredOfNanosecond, string_q &strTimestamp) const
{
    static const string_q SECOND_FRACTION_SEPARATOR1 = ".";
    static const string_q ZERO_STRING = "0";
    static const string_q THREE_ZERO_STRING = "000";
    static const string_q SIX_ZERO_STRING = "000000";
    bool bHasFraction = false;

    if(uMillisecond > 0)
    {
        // Has milliseconds
        bHasFraction = true;
        strTimestamp.Append(SECOND_FRACTION_SEPARATOR1);

        const string_q& strMillisecond = SQInteger::ToString(uMillisecond);

        const unsigned int MILLISECOND_LENGTH = strMillisecond.GetLength();

        for(unsigned int i = 0; i < 3U - MILLISECOND_LENGTH; ++i)
            strTimestamp.Append(ZERO_STRING);

        strTimestamp.Append(strMillisecond);

        if(uMicrosecond > 0)
        {
            // Has milliseconds and microseconds
            const string_q& strMicrosecond = SQInteger::ToString(uMicrosecond);

            const unsigned int MICROSECOND_LENGTH = strMicrosecond.GetLength();

            for(unsigned int i = 0; i < 3U - MICROSECOND_LENGTH; ++i)
                strTimestamp.Append(ZERO_STRING);

            strTimestamp.Append(strMicrosecond);

            if(uHundredOfNanosecond > 0)
            {
                // Has milliseconds, microseconds and hundreds of nanoseconds
                strTimestamp.Append(SQInteger::ToString(uHundredOfNanosecond));
            }
        }
        else
        {
            if(uHundredOfNanosecond > 0)
            {
                // Has milliseconds and hundreds of nanoseconds, no microseconds
                strTimestamp.Append(THREE_ZERO_STRING);
                strTimestamp.Append(SQInteger::ToString(uHundredOfNanosecond));
            }
        }
    }
    else
    {
        if(uMicrosecond > 0)
        {
            // Has microseconds, no milliseconds
            bHasFraction = true;
            strTimestamp.Append(SECOND_FRACTION_SEPARATOR1);
            strTimestamp.Append(THREE_ZERO_STRING);

            const string_q& strMicrosecond = SQInteger::ToString(uMicrosecond);

            const unsigned int MICROSECOND_LENGTH = strMicrosecond.GetLength();

            for(unsigned int i = 0; i < 3U - MICROSECOND_LENGTH; ++i)
                strTimestamp.Append(ZERO_STRING);

            strTimestamp.Append(strMicrosecond);

            if(uHundredOfNanosecond > 0)
            {
                // Has microseconds and hundreds of nanoseconds, no milliseconds
                strTimestamp.Append(SQInteger::ToString(uHundredOfNanosecond));
            }
        }
        else
        {
            if(uHundredOfNanosecond > 0)
            {
                // Has hundreds of nanoseconds, no milliseconds, no microseconds
                bHasFraction = true;
                strTimestamp.Append(SECOND_FRACTION_SEPARATOR1);
                strTimestamp.Append(SIX_ZERO_STRING);
                strTimestamp.Append(SQInteger::ToString(uHundredOfNanosecond));
            }
        }
    }

    if(bHasFraction)
    {
        // Removes last zeroes
        static const char_q CHARACTER_ZERO = '0';
        unsigned int uNumberOfZeroesToRemove = 0;

        string_q::QConstCharIterator it = strTimestamp.GetConstCharIterator();
        it.MoveLast();

        while(!it.IsEnd() && it.GetChar() == CHARACTER_ZERO)
        {
            --it;
            ++uNumberOfZeroesToRemove ;
        }

        if(uNumberOfZeroesToRemove > 0)
            strTimestamp = strTimestamp.Substring(0, strTimestamp.GetLength() - 1U - uNumberOfZeroesToRemove);
    }
}

void QDateTime::Decompose(unsigned int &uYear, unsigned int &uMonth, unsigned int &uDay, unsigned int &uHour, unsigned int &uMinute, unsigned int &uSecond, 
                          unsigned int &uMillisecond, unsigned int &uMicrosecond, unsigned int &uHundredOfNanosecond) const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "The date / time is undefined");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYearProvisional = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYearProvisional = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYearProvisional - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYearProvisional * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYearProvisional > 3ULL && QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(uYearProvisional > 1ULL && (uYearProvisional < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)))
        {
            // Fixes the year value
            ++uYearProvisional;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    uYear = scast_q(uYearProvisional, unsigned int);

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uMonth = 1;
        uDay = 1;
        uHour = 0;
        uMinute = 0;
        uSecond = 0;
        uMillisecond = 0;
        uMicrosecond = 0;
        uHundredOfNanosecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYearProvisional, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYearProvisional, int));
            uMonthCounter++;
        }
        
        uMonth = uMonthCounter;
        uDay = scast_q(uDaysInLastYear, unsigned int) + 1;

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        uHour = scast_q(uHoursInLastDay / HNS_PER_HOUR, unsigned int);

        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        uMinute = scast_q(uMinutesInLastHour / HNS_PER_MINUTE, unsigned int);

        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        uSecond = scast_q(uSecondsInLastMinute / HNS_PER_SECOND, unsigned int);

        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        uMillisecond = scast_q(uMillisecondsInLastSecond / HNS_PER_MILLISECOND, unsigned int);

        u64_q uMicrosecondsInLastMillisecond = uMillisecondsInLastSecond % HNS_PER_MILLISECOND;
        uMicrosecond = scast_q(uMicrosecondsInLastMillisecond / HNS_PER_MICROSECOND, unsigned int);

        uHundredOfNanosecond = scast_q(uMicrosecondsInLastMillisecond % HNS_PER_MICROSECOND, unsigned int);
    }
}

void QDateTime::DecomposeDate(unsigned int &uYear, unsigned int &uMonth, unsigned int &uDay) const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "The date is undefined");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYearProvisional = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYearProvisional = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYearProvisional - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYearProvisional * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYearProvisional > 3ULL && QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(uYearProvisional > 1ULL && (uYearProvisional < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)))
        {
            // Fixes the year value
            ++uYearProvisional;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    uYear = scast_q(uYearProvisional, unsigned int);

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uMonth = 1;
        uDay = 1;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYearProvisional, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYearProvisional, int));
            uMonthCounter++;
        }
        
        uMonth = uMonthCounter;
        uDay = scast_q(uDaysInLastYear, unsigned int) + 1;
    }
}

void QDateTime::DecomposeTime(unsigned int &uHour, unsigned int &uMinute, unsigned int &uSecond) const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "The time is undefined");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYearProvisional = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYearProvisional = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYearProvisional - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYearProvisional * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYearProvisional > 3ULL && QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(uYearProvisional > 1ULL && (uYearProvisional < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)))
        {
            // Fixes the year value
            ++uYearProvisional;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uHour = 0;
        uMinute = 0;
        uSecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }
        
        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        uHour = scast_q(uHoursInLastDay / HNS_PER_HOUR, unsigned int);

        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        uMinute = scast_q(uMinutesInLastHour / HNS_PER_MINUTE, unsigned int);

        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        uSecond = scast_q(uSecondsInLastMinute / HNS_PER_SECOND, unsigned int);
    }
}

void QDateTime::DecomposeTime(unsigned int &uHour, unsigned int &uMinute, unsigned int &uSecond, unsigned int &uMillisecond, 
                              unsigned int &uMicrosecond, unsigned int &uHundredOfNanosecond) const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "The time is undefined");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYearProvisional = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYearProvisional = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYearProvisional - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYearProvisional * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYearProvisional > 3ULL && QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        else if(uYearProvisional > 1ULL && (uYearProvisional < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYearProvisional;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYearProvisional + 1ULL, int)))
        {
            // Fixes the year value
            ++uYearProvisional;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uHour = 0;
        uMinute = 0;
        uSecond = 0;
        uMillisecond = 0;
        uMicrosecond = 0;
        uHundredOfNanosecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYearProvisional, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        uHour = scast_q(uHoursInLastDay / HNS_PER_HOUR, unsigned int);

        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        uMinute = scast_q(uMinutesInLastHour / HNS_PER_MINUTE, unsigned int);

        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        uSecond = scast_q(uSecondsInLastMinute / HNS_PER_SECOND, unsigned int);

        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        uMillisecond = scast_q(uMillisecondsInLastSecond / HNS_PER_MILLISECOND, unsigned int);

        u64_q uMicrosecondsInLastMillisecond = uMillisecondsInLastSecond % HNS_PER_MILLISECOND;
        uMicrosecond = scast_q(uMicrosecondsInLastMillisecond / HNS_PER_MICROSECOND, unsigned int);

        uHundredOfNanosecond = scast_q(uMicrosecondsInLastMillisecond % HNS_PER_MICROSECOND, unsigned int);
    }
}

bool QDateTime::IsLeapYear(const int nYear)
{
    const unsigned int ABS_YEAR = SQInteger::Abs(nYear);
    return (ABS_YEAR % 400U) == 0 || ((ABS_YEAR % 100U != 0) && (ABS_YEAR % 4U == 0));
}

QTimeSpan QDateTime::GetInstantWithSubtractedTimeZoneOffset(const QTimeSpan &instant, const QTimeZone* pTimeZone) const
{
    QDateTime localDateTime;
    localDateTime.m_instant = instant;

    QTimeSpan timeZoneOffset(0);
    bool bOffsetIsNegative = false;
    pTimeZone->CalculateOffset(localDateTime, timeZoneOffset, bOffsetIsNegative);

    // The offset is subtracted to the calculated instant to make it UTC
    if(bOffsetIsNegative)
        localDateTime.m_instant += timeZoneOffset;
    else
        localDateTime.m_instant -= timeZoneOffset;

    return localDateTime.m_instant;
}

QTimeSpan QDateTime::GetInstantWithAddedTimeZoneOffset(const QTimeSpan &instant, const QTimeZone* pTimeZone) const
{
    QDateTime localDateTime;
    localDateTime.m_instant = instant;

    QTimeSpan timeZoneOffset(0);
    bool bOffsetIsNegative = false;
    pTimeZone->CalculateOffset(localDateTime, timeZoneOffset, bOffsetIsNegative);

    // The offset is added to the calculated instant to make it UTC
    if(bOffsetIsNegative)
        localDateTime.m_instant -= timeZoneOffset;
    else
        localDateTime.m_instant += timeZoneOffset;

    return localDateTime.m_instant;
}


void QDateTime::ParseTimestampCompleteDate(const string_q &strTimestamp, const u32_q uTPosition, i32_q &nYear, u32_q &uMonth, u32_q &uDay) const
{
    static const char_q DATE_SEPARATOR = '-';
    static const char_q TIME_START_SEPARATOR = 'T';

    // Gets the Date part only
    const string_q& DATE_PART = strTimestamp.Substring(0, uTPosition - 1);

    const int FIRST_SEPARATOR_POSITION = DATE_PART.IndexOf(DATE_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive, 1U); // Starts from 1 because it may have a "-" sign at the beginning

    if(FIRST_SEPARATOR_POSITION != string_q::PATTERN_NOT_FOUND)
    {
        // The date uses separators (YYYYY-MM-DD)
        const string_q& YEAR_PART  = DATE_PART.Substring(0,                            FIRST_SEPARATOR_POSITION - 1);
        const string_q& MONTH_PART = DATE_PART.Substring(FIRST_SEPARATOR_POSITION + 1, FIRST_SEPARATOR_POSITION + 2);
        const string_q& DAY_PART   = DATE_PART.Substring(FIRST_SEPARATOR_POSITION + 4, FIRST_SEPARATOR_POSITION + 5);
        nYear  = scast_q(YEAR_PART.ToInteger(),  i32_q);
        uMonth = scast_q(MONTH_PART.ToInteger(), u32_q);
        uDay   = scast_q(DAY_PART.ToInteger(),   u32_q);
    }
    else
    {
        // The date does not use separators (YYYYYMMDD)
        unsigned int uLength = DATE_PART.GetLength();
        const string_q& DAY_PART   = DATE_PART.Substring(uLength - 2U, uLength - 1U);
        const string_q& MONTH_PART = DATE_PART.Substring(uLength - 4U, uLength - 3U);
        const string_q& YEAR_PART  = DATE_PART.Substring(0,            uLength - 5U);
        nYear  = scast_q(YEAR_PART.ToInteger(),  i32_q);
        uMonth = scast_q(MONTH_PART.ToInteger(), u32_q);
        uDay   = scast_q(DAY_PART.ToInteger(),   u32_q);
    }

    // -0000 represents the year 1 BC, or -1, so it must be adjusted
    if(nYear <= 0)
        nYear -= 1;
}

void QDateTime::ParseTimestampCompleteTime(const string_q &strTimestamp, const u32_q uTPosition, u32_q &uHour, u32_q &uMinute, u32_q &uSecond, u32_q &uMillisecond, u32_q &uMicrosecond, u32_q &uHundredOfNanosecond, i32_q &nOffsetHours, u32_q& uOffsetMinutes) const
{
    static const char_q TIME_START_SEPARATOR = 'T';
    static const char_q SECOND_FRACTION_SEPARATOR1 = '.';
    static const char_q SECOND_FRACTION_SEPARATOR2 = ',';
    static const char_q POSITIVE_SIGN = '+';
    static const char_q NEGATIVE_SIGN = '-';
    static const char_q ZULU_TIME = 'Z';

    const unsigned int FIRST_TIME_POSITION = uTPosition + 1U;

    const string_q& TIME_PART = strTimestamp.Substring(FIRST_TIME_POSITION, strTimestamp.GetLength());

    // Searches for a comma or a dot that indicate the presence of a second fraction
    int nFractionSeparatorPosition = TIME_PART.IndexOf(SECOND_FRACTION_SEPARATOR1, EQComparisonType::E_BinaryCaseSensitive);
    if(nFractionSeparatorPosition == string_q::PATTERN_NOT_FOUND)
    {
        nFractionSeparatorPosition = TIME_PART.IndexOf(SECOND_FRACTION_SEPARATOR2, EQComparisonType::E_BinaryCaseSensitive);
    }

    if(nFractionSeparatorPosition != string_q::PATTERN_NOT_FOUND)
    {
        // Contains a fraction

        const string_q& TIME_WITHOUT_FRACTION = TIME_PART.Substring(0, nFractionSeparatorPosition - 1);
        this->ParseTimestampCompleteTimePart(TIME_WITHOUT_FRACTION, uHour, uMinute, uSecond);

        // Searches for a positive or a negative sign
        int nSignPosition = TIME_PART.IndexOf(POSITIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);
        if(nSignPosition == string_q::PATTERN_NOT_FOUND)
        {
            nSignPosition = TIME_PART.IndexOf(NEGATIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);
        }

        if(nSignPosition != string_q::PATTERN_NOT_FOUND)
        {
            // Contains a positive or negative sign

            const string_q& TIME_FRACTION = TIME_PART.Substring(nFractionSeparatorPosition + 1, nSignPosition - 1);
            this->ParseTimestampTimeFraction(TIME_FRACTION, uMillisecond, uMicrosecond, uHundredOfNanosecond);
            const string_q& TIME_OFFSET = TIME_PART.Substring(nSignPosition, TIME_PART.GetLength() - 1U);
            this->ParseTimestampTimeOffset(TIME_OFFSET, nOffsetHours, uOffsetMinutes);
        }
        else
        {
            // Does not contain a positive or negative sign

            nOffsetHours   = 0;
            uOffsetMinutes = 0;

            // Searches for a Z at the end
            unsigned int uLastPosition = TIME_PART.GetLength() - 1U;
            const string_q& LAST_CHAR = TIME_PART[uLastPosition];

            // If Z exists, the last position of the time part is one position before
            if(LAST_CHAR == ZULU_TIME)
                --uLastPosition;

            const string_q& TIME_FRACTION = TIME_PART.Substring(nFractionSeparatorPosition + 1, uLastPosition);
            this->ParseTimestampTimeFraction(TIME_FRACTION, uMillisecond, uMicrosecond, uHundredOfNanosecond);
        }
    }
    else
    {
        // Does not contain a fraction
        uMillisecond = 0;
        uMicrosecond = 0;
        uHundredOfNanosecond = 0;

        // Searches for a positive or a negative sign
        int nSignPosition = TIME_PART.IndexOf(POSITIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);
        if(nSignPosition == string_q::PATTERN_NOT_FOUND)
        {
            nSignPosition = TIME_PART.IndexOf(NEGATIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);
        }

        if(nSignPosition != string_q::PATTERN_NOT_FOUND)
        {
            // Contains a positive or negative sign
            const string_q& TIME_WITHOUT_OFFSET = TIME_PART.Substring(0, nSignPosition - 1);
            this->ParseTimestampCompleteTimePart(TIME_WITHOUT_OFFSET, uHour, uMinute, uSecond);

            const string_q& TIME_OFFSET = TIME_PART.Substring(nSignPosition, TIME_PART.GetLength() - 1U);
            this->ParseTimestampTimeOffset(TIME_OFFSET, nOffsetHours, uOffsetMinutes);
        }
        else
        {
            // Does not contain a positive or negative sign
            nOffsetHours   = 0;
            uOffsetMinutes = 0;

            // Searches for a Z at the end
            unsigned int uLastPosition = TIME_PART.GetLength() - 1U;
            const string_q& LAST_CHAR = TIME_PART.Substring(uLastPosition, uLastPosition);

            // If Z exists, the last position of the time part is one position before
            if(LAST_CHAR == ZULU_TIME)
                --uLastPosition;

            this->ParseTimestampCompleteTimePart(TIME_PART.Substring(0, uLastPosition), uHour, uMinute, uSecond);
        }
    }
}

void QDateTime::ParseTimestampCompleteTimePart(const string_q& strTimeWithoutFraction, u32_q& uHour, u32_q& uMinute, u32_q& uSecond) const
{
    static const char_q TIME_SEPARATOR = ':';

    // Calculates the separator space to be skipped in the next operation, depending on whether it exists or not
    u32_q uSeparatorSpace = 0;

    if(strTimeWithoutFraction.IndexOf(TIME_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive) != string_q::PATTERN_NOT_FOUND)
        uSeparatorSpace = 1U;

    const string_q& HOUR   = strTimeWithoutFraction.Substring(0, 1U);
    const string_q& MINUTE = strTimeWithoutFraction.Substring(2U + uSeparatorSpace, 3U + uSeparatorSpace);
    const string_q& SECOND = strTimeWithoutFraction.Substring(4U + uSeparatorSpace * 2U, 5U + uSeparatorSpace * 2U);

    uHour   = scast_q(HOUR.ToInteger(),   u32_q);
    uMinute = scast_q(MINUTE.ToInteger(), u32_q);
    uSecond = scast_q(SECOND.ToInteger(), u32_q);

    // 24 is allowed in a timestamp, but not as a valid hour in this class
    if(uHour == 24)
        uHour = 0;
}

void QDateTime::ParseTimestampTimeOffset(const string_q& strTimeOffset, i32_q& nOffsetHours, u32_q& uOffsetMinutes) const
{
    static const string_q TIME_SEPARATOR = ":";

    const string_q& HOURS = strTimeOffset.Substring(0, 2);
    nOffsetHours = scast_q(HOURS.ToInteger(), u32_q);

    // If the length is greater than 3, that means that it contains more than the hour part (hh:)
    if(strTimeOffset.GetLength() > 3)
    {
        const int SEPARATOR_POSITION = strTimeOffset.IndexOf(TIME_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive);
        const u32_q SEPARATOR_WIDTH = SEPARATOR_POSITION != string_q::PATTERN_NOT_FOUND ? 1 : 0;

        const string_q& MINUTES = strTimeOffset.Substring(3U + SEPARATOR_WIDTH, 4 + SEPARATOR_WIDTH);
        uOffsetMinutes = scast_q(MINUTES.ToInteger(), u32_q);
    }
    else
    {
        uOffsetMinutes = 0;
    }
}

void QDateTime::ParseTimestampTimeFraction(const string_q& strTimeFraction, u32_q& uMillisecond, u32_q& uMicrosecond, u32_q& uHundredOfNanosecond) const
{
    static const double MAXIMUM_FRACTION_LENGTH = 7.0; // 3 numbers for milliseconds, 3 numbers for microseconds and 1 number for hundreds of nanoseconds
    const double FRACTION_LENGTH = scast_q(strTimeFraction.GetLength(), double);
    // Decimals must be multiplied by 10 N times depending on how many cyphers are there, regarding the maximum, which is set for hundreds of nanoseconds
    // For example: .0132 --> 0132 * 10^3 = 132000 hundreds of nanoseconds, where N = 3 because 7(max) - 4(length) = 3
    const unsigned int DECIMAL_OFFSET = scast_q(powl(10.0, MAXIMUM_FRACTION_LENGTH - FRACTION_LENGTH), unsigned int);

    unsigned int uHundredsOfNanoseconds = 0;

    if(FRACTION_LENGTH > MAXIMUM_FRACTION_LENGTH)
    {
        uHundredsOfNanoseconds = scast_q(strTimeFraction.Substring(0, scast_q(MAXIMUM_FRACTION_LENGTH, unsigned int) - 1U).ToInteger(), unsigned int);
    }
    else
    {
        uHundredsOfNanoseconds = scast_q(strTimeFraction.ToInteger(), unsigned int);
        uHundredsOfNanoseconds *= DECIMAL_OFFSET;
    }

    uMillisecond = uHundredsOfNanoseconds / QDateTime::HNS_PER_MILLISECOND;
    uMicrosecond = (uHundredsOfNanoseconds % QDateTime::HNS_PER_MILLISECOND) / QDateTime::HNS_PER_MICROSECOND;
    uHundredOfNanosecond = (uHundredsOfNanoseconds % QDateTime::HNS_PER_MICROSECOND);
}

void QDateTime::ParseTimestampIncompleteDateWithSeparators(const string_q &strTimestamp, const u32_q uFirstSeparatorPosition, i32_q &nYear, u32_q &uMonth, u32_q &uDay) const
{
    static const u32_q FIRST_DAY_NUMBER = 1;
    static const u32_q SEPARATOR_WIDTH = 1;
    const string_q& YEAR = strTimestamp.Substring(0, uFirstSeparatorPosition - 1U);
    const string_q& MONTH = strTimestamp.Substring(uFirstSeparatorPosition + SEPARATOR_WIDTH, uFirstSeparatorPosition + 2U * SEPARATOR_WIDTH);
    nYear  = scast_q(YEAR.ToInteger(), i32_q);
    uMonth = scast_q(MONTH.ToInteger(), u32_q);

    if(strTimestamp.GetLength() > uFirstSeparatorPosition + 2U * SEPARATOR_WIDTH)
    {
        const string_q& DAY = strTimestamp.Substring(uFirstSeparatorPosition + 4U);
        uDay = scast_q(DAY.ToInteger(), u32_q);
    }
    else
    {
        uDay = FIRST_DAY_NUMBER;
    }

    // -0000 represents the year 1 BC, or -1, so it must be adjusted
    if(nYear <= 0)
        nYear -= 1;
}

void QDateTime::ParseTimestampIncompleteDateWithoutSeparators(const string_q &strTimestamp, i32_q &nYear, u32_q &uMonth, u32_q &uDay) const
{
    static const u32_q FIRST_MONTH_NUMBER = 1;
    static const u32_q FIRST_DAY_NUMBER = 1;
    static const u32_q MAXIMUM_YEAR_SIZE_INCLUDING_SIGN = 6; // +YYYYY

    if(strTimestamp.GetLength() > MAXIMUM_YEAR_SIZE_INCLUDING_SIGN)
    {
        const u32_q LAST_POSITION = strTimestamp.GetLength() - 1U;
        const string_q& DAY = strTimestamp.Substring(LAST_POSITION - 1U, LAST_POSITION);
        const string_q& MONTH = strTimestamp.Substring(LAST_POSITION - 3U, LAST_POSITION - 2U);
        const string_q& YEAR = strTimestamp.Substring(0, LAST_POSITION - 4U);

        nYear  = scast_q(YEAR.ToInteger(), i32_q);
        uMonth = scast_q(MONTH.ToInteger(), u32_q);
        uDay   = scast_q(DAY.ToInteger(), u32_q);
    }
    else
    {
        nYear  = scast_q(strTimestamp.ToInteger(), i32_q);
        uMonth = FIRST_MONTH_NUMBER;
        uDay   = FIRST_DAY_NUMBER;
    }

    // -0000 represents the year 1 BC, or -1, so it must be adjusted
    if(nYear <= 0)
        nYear -= 1;
}

void QDateTime::ParseTimestampIncompleteTime(const string_q &strTimestamp, u32_q &uHour, u32_q &uMinute, u32_q &uSecond, u32_q &uMillisecond, 
                                             u32_q &uMicrosecond, u32_q &uHundredOfNanosecond, i32_q &nOffsetHours, u32_q& uOffsetMinutes) const
{
    static const string_q SECOND_FRACTION_SEPARATOR1 = ".";
    static const string_q SECOND_FRACTION_SEPARATOR2 = ",";
    static const string_q POSITIVE_SIGN = "+";
    static const string_q NEGATIVE_SIGN = "-";
    static const char_q ZULU_TIME = 'Z';

    const u32_q TIME_LENGTH = strTimestamp.GetLength();

    if(TIME_LENGTH > 2)
    {
        // There are more parts besides the hour

        int nFractionSeparator = strTimestamp.IndexOf(SECOND_FRACTION_SEPARATOR1, EQComparisonType::E_BinaryCaseSensitive);
        if(nFractionSeparator == string_q::PATTERN_NOT_FOUND)
            nFractionSeparator = strTimestamp.IndexOf(SECOND_FRACTION_SEPARATOR2, EQComparisonType::E_BinaryCaseSensitive);

        if(nFractionSeparator != string_q::PATTERN_NOT_FOUND)
        {
            // Contains a fraction
            const string_q& TIME_WITHOUT_FRACTION = strTimestamp.Substring(0, nFractionSeparator - 1U);
            this->ParseTimestampIncompleteTimePart(TIME_WITHOUT_FRACTION, uHour, uMinute, uSecond);

            int nSignPosition = strTimestamp.IndexOf(POSITIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);
            if(nSignPosition == string_q::PATTERN_NOT_FOUND)
                nSignPosition = strTimestamp.IndexOf(NEGATIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);

            if(nSignPosition != string_q::PATTERN_NOT_FOUND)
            {
                // Contains an offset

                const string_q& TIME_FRACTION = strTimestamp.Substring(nFractionSeparator + 1U, nSignPosition - 1U);
                this->ParseTimestampTimeFraction(TIME_FRACTION, uMillisecond, uMicrosecond, uHundredOfNanosecond);
                const string_q& TIME_OFFSET = strTimestamp.Substring(nSignPosition, TIME_LENGTH - 1U);
                this->ParseTimestampTimeOffset(TIME_OFFSET, nOffsetHours, uOffsetMinutes);
            }
            else
            {
                // Does not contain an offset
                nOffsetHours   = 0;
                uOffsetMinutes = 0;

                const u32_q Z_WIDTH = strTimestamp[TIME_LENGTH - 1U] == ZULU_TIME ? 1 : 0;
                const string_q& TIME_FRACTION = strTimestamp.Substring(nFractionSeparator + 1U, TIME_LENGTH - 1U - Z_WIDTH);
                this->ParseTimestampTimeFraction(TIME_FRACTION, uMillisecond, uMicrosecond, uHundredOfNanosecond);
            }
        }
        else
        {
            // Does not contain a fraction

            int nSignPosition = strTimestamp.IndexOf(POSITIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);
            if(nSignPosition == string_q::PATTERN_NOT_FOUND)
                nSignPosition = strTimestamp.IndexOf(NEGATIVE_SIGN, EQComparisonType::E_BinaryCaseSensitive);

            if(nSignPosition != string_q::PATTERN_NOT_FOUND)
            {
                // Contains an offset
                const string_q& TIME_WITHOUT_OFFSET = strTimestamp.Substring(0, nSignPosition - 1U);
                this->ParseTimestampIncompleteTimePart(TIME_WITHOUT_OFFSET, uHour, uMinute, uSecond);
                const string_q& TIME_OFFSET = strTimestamp.Substring(nSignPosition, TIME_LENGTH - 1U);
                this->ParseTimestampTimeOffset(TIME_OFFSET, nOffsetHours, uOffsetMinutes);
            }
            else
            {
                // Does not contain an offset
                nOffsetHours   = 0;
                uOffsetMinutes = 0;

                const u32_q Z_WIDTH = strTimestamp[TIME_LENGTH - 1U] == ZULU_TIME ? 1 : 0;

                const string_q& TIME_WITHOUT_Z = strTimestamp.Substring(0, TIME_LENGTH - 1U - Z_WIDTH);
                this->ParseTimestampIncompleteTimePart(TIME_WITHOUT_Z, uHour, uMinute, uSecond);
            }

            uMillisecond = 0;
            uMicrosecond = 0;
            uHundredOfNanosecond = 0;
        }
    }
    else
    {
        // There is information about the hour only
        const string_q& HOUR = strTimestamp.Substring(0, 1);
        uHour = scast_q(HOUR.ToInteger(), u32_q);

        // 24 is allowed in a timestamp, but not as a valid hour in this class
        if(uHour == 24)
            uHour = 0;

        uMinute = 0;
        uSecond = 0;
        uMillisecond = 0;
        uMicrosecond = 0;
        uHundredOfNanosecond = 0;
        nOffsetHours = 0;
        uOffsetMinutes = 0;
    }
}

void QDateTime::ParseTimestampIncompleteTimePart(const string_q& strTimeWithoutFraction, u32_q& uHour, u32_q& uMinute, u32_q& uSecond) const
{
    static const string_q TIME_SEPARATOR = ":";

    const u32_q TIME_LENGTH = strTimeWithoutFraction.GetLength();
    const i32_q FISRT_SEPARATOR_POSITION = strTimeWithoutFraction.IndexOf(TIME_SEPARATOR, EQComparisonType::E_BinaryCaseSensitive);

    const string_q& HOUR = strTimeWithoutFraction.Substring(0, 1);
    uHour = scast_q(HOUR.ToInteger(), u32_q);

    // 24:00 is allowed in a timestamp, but not as a valid hour in this class
    if(uHour == 24U)
        uHour = 0;

    if(FISRT_SEPARATOR_POSITION != string_q::PATTERN_NOT_FOUND)
    {
        // It uses separators

        const string_q& MINUTE = strTimeWithoutFraction.Substring(3U, 4U);
        uMinute = scast_q(MINUTE.ToInteger(), u32_q);

        // If the length is greater than 5 (hh:mm), it contains information about the second
        if(TIME_LENGTH > 5U)
        {
            const string_q& SECOND = strTimeWithoutFraction.Substring(6U, 7U);
            uSecond = scast_q(SECOND.ToInteger(), u32_q);
        }
        else
        {
            uSecond = 0;
        }
    }
    else if(TIME_LENGTH > 2U) // If the length is greater than 2 (hh), it contains information about the minute
    {
        // It does not use separators and has minutes

        const string_q& MINUTE = strTimeWithoutFraction.Substring(2U, 3U);
        uMinute = scast_q(MINUTE.ToInteger(), u32_q);

        if(TIME_LENGTH > 4U)// If the length is greater than 4 (hhmm), it contains information about the second
        {
            const string_q& SECOND = strTimeWithoutFraction.Substring(4U, 5U);
            uSecond = scast_q(SECOND.ToInteger(), u32_q);
        }
        else
        {
            uSecond = 0;
        }
    }
    else
    {
        uMinute = 0;
        uSecond = 0;
    }
}

void QDateTime::ApplyOffsetToTimestampWithoutAffectingDate(const i32_q nOffsetHours, const u32_q uOffsetMinutes, const u32_q uHour, const u32_q uMinute, QDateTime &dateTime)
{
    static const QTimeSpan HNS_PER_DAY_TIMESPAN(QDateTime::HNS_PER_DAY);

    // The offset is subtracted to the time

    const i32_q OFFSET_SIGN = nOffsetHours < 0 ? -1 : 1;
    QTimeSpan offset(QDateTime::HNS_PER_HOUR * scast_q(OFFSET_SIGN * nOffsetHours, u64_q) + QDateTime::HNS_PER_MINUTE * uOffsetMinutes);
    QTimeSpan timeInHNS(QDateTime::HNS_PER_HOUR * uHour + QDateTime::HNS_PER_MINUTE * uMinute);

    if(nOffsetHours < 0) // The offset is negative, so it will be added
    {
        if(offset + timeInHNS > HNS_PER_DAY_TIMESPAN)
            dateTime -= HNS_PER_DAY_TIMESPAN - offset; // 1 day is subtracted to keep the date being 0001-01-01
        else
            dateTime += offset;
    }
    else if(nOffsetHours > 0 || uOffsetMinutes > 0) // The offset is positive, so it will be subtracted
    {
        if(offset > timeInHNS)
            dateTime += HNS_PER_DAY_TIMESPAN - offset; // 1 day is added to keep the date being 0001-01-01
        else
            dateTime -= offset;
    }
}

void QDateTime::ApplyOffsetToTimestamp(const i32_q nOffsetHours, const u32_q uOffsetMinutes, const u32_q uHour, const u32_q uMinute, QDateTime &dateTime)
{
    // The offset is subtracted to the time

    const i32_q OFFSET_SIGN = nOffsetHours < 0 ? -1 : 1;
    QTimeSpan offset(QDateTime::HNS_PER_HOUR * scast_q(OFFSET_SIGN * nOffsetHours, u64_q) + QDateTime::HNS_PER_MINUTE * uOffsetMinutes);
    QTimeSpan timeInHNS(QDateTime::HNS_PER_HOUR * uHour + QDateTime::HNS_PER_MINUTE * uMinute);

    if(nOffsetHours < 0) // The offset is negative, so it will be added
        dateTime += offset;
    else if(nOffsetHours > 0 || uOffsetMinutes > 0) // The offset is positive, so it will be subtracted
        dateTime -= offset;
}


//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |         PROPERTIES         |             ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

bool QDateTime::IsLeapYear() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent either normal years or leap years");

    const unsigned int THIS_YEAR = this->GetYear();

    return QDateTime::IsLeapYear(scast_q(THIS_YEAR, int));
}

const QTimeZone* QDateTime::GetTimeZone() const
{
    return m_pTimeZone;
}

unsigned int QDateTime::GetYear() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent years");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    return scast_q(uYear, unsigned int);
}

unsigned int QDateTime::GetMonth() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent months");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMonth = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uMonth = 1;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        uMonth = uMonthCounter;
    }

    return scast_q(uMonth, unsigned int);
}

unsigned int QDateTime::GetDay() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent days");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uDay = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uDay = 1;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        uDay = uDaysInLastYear + 1;
    }

    return scast_q(uDay, unsigned int);
}

unsigned int QDateTime::GetHour() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent hours");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uHour = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uHour = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        uHour = uHoursInLastDay / HNS_PER_HOUR;
    }

    return scast_q(uHour, unsigned int);
}

unsigned int QDateTime::GetMinute() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent minutes");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMinute = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uMinute = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        uMinute = uMinutesInLastHour / HNS_PER_MINUTE;
    }

    return scast_q(uMinute, unsigned int);
}

unsigned int QDateTime::GetSecond() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent seconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uSecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uSecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        uSecond = uSecondsInLastMinute / HNS_PER_SECOND;
    }

    return scast_q(uSecond, unsigned int);
}

unsigned int QDateTime::GetMillisecond() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent milliseconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMillisecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uMillisecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        uMillisecond = uMillisecondsInLastSecond / HNS_PER_MILLISECOND;
    }

    return scast_q(uMillisecond, unsigned int);
}

unsigned int QDateTime::GetMicrosecond() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent microseconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uMicrosecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uMicrosecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        u64_q uMicrosecondsInLastMillisecond = uMillisecondsInLastSecond % HNS_PER_MILLISECOND;
        uMicrosecond = uMicrosecondsInLastMillisecond / HNS_PER_MICROSECOND;
    }

    return scast_q(uMicrosecond, unsigned int);
}

unsigned int QDateTime::GetHundredOfNanosecond() const
{
    QE_ASSERT_ERROR(!this->IsUndefined(), "Undefined dates cannot represent nanoseconds");

    // Adds the time zone offset
    QTimeSpan localTimeInstant = m_pTimeZone == null_q ? m_instant :
                                                         this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);

    u64_q uHundredOfNanosecond = 0;

    const u64_q HNS_IN_INSTANT = localTimeInstant.GetHundredsOfNanoseconds();

    const bool IS_NEGATIVE_DATE = HNS_IN_INSTANT < QDateTime::HALF_VALUE;

    // Depending on whether the date is negative or not, it is subtracted to the offset or vice versa
    u64_q uInputWithoutOffset = IS_NEGATIVE_DATE ?
                                                  QDateTime::HALF_VALUE - HNS_IN_INSTANT :
                                                  HNS_IN_INSTANT - QDateTime::HALF_VALUE;
    
    // If it is a positive date, the year-zero is added back
    if(!IS_NEGATIVE_DATE)
        uInputWithoutOffset += QDateTime::HNS_PER_YEAR;

    u64_q uYear = 1;
    u64_q uHnsInLastYear = uInputWithoutOffset;

    // Corner case: The year -1. Since the remaining time is substracted to the year, it will be lower than 1 year and hence the result would be zero
    if(uInputWithoutOffset >= QDateTime::HNS_PER_YEAR)
    {
        // Calculates the number of common years that have passed, including the extra days of the leap years (they are removed later)
        const u64_q NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS = uInputWithoutOffset / QDateTime::HNS_PER_YEAR;

        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS = NUMBER_OF_COMMON_YEARS_COUNTING_LEAP_DAYS - 1ULL;
        const u64_q APPROXIMATED_LEAP_DAYS = NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 4ULL - 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 100ULL + 
                                             NUMBER_OF_COMPLETE_PASSED_COMMON_YEARS_COUNTING_LEAP_DAYS / 400ULL;

        // Leap days are substracted to the original time
        const u64_q INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS = uInputWithoutOffset - APPROXIMATED_LEAP_DAYS * QDateTime::HNS_PER_DAY;

        // Counts the number of common years (extra days were substracted)
        uYear = INPUT_WITHOUT_OFFSET_WITHOUT_LEAP_DAYS / QDateTime::HNS_PER_YEAR;

        // Now we know the actual year enclosed in the input time, we can know the actual amount of leap days
        // Leap days are calculated as: (Year - 1) / 4 - (Year - 1) / 100 + (Year - 1) / 400, as leap years rule states
        const u64_q ACTUAL_PASSED_COMPLETE_YEARS = uYear - 1ULL;
        const u64_q ACTUAL_LEAP_DAYS = ACTUAL_PASSED_COMPLETE_YEARS / 4ULL - 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 100ULL + 
                                       ACTUAL_PASSED_COMPLETE_YEARS / 400ULL;

        // Gets the amount of time passed in the last year
        uHnsInLastYear = uInputWithoutOffset - ACTUAL_LEAP_DAYS * QDateTime::HNS_PER_DAY - uYear * QDateTime::HNS_PER_YEAR;
    }

    if(IS_NEGATIVE_DATE)
    {
        if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is lower than or equal to 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //            [>]-|
            //             -->
            //                [<]----|

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - (QDateTime::HNS_PER_DAY - uHnsInLastYear);
        }
        else if(uYear > 3ULL && QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)) && uHnsInLastYear <= QDateTime::HNS_PER_DAY)
        {
            // Corner case: It is a leap year and the remaining time is grater than 1 day

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|______|______|______|______|______|______|
            //                    [>>>>>>]     ( <=366 days )
            //                           |--|

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(QDateTime::IsLeapYear(scast_q(uYear, int)) && uHnsInLastYear > QDateTime::HNS_PER_DAY && uHnsInLastYear <= 2ULL * QDateTime::HNS_PER_DAY)
        {
            // Corner case: The next year is a leap year and the remaining time is greater than 1 day and lower than or equal to 2 days

            //     -6     -5     -4     -3     -2     -1     +1     +2
            //  |______|______|_______|______|______|______|______|______|
            //                |     [<<<<<<<<|<<<<<<|<<<<<<]
            //                [>>>] |*|      
            //                |   | |*|
            //                |   |
            //             [>>>]|*| <---

            // The day is inverted and displaced to the beginning of the leap year
            uHnsInLastYear -= QDateTime::HNS_PER_DAY;
            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        else if(uYear > 1ULL && (uYear < 3ULL || uHnsInLastYear != 0)) // Remaining time is zero when it is the first instant of the year
        {
            // Note: year > 1 and year < 3 could be replaced by year == 2, obviously, but this way we can relate the limits with previous parts of the algorithm

            // The year is incremented. Since the remaining time was substracted to the year, it is normally 1 less than it should
            ++uYear;
        }
        
    }
    else if(uHnsInLastYear >= QDateTime::HNS_PER_YEAR)
    {
        // It is a positive year
        // If the remaining time in the last year is equal to or greater than 1 year, it is a leap year

        // There is a corner case: The first day of a leap year
        // It could not be accurately calculated previously, it is 1 year less than it actually is
        // This acts as a flag: If adding 1 year we get a leap year, then we are in the first day
        if(QDateTime::IsLeapYear(scast_q(uYear + 1ULL, int)))
        {
            // Fixes the year value
            ++uYear;

            // Removes the year to get just the remaining time in the first day
            uHnsInLastYear -= QDateTime::HNS_PER_YEAR;
        }
    }

    if(uHnsInLastYear == 0)
    {
        // Shourcut, it is the first instant of the year
        uHundredOfNanosecond = 0;
    }
    else
    {
        if(IS_NEGATIVE_DATE)
        {
            // The last year is reversed to get the actual remaining time
            if(QDateTime::IsLeapYear(scast_q(uYear, int)))
                uHnsInLastYear = QDateTime::HNS_PER_LEAPYEAR - uHnsInLastYear;
            else
                uHnsInLastYear = QDateTime::HNS_PER_YEAR - uHnsInLastYear;
        }

        u64_q uDaysInLastYear = uHnsInLastYear / QDateTime::HNS_PER_DAY;

        static const unsigned int FIRST_MONTH_OF_THE_YEAR = 1;
        static const unsigned int LAST_MONTH_OF_THE_YEAR  = 12;

        unsigned int uMonthCounter = FIRST_MONTH_OF_THE_YEAR;

        // For every month in the year, we subtract its number of days while the days in the current month
        // are lower than or equal to the remaining days
        while(uMonthCounter <= LAST_MONTH_OF_THE_YEAR && QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int)) <= uDaysInLastYear)
        {
            uDaysInLastYear -= QDateTime::GetDaysInMonth(uMonthCounter, scast_q(uYear, int));
            uMonthCounter++;
        }

        u64_q uHoursInLastDay = uHnsInLastYear % HNS_PER_DAY;
        u64_q uMinutesInLastHour = uHoursInLastDay % HNS_PER_HOUR;
        u64_q uSecondsInLastMinute = uMinutesInLastHour % HNS_PER_MINUTE;
        u64_q uMillisecondsInLastSecond = uSecondsInLastMinute % HNS_PER_SECOND;
        u64_q uMicrosecondsInLastMillisecond = uMillisecondsInLastSecond % HNS_PER_MILLISECOND;
        uHundredOfNanosecond = uMicrosecondsInLastMillisecond % HNS_PER_MICROSECOND;
    }

    return scast_q(uHundredOfNanosecond, unsigned int);
}

QDateTime QDateTime::GetUtc() const
{
    return QDateTime(*this, null_q);
}

const QDateTime& QDateTime::GetMaxDateTime()
{
    static const QDateTime MAXIMUM_DATETIME(29228, 9, 14, 2, 48, 5, 477, 580, 7);
    return MAXIMUM_DATETIME;
}

const QDateTime& QDateTime::GetMinDateTime()
{
    static const QDateTime MINIMUM_DATETIME(-29228, 4, 18, 21, 11, 54, 522, 419, 3);
    return MINIMUM_DATETIME;
}

bool QDateTime::IsPositive() const
{
    bool bIsPositive = false;

    if(this->IsUndefined())
    {
        QE_ASSERT_ERROR(false, "The date is undefined, it cannot be positive");
    }
    else if(m_pTimeZone == null_q)
    {
        bIsPositive = m_instant.GetHundredsOfNanoseconds() >= QDateTime::HALF_VALUE;
    }
    else
    {
        // Adds the time zone offset before checking
        QTimeSpan localTime = m_instant;
        localTime = this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);
        bIsPositive = localTime.GetHundredsOfNanoseconds() >= QDateTime::HALF_VALUE;
    }

    return bIsPositive;
}

bool QDateTime::IsNegative() const
{
    bool bIsNegative = false;

    if(this->IsUndefined())
    {
        QE_ASSERT_ERROR(false, "The date is undefined, it cannot be negative");
    }
    else if(m_pTimeZone == null_q)
    {
        bIsNegative = m_instant.GetHundredsOfNanoseconds() < QDateTime::HALF_VALUE;
    }
    else
    {
        // Adds the time zone offset before checking
        QTimeSpan localTime = m_instant;
        localTime = this->GetInstantWithAddedTimeZoneOffset(m_instant, m_pTimeZone);
        bIsNegative = localTime.GetHundredsOfNanoseconds() < QDateTime::HALF_VALUE;
    }

    return bIsNegative;
}

bool QDateTime::IsUndefined() const
{
    return m_instant == QDateTime::UNDEFINED_VALUE;
}

} //namespace Time
} //namespace Tools
} //namespace QuimeraEngine
} //namespace Kinesis
