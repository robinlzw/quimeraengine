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

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
using namespace boost::unit_test;

#include "../../testsystem/TestingExternalDefinitions.h"

#include "QStringUnicode.h"
#include "QAssertException.h"

using Kinesis::QuimeraEngine::Common::Exceptions::QAssertException;
using Kinesis::QuimeraEngine::Common::DataTypes::QStringUnicode;



QTEST_SUITE_BEGIN( QConstCharIterator_TestSuite )

/// <summary>
/// Checks that the iterator points to the first position when using a common string.
/// </summary>
QTEST_CASE ( Constructor1_IteratorPointsToFirstPositionWhenUsingCommonString_Test )
{
    // [Preparation]
    const QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator EXPECTED_ITERATOR(SOURCE_STRING);
    EXPECTED_ITERATOR.MoveFirst();

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
    
    // [Verification]
    BOOST_CHECK(iterator == EXPECTED_ITERATOR);
}

/// <summary>
/// Checks that the iterator points to the end position when using an empty string.
/// </summary>
QTEST_CASE ( Constructor1_IteratorPointsToEndPositionWhenUsingEmptyString_Test )
{
    // [Preparation]
    const QStringUnicode SOURCE_STRING("");
    const bool IS_END = true;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
    
    // [Verification]
    BOOST_CHECK_EQUAL(iterator.IsEnd(), IS_END);
}

/// <summary>
/// Checks that the iterator is correctly copied when using a common input interator.
/// </summary>
QTEST_CASE ( Constructor2_IteratorIsCorrectlyCopiedWhenUsingCommonIterator_Test )
{
    // [Preparation]
    const QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator EXPECTED_ITERATOR(SOURCE_STRING);
    ++EXPECTED_ITERATOR;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(EXPECTED_ITERATOR);
    
    // [Verification]
    BOOST_CHECK(iterator == EXPECTED_ITERATOR);
}

/// <summary>
/// Checks that the iterator is correctly copied when using an invalid input interator.
/// </summary>
QTEST_CASE ( Constructor2_IteratorIsCorrectlyCopiedWhenUsingInvalidIterator_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator EXPECTED_ITERATOR(SOURCE_STRING);
    EXPECTED_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(EXPECTED_ITERATOR);
    
    // [Verification]
    BOOST_CHECK(!iterator.IsValid());
}

/// <summary>
/// Checks that the iterator points to the given position.
/// </summary>
QTEST_CASE ( Constructor3_IteratorPointsToTheGivenPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    const QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    const QCharUnicode EXPECTED_CHARACTER = 'F';

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING, 5);
    
    // [Verification]
    QCharUnicode charPointedToByIterator = iterator.GetChar();
    BOOST_CHECK(charPointedToByIterator == EXPECTED_CHARACTER);
}

/// <summary>
/// Checks that the iterator points to end position (forward) when input initial position is out of bounds.
/// </summary>
QTEST_CASE ( Constructor3_IteratorPointsToEndPositionWhenInitialPositionIsOutOfBounds_Test )
{
    // [Preparation]
    const QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    const bool POINTS_TO_END_POSITION = true;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING, SOURCE_STRING.GetLength());
    
    // [Verification]
    bool bPointsToEndPosition = iterator.IsEnd();
    BOOST_CHECK_EQUAL(bPointsToEndPosition, POINTS_TO_END_POSITION);
}

/// <summary>
/// Checks that the iterator points to end position (forward) when the string is empty.
/// </summary>
QTEST_CASE ( Constructor3_IteratorPointsToEndPositionWhenStringIsEmpty_Test )
{
    // [Preparation]
    const QStringUnicode SOURCE_STRING = QStringUnicode::GetEmpty();
    const bool POINTS_TO_END_POSITION = true;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING, 0);
    
    // [Verification]
    bool bPointsToEndPosition = iterator.IsEnd();
    BOOST_CHECK_EQUAL(bPointsToEndPosition, POINTS_TO_END_POSITION);
}

