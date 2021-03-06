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

#include "QList.h"
#include "QListWhiteBox.h"
#include "ArrayElementMock.h"

using Kinesis::QuimeraEngine::Tools::Containers::QList;
using Kinesis::QuimeraEngine::Common::DataTypes::u64_q;
using Kinesis::QuimeraEngine::Tools::Containers::Test::QListWhiteBox;
using Kinesis::QuimeraEngine::Tools::Containers::Test::ArrayElementMock;


QTEST_SUITE_BEGIN( QList_TestSuite )

/// <summary>
/// Checks if constructor without parameteres initializes members correctly.
/// </summary>
QTEST_CASE ( Constructor1_ChecksIfConstructorWithoutParametersInitializesMembersCorrectly_Test )
{
    // [Preparation]
    static const pointer_uint_q EXPECTED_END_POSITION_FORWARD = -1;
    static const pointer_uint_q EXPECTED_END_POSITION_BACKWARD = -2;
    static const pointer_uint_q INITIAL_CAPACITY = QListWhiteBox<u64_q>::GetDefaultInitialCapacity();

    // [Execution]
    QListWhiteBox<u64_q> list = QListWhiteBox<u64_q>();

    // [Verification]
    pointer_uint_q uFirst = list.GetFirst();
    pointer_uint_q uLast  = list.GetLast();
    const QPoolAllocator* pElementsAllocator = list.GetAllocator();

    // [TODO] rdelasheras: Replace by GetCapacity when exists.
    pointer_uint_q uInitialCapacity = pElementsAllocator->GetPoolSize() / sizeof(u64_q);


    BOOST_CHECK_EQUAL( uFirst, EXPECTED_END_POSITION_BACKWARD );
    BOOST_CHECK_EQUAL( uLast, EXPECTED_END_POSITION_FORWARD );
    BOOST_CHECK_EQUAL( uInitialCapacity, INITIAL_CAPACITY );
}

/// <summary>
/// Checks if constructor with number of elements to reserve by parameter initializes members correctly.
/// </summary>
QTEST_CASE ( Constructor2_ChecksIfConstructorInitializesMembersCorrectly_Test )
{
    // [Preparation]
    static const pointer_uint_q EXPECTED_END_POSITION_FORWARD = -1;
    static const pointer_uint_q EXPECTED_END_POSITION_BACKWARD = -2;
    static const pointer_uint_q INITIAL_CAPACITY = 5;

    // [Execution]
    QListWhiteBox<u64_q> list = QListWhiteBox<u64_q>( INITIAL_CAPACITY );

    // [Verification]
    pointer_uint_q uFirst = list.GetFirst();
    pointer_uint_q uLast  = list.GetLast();
    const QPoolAllocator* pElementsAllocator = list.GetAllocator();

    // [TODO] rdelasheras: Replace by GetCapacity when exists.
    pointer_uint_q uInitialCapacity = pElementsAllocator->GetPoolSize() / sizeof(u64_q);

    BOOST_CHECK_EQUAL( uFirst, EXPECTED_END_POSITION_BACKWARD );
    BOOST_CHECK_EQUAL( uLast, EXPECTED_END_POSITION_FORWARD );
    BOOST_CHECK_EQUAL( uInitialCapacity, INITIAL_CAPACITY );

}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Assertion failed when passing zero elements as initial capacity.
/// </summary>
QTEST_CASE ( Constructor2_AssertionFailedWhenPassingZeroElementsAsInitialCapacity_Test )
{
    // [Preparation]
    const pointer_uint_q INITIAL_CAPACITY_ZERO = 0;
    const bool ASSERTION_FAILED = true;

    // [Execution]
    bool bAssertionFailed = false;

    try
    {
        QList<u64_q> list = QList<u64_q>(INITIAL_CAPACITY_ZERO);
    }
    catch(...)
    {
        bAssertionFailed = true;
    }

    // [Verification]
    BOOST_CHECK_EQUAL( bAssertionFailed, ASSERTION_FAILED );
}

#endif // QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS


