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

using Kinesis::QuimeraEngine::Common::DataTypes::QStringUnicode;
using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;



QTEST_SUITE_BEGIN( QCharIterator_TestSuite )

/// <summary>
/// Checks that the iterator points to the first position when using a common string.
/// </summary>
QTEST_CASE ( Constructor1_IteratorPointsToFirstPositionWhenUsingCommonString_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator EXPECTED_ITERATOR(strSourceString);
    EXPECTED_ITERATOR.MoveFirst();

	// [Execution]
    QStringUnicode::QCharIterator iterator(strSourceString);
    
    // [Verification]
    BOOST_CHECK(iterator == EXPECTED_ITERATOR);
}

/// <summary>
/// Checks that the iterator points to the end position when using an empty string.
/// </summary>
QTEST_CASE ( Constructor1_IteratorPointsToEndPositionWhenUsingEmptyString_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("");
    const bool IS_END = true;

	// [Execution]
    QStringUnicode::QCharIterator iterator(strSourceString);
    
    // [Verification]
    BOOST_CHECK_EQUAL(iterator.IsEnd(), IS_END);
}

/// <summary>
/// Checks that the iterator is correctly copied when using a common input interator.
/// </summary>
QTEST_CASE ( Constructor2_IteratorIsCorrectlyCopiedWhenUsingCommonIterator_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator EXPECTED_ITERATOR(strSourceString);
    ++EXPECTED_ITERATOR;

	// [Execution]
    QStringUnicode::QCharIterator iterator(EXPECTED_ITERATOR);
    
    // [Verification]
    BOOST_CHECK(iterator == EXPECTED_ITERATOR);
}