/// <summary>
/// Checks that the iterator points to the right position when using characters from the Supplementary Multilingual Plane (SMP).
/// </summary>
QTEST_CASE ( Constructor3_IteratorStepsOncePerCodePoint_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING(QCharUnicode(0x00010300)); // A character from the SMP (will use more than one code unit internally)
    SOURCE_STRING.Append("X");
    SOURCE_STRING.Append(QCharUnicode(0x00010400));
    const QCharUnicode EXPECTED_CHARACTER(QCharUnicode(0x00010400)); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING, 2U);
    
    // [Verification]
    QCharUnicode currentChar = iterator.GetChar();
    BOOST_CHECK(currentChar == EXPECTED_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR++;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the iterator already points to the end position.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_AssertionFailsWhenIteratorAlreadyPointsToLastEndPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR_END++;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator does not change when it already points to the end position.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_IteratorDoesNotChangeWhenItAlreadyPointsToLastEndPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ITERATOR_END);
    iterator++;
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_END);
}

#endif

/// <summary>
/// Checks that the iterator steps forward properly and returns the previous state.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_CommonIteratorStepsForwardProperlyAndReturnsPreviousState_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveFirst();
    const QCharUnicode SECOND_CHARACTER('B'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorPreviousState = iterator++;
    
    // [Verification]
    BOOST_CHECK(iteratorPreviousState == ORIGINAL_ITERATOR);
    BOOST_CHECK(iterator.GetChar() == SECOND_CHARACTER);
}

/// <summary>
/// Checks that the iterator steps once per code point, and not per code unit.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_IteratorStepsOncePerCodePoint_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING(QCharUnicode(0x00010300)); // A character from the SMP (will use more than one code unit internally)
    SOURCE_STRING.Append("X");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    const QCharUnicode EXPECTED_CHARACTER('X'); 

	// [Execution]
    ITERATOR++;
    
    // [Verification]
    QCharUnicode currentChar = ITERATOR.GetChar();
    BOOST_CHECK(currentChar == EXPECTED_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR--;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the iterator already points to the end position before the first one.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_AssertionFailsWhenIteratorAlreadyPointsToEndPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveFirst();
    ITERATOR_END--;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR_END--;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator does not change when it already points to the end position.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_IteratorDoesNotChangeWhenItAlreadyPointsToEndPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveFirst();
    ITERATOR_END--;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ITERATOR_END);
    iterator--;
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_END);
}

#endif

/// <summary>
/// Checks that the iterator steps backward properly and returns the previous state.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_CommonIteratorStepsBackwardProperlyAndReturnsPreviousState_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveLast();
    const QCharUnicode BEFORE_LAST_CHARACTER('M'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorPreviousState = iterator--;
    
    // [Verification]
    BOOST_CHECK(iteratorPreviousState == ORIGINAL_ITERATOR);
    BOOST_CHECK(iterator.GetChar() == BEFORE_LAST_CHARACTER);
}

/// <summary>
/// Checks that the iterator points to the last position and returns the previous state when it was pointing to the end position after the last one.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_IteratorPointsToLastPositionAndReturnsPreviousStateWhenItWasPointingToLastEndPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveLast();
    ++ORIGINAL_ITERATOR;
    const QCharUnicode LAST_CHARACTER('N'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorPreviousState = iterator--;
    
    // [Verification]
    BOOST_CHECK(iteratorPreviousState == ORIGINAL_ITERATOR);
    BOOST_CHECK(iterator.GetChar() == LAST_CHARACTER);
}

/// <summary>
/// Checks that the iterator steps once per code point, and not per code unit.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_IteratorStepsOncePerCodePoint_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING(QCharUnicode(0x00010300)); // A character from the SMP (will use more than one code unit internally)
    SOURCE_STRING.Append("X");
    SOURCE_STRING.Append(QCharUnicode(0x00010400));
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING, 2U);
    const QCharUnicode EXPECTED_CHARACTER(QCharUnicode(0x00010300)); 

	// [Execution]
    ITERATOR--;
    ITERATOR--;
    
    // [Verification]
    QCharUnicode currentChar = ITERATOR.GetChar();
    BOOST_CHECK(currentChar == EXPECTED_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ++INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the iterator already points to the end position.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_AssertionFailsWhenIteratorAlreadyPointsToLastEndPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ++ITERATOR_END;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator does not change when it already points to the end position.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_IteratorDoesNotChangeWhenItAlreadyPointsToLastEndPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ITERATOR_END);
    ++iterator;
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_END);
}