// [TODO] rdelasheras. Uncomment when copy constructor exists.
/*
/// <summary>
/// Checks if copy constructor initializes correctly when passing a list with zero elements.
/// </summary>
QTEST_CASE ( Constructor3_ChecksIfConstructorInitializesCorrectlyWhenPassingAListWithZeroElements_Test )
{
    // [Preparation]
    static const pointer_uint_q EXPECTED_END_POSITION_FORWARD = -1;
    static const pointer_uint_q EXPECTED_END_POSITION_BACKWARD = -2;

    // [Execution]
    QListWhiteBox<u64_q> listOrigin = QListWhiteBox<u64_q>();
    QListWhiteBox<u64_q> listDestination = QListWhiteBox<u64_q>(listOrigin);

    // [Verification]
    pointer_uint_q uFirst = listDestination.GetFirst();
    pointer_uint_q uLast  = listDestination.GetLast();
    const QPoolAllocator* pElementsAllocator = listDestination.GetAllocator();
    const QPoolAllocator* pLinkAllocator = listDestination.GetLinkAllocator();

    BOOST_CHECK_EQUAL( uFirst, EXPECTED_END_POSITION_BACKWARD );
    BOOST_CHECK_EQUAL( uLast, EXPECTED_END_POSITION_FORWARD );
}
*/
// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if copy constructor initializes correctly when passing a full list.
/// </summary>
QTEST_CASE ( Constructor3_ChecksIfConstructorInitializesCorrectlyWhenPassingAFullList_Test )
{
    // [Preparation]
    const pointer_uint_q NUMBER_OF_ELEMENTS = 5;
    const bool SAME_VALUES = true;

    QList<u64_q> listOrigin = QList<u64_q>(NUMBER_OF_ELEMENTS);

    for(u64_q uIndex = 0; uIndex < NUMBER_OF_ELEMENTS; uIndex++)
    {
        listOrigin.Add(uIndex);
    }

    // [Execution]
    QList<u64_q> listDestination = QList<u64_q>(listOrigin);

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < NUMBER_OF_ELEMENTS; uIndex++)
    {
        bSameValues = bSameValues && (listOrigin.GetValue(uIndex) == listDestination.GetValue(uIndex));
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );

}
  */

// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if copy constructor initializes correctly when passing an empty list.
/// </summary>
QTEST_CASE ( Constructor3_ChecksIfConstructorInitializesCorrectlyWhenPassingAnEmptyList_Test )
{
    // [Preparation]
    const pointer_uint_q INITIAL_CAPACITY = 5;
    
    QListWhiteBox<u64_q> listOrigin = QListWhiteBox<u64_q>(INITIAL_CAPACITY);

    // [Execution]
    QList<u64_q> listDestination = QList<u64_q>(listOrigin);

    // [Verification]
    pointer_uint_q uFirst = listDestination.GetFirst();
    pointer_uint_q uLast  = listDestination.GetLast();

    BOOST_CHECK_EQUAL( uFirst, EXPECTED_END_POSITION_BACKWARD );
    BOOST_CHECK_EQUAL( uLast, EXPECTED_END_POSITION_FORWARD );
}
  */


// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when the size of the origin list is equal to the destination list.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenOriginListSizeIsEqualToDestinationListSize_Test )
{
    // [Preparation]
    const pointer_uint_q LIST_COUNT = 5;
    QList<u64_q> listOrigin = QList<u64_q>(LIST_COUNT);
    QList<u64_q> listDestination = QList<u64_q>(LIST_COUNT);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT; uIndex++)
    {
        listOrigin.Add(uIndex)
        listDestination.Add(LIST_COUNT-uIndex);
    }

    const bool SAME_VALUES = true;

    // [Execution]
    listDestination = listOrigin;

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT; uIndex++)
    {
        bSameValues = bSameValues && (listDestination.GetValue(uIndex) == listOrigin.GetValue(uIndex));
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );
}
*/
// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when assigning a list to itself.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenAssigningAListToItself_Test )
{
    // [Preparation]
    const pointer_uint_q LIST_COUNT = 5;
    QList<u64_q> list = QList<u64_q>(LIST_COUNT);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT; uIndex++)
    {
        list.Add(uIndex)
    }

    const bool SAME_VALUES = true;

    // [Execution]
    list = list;

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT; uIndex++)
    {
        bSameValues = bSameValues && (list.GetValue(uIndex) == uIndex);
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );
}
*/


// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when the size of the origin list is minor than the destination list.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenOriginListSizeIsMinorThanDestinationListSize_Test )
{
    // [Preparation]
    const pointer_uint_q INITIAL_CAPACITY = 5;
    const pointer_uint_q LIST_COUNT_ORIGIN = 3;
    const pointer_uint_q LIST_COUNT_DESTINATION = 5;
    QList<u64_q> listOrigin = QList<u64_q>(INITIAL_CAPACITY);
    QList<u64_q> listDestination = QList<u64_q>(INITIAL_CAPACITY);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        listOrigin.Add(uIndex+10);
    }

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_DESTINATION; uIndex++)
    {
        listDestination.Add(uIndex);
    }

    const bool SAME_VALUES = true;

    // [Execution]
    listDestination = listOrigin;

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        bSameValues = bSameValues && (listDestination.GetValue(uIndex) == listOrigin.GetValue(uIndex));
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );
}

*/

// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when the size of the origin list is zero and the destination list has elements.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenOriginListIsZeroAndTheDestinationListHasElements_Test )
{
    // [Preparation]
    const pointer_uint_q INITIAL_CAPACITY = 5;
    const pointer_uint_q LIST_COUNT_DESTINATION = 5;
    QList<u64_q> listOrigin = QList<u64_q>(INITIAL_CAPACITY);
    QList<u64_q> listDestination = QList<u64_q>(INITIAL_CAPACITY);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_DESTINATION; uIndex++)
    {
        listDestination.Add(uIndex);
    }

    const bool SAME_VALUES = true;

    // [Execution]
    listDestination = listOrigin;

    // [Verification]
    pointer_uint_q uFirst = listDestination.GetFirst();
    pointer_uint_q uLast  = listDestination.GetLast();

    BOOST_CHECK_EQUAL( uFirst, EXPECTED_END_POSITION_BACKWARD );
    BOOST_CHECK_EQUAL( uLast, EXPECTED_END_POSITION_FORWARD );
}

*/


// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when the size of the origin list is greater than the destination list and
/// destination list capacity is not minor than origin list capacity.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenOriginListSizeIsGreaterThanDestinationListSizeAndDestinationListCapacityIsNotMinorThanOriginListCapacity_Test )
{
    // [Preparation]
    const pointer_uint_q LISTS_CAPACITY = 5;
    const pointer_uint_q LIST_COUNT_ORIGIN = 5;
    const pointer_uint_q LIST_COUNT_DESTINATION = 3;
    QList<u64_q> listOrigin = QList<u64_q>(LISTS_CAPACITY);
    QList<u64_q> listDestination = QList<u64_q>(LISTS_CAPACITY);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        listOrigin.Add(uIndex+10);
    }

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_DESTINATION; uIndex++)
    {
        listDestination.Add(uIndex);
    }

    const bool SAME_VALUES = true;

    // [Execution]
    listDestination = listOrigin;

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        bSameValues = bSameValues && (listDestination.GetValue(uIndex) == listOrigin.GetValue(uIndex));
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );
}
*/

// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when the size of the origin list is greater than the destination list and
/// the capacity of the origin list is greater than the capacity of the destination list.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenOriginListSizeIsGreaterThanDestinationListSizeAndOriginListCapacityIsGreaterThanDestinationListCapacity_Test )
{
    // [Preparation]
    const pointer_uint_q LIST_CAPACITY_ORIGIN = 5;
    const pointer_uint_q LIST_CAPACITY_DESTINATION = 3;
    const pointer_uint_q LIST_COUNT_ORIGIN = 5;
    const pointer_uint_q LIST_COUNT_DESTINATION = 3;
    QList<u64_q> listOrigin = QList<u64_q>(LIST_CAPACITY_ORIGIN);
    QList<u64_q> listDestination = QList<u64_q>(LIST_CAPACITY_DESTINATION);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        listOrigin.Add(uIndex+10);
    }

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_DESTINATION; uIndex++)
    {
        listDestination.Add(uIndex);
    }

    const bool SAME_VALUES = true;

    // [Execution]
    listDestination = listOrigin;

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        bSameValues = bSameValues && (listDestination.GetValue(uIndex) == listOrigin.GetValue(uIndex));
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );
}
*/

// [TODO] rdelasheras. Uncomment the test when methods to Add and Get elements from a list exist.
/*
/// <summary>
/// Checks if it assigns correctly when the size of the origin list is greater than the destination list and
/// the capacity of the two lists are equal.
/// </summary>
QTEST_CASE ( OperatorAssignment_CorrectlyAssignedWhenOriginListSizeIsGreaterThanDestinationListSizeAndOriginListCapacityIsGreaterThanDestinationListCapacity_Test )
{
    // [Preparation]
    const pointer_uint_q LIST_CAPACITY = 5;
    const pointer_uint_q LIST_COUNT_ORIGIN = 5;
    const pointer_uint_q LIST_COUNT_DESTINATION = 3;
    QList<u64_q> listOrigin = QList<u64_q>(LIST_CAPACITY);
    QList<u64_q> listDestination = QList<u64_q>(LIST_CAPACITY);

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        listOrigin.Add(uIndex+10);
    }

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_DESTINATION; uIndex++)
    {
        listDestination.Add(uIndex);
    }

    const bool SAME_VALUES = true;

    // [Execution]
    listDestination = listOrigin;

    // [Verification]
    bool bSameValues = true;

    for(pointer_uint_q uIndex = 0; uIndex < LIST_COUNT_ORIGIN; uIndex++)
    {
        bSameValues = bSameValues && (listDestination.GetValue(uIndex) == listOrigin.GetValue(uIndex));
    }

    BOOST_CHECK_EQUAL( bSameValues, SAME_VALUES );
}
*/

// End - Test Suite: QList

QTEST_SUITE_END()