/// <summary>
/// Checks that the iterator is correctly copied when using an invalid input interator.
/// </summary>
QTEST_CASE ( Constructor2_IteratorIsCorrectlyCopiedWhenUsingInvalidIterator_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator EXPECTED_ITERATOR(strSourceString);
    EXPECTED_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");

	// [Execution]
    QStringUnicode::QCharIterator iterator(EXPECTED_ITERATOR);
    
    // [Verification]
    BOOST_CHECK(!iterator.IsValid());
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR++;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR_END++;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

	// [Execution]
    QStringUnicode::QCharIterator iterator(ITERATOR_END);
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveFirst();
    const QCharUnicode SECOND_CHARACTER('B'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorPreviousState = iterator++;
    
    // [Verification]
    BOOST_CHECK(iteratorPreviousState == ORIGINAL_ITERATOR);
    BOOST_CHECK(iterator.GetChar() == SECOND_CHARACTER);
}

/// <summary>
/// Checks that the iterator points to the first position and returns the previous state when it was pointing to the end position before the first one.
/// </summary>
QTEST_CASE ( OperatorPostIncrement_IteratorPointsToFirstPositionAndReturnsPreviousStateWhenItWasPointingToPositionBeforeFirst_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveFirst();
    ORIGINAL_ITERATOR--;
    const QCharUnicode FIRST_CHARACTER('A'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorPreviousState = iterator++;
    QCharUnicode currentChar = iterator.GetChar();
    
    // [Verification]
    BOOST_CHECK(iteratorPreviousState == ORIGINAL_ITERATOR);
    BOOST_CHECK(currentChar == FIRST_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPostDecrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR--;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveFirst();
    ITERATOR_END--;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ITERATOR_END--;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveFirst();
    ITERATOR_END--;

	// [Execution]
    QStringUnicode::QCharIterator iterator(ITERATOR_END);
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveLast();
    const QCharUnicode BEFORE_LAST_CHARACTER('M'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorPreviousState = iterator--;
    
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveLast();
    ++ORIGINAL_ITERATOR;
    const QCharUnicode LAST_CHARACTER('N'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorPreviousState = iterator--;
    
    // [Verification]
    BOOST_CHECK(iteratorPreviousState == ORIGINAL_ITERATOR);
    BOOST_CHECK(iterator.GetChar() == LAST_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPreIncrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ++INVALID_ITERATOR;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        ++ITERATOR_END;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveLast();
    ++ITERATOR_END;

	// [Execution]
    QStringUnicode::QCharIterator iterator(ITERATOR_END);
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveFirst();
    const QCharUnicode SECOND_CHARACTER('B'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorCurrentState = ++iterator;
    
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveFirst();
    ORIGINAL_ITERATOR--;
    const QCharUnicode FIRST_CHARACTER('A'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorCurrentState = ++iterator;
    
    // [Verification]
    BOOST_CHECK(iteratorCurrentState.GetChar() == FIRST_CHARACTER);
    BOOST_CHECK(iterator.GetChar() == FIRST_CHARACTER);
}


#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorPreDecrement_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        --INVALID_ITERATOR;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveFirst();
    --ITERATOR_END;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        --ITERATOR_END;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ITERATOR_END(strSourceString);
    ITERATOR_END.MoveFirst();
    --ITERATOR_END;

	// [Execution]
    QStringUnicode::QCharIterator iterator(ITERATOR_END);
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveLast();
    const QCharUnicode BEFORE_LAST_CHARACTER('M'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorCurrentState = --iterator;
    
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator ORIGINAL_ITERATOR(strSourceString);
    ORIGINAL_ITERATOR.MoveLast();
    ++ORIGINAL_ITERATOR;
    const QCharUnicode LAST_CHARACTER('N'); 

	// [Execution]
    QStringUnicode::QCharIterator iterator(ORIGINAL_ITERATOR);
    QStringUnicode::QCharIterator iteratorCurrentState = --iterator;
    
    // [Verification]
    BOOST_CHECK(iteratorCurrentState.GetChar() == LAST_CHARACTER);
    BOOST_CHECK(iterator.GetChar() == LAST_CHARACTER);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input iterator is not valid.
/// </summary>
QTEST_CASE ( OperatorAssignation_AssertionFailsWhenInputIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QCharIterator iterator(strSourceString);
        iterator = INVALID_ITERATOR;
    }
    catch(...)
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
    QStringUnicode::QCharIterator ITERATOR_A(STRING_A);

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        QStringUnicode::QCharIterator iterator(STRING_B);
        iterator = ITERATOR_A;
    }
    catch(...)
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");

	// [Execution]
    QStringUnicode::QCharIterator iterator(strSourceString);
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
    QStringUnicode::QCharIterator ITERATOR_A(STRING_A);
    QStringUnicode::QCharIterator ITERATOR_B(STRING_B);

	// [Execution]
    QStringUnicode::QCharIterator iterator = ITERATOR_B;
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
    QStringUnicode::QCharIterator ITERATOR_A(STRING_SOURCE);
    QStringUnicode::QCharIterator ITERATOR_B(STRING_SOURCE);

	// [Execution]
    QStringUnicode::QCharIterator iterator = ITERATOR_B;
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
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    QStringUnicode::QCharIterator VALID_ITERATOR(strSourceString);

	// [Execution]
    QStringUnicode::QCharIterator iterator = INVALID_ITERATOR;
    iterator = VALID_ITERATOR;
    
    // [Verification]
    BOOST_CHECK(iterator.IsValid());
    BOOST_CHECK(iterator == VALID_ITERATOR);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the iterator is not valid.
/// </summary>
QTEST_CASE ( SetChar_AssertionFailsWhenIteratorIsNotValid_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator INVALID_ITERATOR(strSourceString);
    INVALID_ITERATOR.MoveLast();
    strSourceString = QStringUnicode("ABC");
    const bool ASSERTION_FAILED = true;
    const QCharUnicode CHARACTER = 'A';

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        INVALID_ITERATOR.SetChar(CHARACTER);
    }
    catch(...)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

/// <summary>
/// Checks that an assertion fails when the iterator points to end position.
/// </summary>
QTEST_CASE ( SetChar_AssertionFailsWhenIteratorPointsToEndPosition_Test )
{
    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    const QCharUnicode CHARACTER = 'A';
    QStringUnicode::QCharIterator iterator(strSourceString);
    iterator.MoveLast();
    ++iterator;

    const bool ASSERTION_FAILED = true;

	// [Execution]
    bool bAssertionFailed = false;

    try
    {
        iterator.SetChar(CHARACTER);
    }
    catch(...)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK_EQUAL(bAssertionFailed, ASSERTION_FAILED);
}

#elif QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_DISABLED

/// <summary>
/// Checks that nothing happens when iterator points to an end position.
/// </summary>
QTEST_CASE ( SetChar_NothingHappensWhenIteratorPointsToEndPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode ORIGINAL_STRING("ABCDEFGHIJKMN");
    QStringUnicode strSourceString(ORIGINAL_STRING);
    QStringUnicode::QCharIterator iterator(strSourceString);
    iterator.MoveLast();
    ++iterator;
    const QCharUnicode CHARACTER = 0xFFFF;

	// [Execution]
    iterator.SetChar(CHARACTER);
    
    // [Verification]
    BOOST_CHECK(strSourceString == ORIGINAL_STRING);
}

#endif

/// <summary>
/// Checks that it returns the expected character when the iterator points to a common position.
/// </summary>
QTEST_CASE ( SetChar_SetsCharacterCorrectlyWhenIteratorPointsCommonPosition_Test )
{
    using Kinesis::QuimeraEngine::Common::DataTypes::QCharUnicode;

    // [Preparation]
    QStringUnicode strSourceString("ABCDEFGHIJKMN");
    QStringUnicode::QCharIterator iterator(strSourceString);
    iterator.MoveFirst();
    ++iterator;
    ++iterator;
    const QCharUnicode EXPECTED_CHARACTER = 'X';

	// [Execution]
    iterator.SetChar(EXPECTED_CHARACTER);
    
    // [Verification]
    QCharUnicode character = iterator.GetChar();
    BOOST_CHECK(character == EXPECTED_CHARACTER);
}


// End - Test Suite: QCharIterator
QTEST_SUITE_END()