#endif

/// <summary>
/// Checks that the iterator steps forward properly and returns the current state.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_CommonIteratorStepsForwardProperlyAndReturnsCurrentState_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveFirst();
    const QCharUnicode SECOND_CHARACTER('B'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorCurrentState = ++iterator;
    
    // [Verification]
    BOOST_CHECK(iteratorCurrentState.GetChar() == SECOND_CHARACTER);
    BOOST_CHECK(iterator.GetChar() == SECOND_CHARACTER);
}

/// <summary>
/// Checks that the iterator points to the first position and returns the current state when it was pointing to the end position before the first one.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_IteratorPointsToFirstPositionAndReturnsCurrentStateWhenItWasPointingToPositionBeforeFirst_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveFirst();
    ORIGINAL_ITERATOR--;
    const QCharUnicode FIRST_CHARACTER('A'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorCurrentState = ++iterator;
    
    // [Verification]
    BOOST_CHECK(iteratorCurrentState.GetChar() == FIRST_CHARACTER);
    BOOST_CHECK(iterator.GetChar() == FIRST_CHARACTER);
}

/// <summary>
/// Checks that the iterator steps once per code point, and not per code unit.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_IteratorStepsOncePerCodePoint_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING(QCharUnicode(0x00010300)); // A character from the SMP (will use more than one code unit internally)
    SOURCE_STRING.Append("X");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    const QCharUnicode EXPECTED_CHARACTER('X'); 

	// [Execution]
    ++ITERATOR;
    
    // [Verification]
    QCharUnicode currentChar = ITERATOR.GetChar();
    BOOST_CHECK(currentChar == EXPECTED_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        --INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the iterator already points to the end position before the first one.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_AssertionFailsWhenIteratorAlreadyPointsToEndPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveFirst();
    --ITERATOR_END;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        --ITERATOR_END;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator does not change when it already points to the end position.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_IteratorDoesNotChangeWhenItAlreadyPointsToEndPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_END(SOURCE_STRING);
    ITERATOR_END.MoveFirst();
    --ITERATOR_END;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ITERATOR_END);
    --iterator;
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_END);
}

#endif

/// <summary>
/// Checks that the iterator steps backward properly and returns the current state.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_CommonIteratorStepsBackwardProperlyAndReturnsCurrentState_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveLast();
    const QCharUnicode BEFORE_LAST_CHARACTER('M'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorCurrentState = --iterator;
    
    // [Verification]
    BOOST_CHECK(iteratorCurrentState.GetChar() == BEFORE_LAST_CHARACTER);
    BOOST_CHECK(iterator.GetChar() == BEFORE_LAST_CHARACTER);
}

/// <summary>
/// Checks that the iterator points to the last position and returns the current state when it was pointing to the end position after the last one.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_IteratorPointsToLastPositionAndReturnsCurrentStateWhenItWasPointingToLastEndPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR.MoveLast();
    ++ORIGINAL_ITERATOR;
    const QCharUnicode LAST_CHARACTER('N'); 

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QConstCharIterator iteratorCurrentState = --iterator;
    
    // [Verification]
    BOOST_CHECK(iteratorCurrentState.GetChar() == LAST_CHARACTER);
    BOOST_CHECK(iterator.GetChar() == LAST_CHARACTER);
}

/// <summary>
/// Checks that the iterator steps once per code point, and not per code unit.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_IteratorStepsOncePerCodePoint_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING(QCharUnicode(0x00010300)); // A character from the SMP (will use more than one code unit internally)
    SOURCE_STRING.Append("X");
    SOURCE_STRING.Append(QCharUnicode(0x00010400));
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING, 2U);
    const QCharUnicode EXPECTED_CHARACTER(QCharUnicode(0x00010300)); 

	// [Execution]
    --ITERATOR;
    --ITERATOR;
    
    // [Verification]
    QCharUnicode currentChar = ITERATOR.GetChar();
    BOOST_CHECK(currentChar == EXPECTED_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorAssignation_AssertionFailsWhenInputIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator = INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorAssignation_AssertionFailsWhenInputIteratorPointsToDifferentString_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator = ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator is copied when it is not valid.
/// </summary>
QTEST_CASE ( OperatorAssignation_IteratorIsCopiedWhenInputIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
    iterator = INVALID_ITERATOR;
    
    // [Verification]
    BOOST_CHECK(iterator == INVALID_ITERATOR);
}

/// <summary>
/// Checks that the iterator is not copied when it points to a different string.
/// </summary>
QTEST_CASE ( OperatorAssignation_IteratorDoesNotChangeIfInputIteratorPointsToDifferentString_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);

	// [Execution]
    QStringUnicode::QConstCharIterator iterator = ITERATOR_B;
    iterator = ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK(iterator != ITERATOR_A);
    BOOST_CHECK(iterator == ITERATOR_B);
}

#endif

/// <summary>
/// Checks that a common iterator is copied.
/// </summary>
QTEST_CASE ( OperatorAssignation_InputIteratorIsCorrectlyCopied_Test )
{
    // [Preparation]
    QStringUnicode STRING_SOURCE("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_SOURCE);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_SOURCE);

	// [Execution]
    QStringUnicode::QConstCharIterator iterator = ITERATOR_B;
    iterator = ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_A);
}

/// <summary>
/// Checks that input iterator is copied when resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorAssignation_InputIteratorIsCorrectlyCopiedWhenResidentIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    QStringUnicode::QConstCharIterator VALID_ITERATOR(SOURCE_STRING);

	// [Execution]
    QStringUnicode::QConstCharIterator iterator = INVALID_ITERATOR;
    iterator = VALID_ITERATOR;
    
    // [Verification]
    BOOST_CHECK(iterator.IsValid());
    BOOST_CHECK(iterator == VALID_ITERATOR);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when either the input or the resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorEquality_AssertionFailsWhenIteratorsAreNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed1 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator == INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed1 = true;
    }
    
    bool bAssertionFailed2 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        INVALID_ITERATOR == iterator;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed2 = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed1, ASSERTION_FAILED);
    BOOST_CHECK_EQUAL(bAssertionFailed2, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorEquality_AssertionFailsWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator == ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when iterators point to a different string.
/// </summary>
QTEST_CASE ( OperatorEquality_ReturnsFalseWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A == ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns True when iterators are equal.
/// </summary>
QTEST_CASE ( OperatorEquality_ReturnsTrueWhenIteratorsAreEqual_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_A == ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when iterators are not equal.
/// </summary>
QTEST_CASE ( OperatorEquality_ReturnsFalseWhenIteratorsAreNotEqual_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;

    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A == ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when either the input or the resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorInequality_AssertionFailsWhenIteratorsAreNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed1 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator != INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed1 = true;
    }
    
    bool bAssertionFailed2 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        INVALID_ITERATOR != iterator;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed2 = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed1, ASSERTION_FAILED);
    BOOST_CHECK_EQUAL(bAssertionFailed2, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorInequality_AssertionFailsWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator != ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when iterators point to a different string.
/// </summary>
QTEST_CASE ( OperatorInequality_ReturnsTrueWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_A != ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns False when iterators are equal.
/// </summary>
QTEST_CASE ( OperatorInequality_ReturnsFalseWhenIteratorsAreEqual_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A != ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns True when iterators are not equal.
/// </summary>
QTEST_CASE ( OperatorInequality_ReturnsTrueWhenIteratorsAreNotEqual_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;

    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_A != ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when either the input or the resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorGreaterThan_AssertionFailsWhenIteratorsAreNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed1 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator > INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed1 = true;
    }
    
    bool bAssertionFailed2 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        INVALID_ITERATOR > iterator;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed2 = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed1, ASSERTION_FAILED);
    BOOST_CHECK_EQUAL(bAssertionFailed2, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorGreaterThan_AssertionFailsWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator > ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when iterators point to a different string.
/// </summary>
QTEST_CASE ( OperatorGreaterThan_ReturnsFalseWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A > ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns True when the resident iterator is greater than the input iterator.
/// </summary>
QTEST_CASE ( OperatorGreaterThan_ReturnsTrueWhenResidentIteratorIsGreaterThanInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_B > ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when the resident iterator is not greater than the input iterator.
/// </summary>
QTEST_CASE ( OperatorGreaterThan_ReturnsFalseWhenResidentIteratorIsNotGreaterThanInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A > ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when either the input or the resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorLowerThan_AssertionFailsWhenIteratorsAreNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed1 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator < INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed1 = true;
    }
    
    bool bAssertionFailed2 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        INVALID_ITERATOR < iterator;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed2 = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed1, ASSERTION_FAILED);
    BOOST_CHECK_EQUAL(bAssertionFailed2, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorLowerThan_AssertionFailsWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator < ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when iterators point to a different string.
/// </summary>
QTEST_CASE ( OperatorLowerThan_ReturnsFalseWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A < ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns True when the resident iterator is lower than the input iterator.
/// </summary>
QTEST_CASE ( OperatorLowerThan_ReturnsTrueWhenResidentIteratorIsLowerThanInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_A < ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when the resident iterator is not lower than the input iterator.
/// </summary>
QTEST_CASE ( OperatorLowerThan_ReturnsFalseWhenResidentIteratorIsNotLowerThanInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_B < ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when either the input or the resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorGreaterThanOrEquals_AssertionFailsWhenIteratorsAreNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed1 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator >= INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed1 = true;
    }
    
    bool bAssertionFailed2 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        INVALID_ITERATOR >= iterator;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed2 = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed1, ASSERTION_FAILED);
    BOOST_CHECK_EQUAL(bAssertionFailed2, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorGreaterThanOrEquals_AssertionFailsWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator >= ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when iterators point to a different string.
/// </summary>
QTEST_CASE ( OperatorGreaterThanOrEquals_ReturnsFalseWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A >= ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns True when the resident iterator is greater than the input iterator.
/// </summary>
QTEST_CASE ( OperatorGreaterThanOrEquals_ReturnsTrueWhenResidentIteratorIsGreaterThanInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_B >= ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns True when the resident iterator is equal to the input iterator.
/// </summary>
QTEST_CASE ( OperatorGreaterThanOrEquals_ReturnsTrueWhenResidentIteratorEqualsInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_B >= ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when the resident iterator is neither greater than nor equals to the input iterator.
/// </summary>
QTEST_CASE ( OperatorGreaterThanOrEquals_ReturnsFalseWhenResidentIteratorIsNotGreaterThanAndDoNotEqualsInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_B;
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A >= ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when either the input or the resident iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorLowerThanOrEquals_AssertionFailsWhenIteratorsAreNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed1 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        iterator <= INVALID_ITERATOR;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed1 = true;
    }
    
    bool bAssertionFailed2 = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(SOURCE_STRING);
        INVALID_ITERATOR <= iterator;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed2 = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed1, ASSERTION_FAILED);
    BOOST_CHECK_EQUAL(bAssertionFailed2, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the input iterator points to a different string.
/// </summary>
QTEST_CASE ( OperatorLowerThanOrEquals_AssertionFailsWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QConstCharIterator iterator(STRING_B);
        iterator <= ITERATOR_A;
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when iterators point to a different string.
/// </summary>
QTEST_CASE ( OperatorLowerThanOrEquals_ReturnsFalseWhenIteratorsPointToDifferentStrings_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode STRING_B("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_B);
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A <= ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns True when the resident iterator is lower than the input iterator.
/// </summary>
QTEST_CASE ( OperatorLowerThanOrEquals_ReturnsTrueWhenResidentIteratorIsLowerThanInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_A;
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_B <= ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns True when the resident iterator is equal to the input iterator.
/// </summary>
QTEST_CASE ( OperatorLowerThanOrEquals_ReturnsTrueWhenResidentIteratorEqualsInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR_B <= ITERATOR_A;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when the resident iterator is neither greater than nor equals to the input iterator.
/// </summary>
QTEST_CASE ( OperatorLowerThanOrEquals_ReturnsFalseWhenResidentIteratorIsNotLowerThanAndDoNotEqualsInputIterator_Test )
{
    // [Preparation]
    QStringUnicode STRING_A("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QConstCharIterator ITERATOR_B(STRING_A);
    ++ITERATOR_A;
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR_A <= ITERATOR_B;
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( IsEnd_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR.IsEnd();
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns False when the iterator is not valid.
/// </summary>
QTEST_CASE ( IsEnd_ReturnsFalseWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = INVALID_ITERATOR.IsEnd();
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#endif

/// <summary>
/// Checks that it returns True when the iterator points to the position before the first.
/// </summary>
QTEST_CASE ( IsEnd_ReturnsTrueWhenIteratorPointsToPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveFirst();
    ITERATOR--;

    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR.IsEnd();
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns True when the iterator points to the position after the last.
/// </summary>
QTEST_CASE ( IsEnd_ReturnsTrueWhenIteratorPointsToPositionAfterLast_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveLast();
    ++ITERATOR;

    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR.IsEnd();
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when the iterator does not point to an end position.
/// </summary>
QTEST_CASE ( IsEnd_ReturnsFalseWhenIteratorDoesNotPointToEndPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveLast();

    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR.IsEnd();
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( GetChar_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR.GetChar();
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the iterator points to end position.
/// </summary>
QTEST_CASE ( GetChar_AssertionFailsWhenIteratorPointsToEndPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveLast();
    ++ITERATOR;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR.GetChar();
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that it returns the Unicode code point U+FFFF when iterator points to an end position.
/// </summary>
QTEST_CASE ( GetChar_ReturnsUFFFFWhenIteratorPointsToEndPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveLast();
    ++ITERATOR;
    const QCharUnicode NONCHARACTER = 0xFFFF;

	// [Execution]
    QCharUnicode character = ITERATOR.GetChar();
    
    // [Verification]
    BOOST_CHECK(character == NONCHARACTER);
}

#endif

/// <summary>
/// Checks that it returns the expected character when the iterator points to a common position.
/// </summary>
QTEST_CASE ( GetChar_ReturnsExpectedCharacterWhenIteratorPointsCommonPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveFirst();
    ++ITERATOR;
    ++ITERATOR;
    const QCharUnicode EXPECTED_CHARACTER = 'C';

	// [Execution]
    QCharUnicode character = ITERATOR.GetChar();
    
    // [Verification]
    BOOST_CHECK(character == EXPECTED_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the string is empty.
/// </summary>
QTEST_CASE ( MoveFirst_AssertionFailsWhenStringIsEmpty_Test )
{
    // [Preparation]
    QStringUnicode EMPTY_STRING("");
    QStringUnicode::QConstCharIterator ITERATOR(EMPTY_STRING);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR.MoveFirst();
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator points to the end position when the string is empty.
/// </summary>
QTEST_CASE ( MoveFirst_IteratorPointsToEndPositionWhenStringIsEmpty_Test )
{
    // [Preparation]
    QStringUnicode EMPTY_STRING("");
    QStringUnicode::QConstCharIterator END_ITERATOR(EMPTY_STRING);
    END_ITERATOR.MoveLast();
    ++END_ITERATOR;
    QStringUnicode::QConstCharIterator ITERATOR(EMPTY_STRING);

	// [Execution]
    ITERATOR.MoveFirst();
    
    // [Verification]
    BOOST_CHECK(ITERATOR == END_ITERATOR);
}

#endif

/// <summary>
/// Checks that the iterator points to the first position when the string is not empty.
/// </summary>
QTEST_CASE ( MoveFirst_IteratorPointsToFirstPositionWhenStringIsNotEmpty_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ++ORIGINAL_ITERATOR;
    ++ORIGINAL_ITERATOR;
    ++ORIGINAL_ITERATOR;
    QStringUnicode::QConstCharIterator ITERATOR_FIRST(SOURCE_STRING);

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    iterator.MoveFirst();
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_FIRST);
}

/// <summary>
/// Checks that the iterator points to the first position even when it was not valid.
/// </summary>
QTEST_CASE ( MoveFirst_IteratorPointsToFirstPositionWhenStringIsNotEmptyAndIteratorWasNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR_FIRST(SOURCE_STRING);
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = "ABC";

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(INVALID_ITERATOR);
    iterator.MoveFirst();
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_FIRST);
}

/// <summary>
/// Checks that the iterator points to the first position even when it was pointing to the position before the first.
/// </summary>
QTEST_CASE ( MoveFirst_IteratorPointsToFirstPositionWhenStringIsNotEmptyAndIteratorWasPointingToPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCDEFGHIJKMN");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR--;

    QStringUnicode::QConstCharIterator ITERATOR_FIRST(SOURCE_STRING);

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    iterator.MoveFirst();
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_FIRST);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the string is empty.
/// </summary>
QTEST_CASE ( MoveLast_AssertionFailsWhenStringIsEmpty_Test )
{
    // [Preparation]
    QStringUnicode EMPTY_STRING("");
    QStringUnicode::QConstCharIterator ITERATOR(EMPTY_STRING);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR.MoveLast();
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that the iterator points to the end position when the string is empty.
/// </summary>
QTEST_CASE ( MoveLast_IteratorPointsToEndPositionWhenStringIsEmpty_Test )
{
    // [Preparation]
    QStringUnicode EMPTY_STRING("");
    QStringUnicode::QConstCharIterator END_ITERATOR(EMPTY_STRING);
    END_ITERATOR.MoveLast();
    ++END_ITERATOR;
    QStringUnicode::QConstCharIterator ITERATOR(EMPTY_STRING);

	// [Execution]
    ITERATOR.MoveLast();
    
    // [Verification]
    BOOST_CHECK(ITERATOR == END_ITERATOR);
}

#endif

/// <summary>
/// Checks that the iterator points to the last position when the string is not empty.
/// </summary>
QTEST_CASE ( MoveLast_IteratorPointsToLastPositionWhenStringIsNotEmpty_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABC");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    QStringUnicode::QConstCharIterator ITERATOR_LAST(SOURCE_STRING);
    ++ITERATOR_LAST;
    ++ITERATOR_LAST;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    iterator.MoveLast();
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_LAST);
}

/// <summary>
/// Checks that the iterator points to the last position even when it was not valid.
/// </summary>
QTEST_CASE ( MoveLast_IteratorPointsToLastPositionWhenStringIsNotEmptyAndIteratorWasNotValid_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABC");
    QStringUnicode::QConstCharIterator ITERATOR_LAST(SOURCE_STRING);
    ++ITERATOR_LAST;
    ++ITERATOR_LAST;
    QStringUnicode::QConstCharIterator INVALID_ITERATOR(SOURCE_STRING);
    INVALID_ITERATOR.MoveLast();
    SOURCE_STRING = "ABC";

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(INVALID_ITERATOR);
    iterator.MoveLast();
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_LAST);
}

/// <summary>
/// Checks that the iterator points to the last position even when it was pointing to the position before the first.
/// </summary>
QTEST_CASE ( MoveLast_IteratorPointsToLastPositionWhenStringIsNotEmptyAndIteratorWasPointingToPositionBeforeFirst_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABC");
    QStringUnicode::QConstCharIterator ORIGINAL_ITERATOR(SOURCE_STRING);
    ORIGINAL_ITERATOR--;

    QStringUnicode::QConstCharIterator ITERATOR_LAST(SOURCE_STRING);
    ++ITERATOR_LAST;
    ++ITERATOR_LAST;

	// [Execution]
    QStringUnicode::QConstCharIterator iterator(ORIGINAL_ITERATOR);
    iterator.MoveLast();
    
    // [Verification]
    BOOST_CHECK(iterator == ITERATOR_LAST);
}

/// <summary>
/// Checks that it returns True when iterator points to a common position.
/// </summary>
QTEST_CASE ( IsValid_ReturnsTrueWhenIteratorPointsToCommonPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABC");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    const bool EXPECTED_RESULT = true;

	// [Execution]
    bool bResult = ITERATOR.IsValid();
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns False when the iterator points to an out of bounds position.
/// </summary>
QTEST_CASE ( IsValid_ReturnsFalseWhenIteratorPointsToOutOfBoundPosition_Test )
{
    // [Preparation]
    QStringUnicode SOURCE_STRING("ABCEFGHIJKMN");
    QStringUnicode::QConstCharIterator ITERATOR(SOURCE_STRING);
    ITERATOR.MoveLast();
    SOURCE_STRING = "ABC";
    const bool EXPECTED_RESULT = false;

	// [Execution]
    bool bResult = ITERATOR.IsValid();
    
    // [Verification]
    BOOST_CHECK_EQUAL(bResult, EXPECTED_RESULT);
}

// End - Test Suite: QConstCharIterator
QTEST_SUITE_END()
