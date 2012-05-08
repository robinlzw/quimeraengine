// [TERMS&CONDITIONS]

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
using namespace boost::unit_test;

#include "../../testsystem/TestingExternalDefinitions.h"

#include "QTransformationMatrix3x3.h"
#include "QVector2.h"

using Kinesis::QuimeraEngine::Tools::Math::QTransformationMatrix3x3;

QTEST_SUITE_BEGIN( QTransformationMatrix3x3_TestSuite )

/// <summary>
/// Checks if default values have changed.
/// </summary>
QTEST_CASE ( Constructor1_DefaultValuesHaventChanged_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QMatrix3x3;

    const QTransformationMatrix3x3 EXPECTED_VALUE(QMatrix3x3::GetIdentity());

    // Execution
    QTransformationMatrix3x3 matrixUT;

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that every matrix element is copied to the right destination matrix element.
/// </summary>
QTEST_CASE ( Constructor2_EveryElementCopiedToCorrespondingElement_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1, SQFloat::_2, SQFloat::_3,
                                        SQFloat::_4, SQFloat::_5, SQFloat::_6,
                                        SQFloat::_7, SQFloat::_8, SQFloat::_9);

    // Execution
    QTransformationMatrix3x3 matrixUT(EXPECTED_VALUE);

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that the matrix is correctly created when using a combination of common translation, rotation and scaling matrices.
/// </summary>
QTEST_CASE ( Constructor3_MatrixIsCorrectlyCreatedFromCommonTranslationRotationAndScale_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QBaseVector2 TRANSLATION(SQFloat::_2, SQFloat::_3);
    const QBaseVector2 SCALE(SQFloat::_4, SQFloat::_5);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION = SQAngle::_45;
    #else
        const float_q ROTATION = SQAngle::_QuarterPi;
    #endif

    const QBaseMatrix3x3 EXPECTED_VALUE((float_q)2.8284271f,  (float_q)2.8284271f, SQFloat::_0,
                                        (float_q)-3.5355339f, (float_q)3.5355339f, SQFloat::_0,
                                        SQFloat::_2,          SQFloat::_3,         SQFloat::_1);

    // Execution
    QTransformationMatrix3x3 matrixUT(TRANSLATION, ROTATION, SCALE);

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that the matrix is correctly created when using a combination of common translation, rotation and scaling matrices.
/// </summary>
QTEST_CASE ( Constructor3_IdentityMatrixIsObtainedWhenUsingNeutralTransformations_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QBaseVector2 TRANSLATION(SQFloat::_0, SQFloat::_0);
    const QBaseVector2 SCALE(SQFloat::_1, SQFloat::_1);
    const float_q ROTATION = SQFloat::_0;

    const QTransformationMatrix3x3 EXPECTED_VALUE = QTransformationMatrix3x3::GetIdentity();

    // Execution
    QTransformationMatrix3x3 matrixUT(TRANSLATION, ROTATION, SCALE);

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that the rotation contained in the matrix follows left-handed rules (counter-clockwise).
/// </summary>
QTEST_CASE ( Constructor3_RotationFollowsLeftHandedRules_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;
    using Kinesis::QuimeraEngine::Tools::Math::QVector2;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QBaseVector2 TRANSLATION(SQFloat::_0, SQFloat::_0);
    const QBaseVector2 SCALE(SQFloat::_1, SQFloat::_1);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION = SQAngle::_45;
    #else
        const float_q ROTATION = SQAngle::_QuarterPi;
    #endif

    const QVector2 POINT = QVector2(SQFloat::_1, SQFloat::_0);

    // Execution
    QTransformationMatrix3x3 matrix(TRANSLATION, ROTATION, SCALE);
    QVector2 rotatedPointUT = POINT.Transform(matrix);

    // Verification
    BOOST_CHECK(SQFloat::IsPositive(rotatedPointUT.x));
    BOOST_CHECK(SQFloat::IsPositive(rotatedPointUT.y));
}

/// <summary>
/// Checks that the rotation data is correctly generated (neutral translation and scale used).
/// </summary>
QTEST_CASE ( Constructor3_RotationDataIsCorrectlyGenerated_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QBaseVector2 TRANSLATION(SQFloat::_0, SQFloat::_0);
    const QBaseVector2 SCALE(SQFloat::_1, SQFloat::_1);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q ROTATION = SQAngle::_60;
    #else
        const float_q ROTATION = SQAngle::_ThirdPi;
    #endif

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_0_5,        (float_q)0.8660254f, SQFloat::_0,
                                        (float_q)-0.8660254f, SQFloat::_0_5,       SQFloat::_0,
                                        SQFloat::_0,          SQFloat::_0,         SQFloat::_1);

    // Execution
    QTransformationMatrix3x3 matrixUT(TRANSLATION, ROTATION, SCALE);

    // Verification
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]) );
    BOOST_CHECK( SQFloat::AreEquals(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]) );
}

/// <summary>
/// Checks that the scaling data is correctly generated (neutral rotation and translation used).
/// </summary>
QTEST_CASE ( Constructor3_ScalingDataIsCorrectlyGenerated_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QBaseVector2 TRANSLATION(SQFloat::_0, SQFloat::_0);
    const QBaseVector2 SCALE(SQFloat::_2, SQFloat::_3);
    const float_q ROTATION = SQFloat::_0;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_2, SQFloat::_0, SQFloat::_0,
                                        SQFloat::_0, SQFloat::_3, SQFloat::_0,
                                        SQFloat::_0, SQFloat::_0, SQFloat::_1);

    // Execution
    QTransformationMatrix3x3 matrixUT(TRANSLATION, ROTATION, SCALE);

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that the translation data is correctly generated (neutral rotation and scale used).
/// </summary>
QTEST_CASE ( Constructor3_TranslationDataIsCorrectlyGenerated_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QBaseVector2 TRANSLATION(SQFloat::_2, SQFloat::_3);
    const QBaseVector2 SCALE(SQFloat::_1, SQFloat::_1);
    const float_q ROTATION = SQFloat::_0;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1, SQFloat::_0, SQFloat::_0,
                                        SQFloat::_0, SQFloat::_1, SQFloat::_0,
                                        SQFloat::_2, SQFloat::_3, SQFloat::_1);

    // Execution
    QTransformationMatrix3x3 matrixUT(TRANSLATION, ROTATION, SCALE);

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that all the elements of the returned matrix equal zero but those that are in the diagonal, which equal one.
/// </summary>
QTEST_CASE ( GetIdentity_AllElementsEqualZeroAndDiagonalEqualsOne_Test )
{
    // Preparation
    const float_q EXPECTED_VALUE_FOR_ALL = SQFloat::_0;
    const float_q EXPECTED_VALUE_FOR_DIAGONAL = SQFloat::_1;

	// Execution
    QTransformationMatrix3x3 matrixUT = QTransformationMatrix3x3::GetIdentity();

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE_FOR_DIAGONAL);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE_FOR_DIAGONAL);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE_FOR_ALL);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE_FOR_DIAGONAL);
}

/// <summary>
/// Checks that every element of the source matrix is copied to the right element of the target matrix.
/// </summary>
QTEST_CASE ( OperatorAssignation_EveryElementIsAssignedToRightTargetElement_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1,  SQFloat::_2,    SQFloat::_3,
                                        SQFloat::_4,  SQFloat::_5,    SQFloat::_6,
                                        SQFloat::_7,  SQFloat::_8,    SQFloat::_9);

    // Execution
    QTransformationMatrix3x3 matrixUT;
    matrixUT = EXPECTED_VALUE;

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that obtained transformations are neutral when the transformation matrix is the Identity.
/// </summary>
QTEST_CASE ( Decompose_ObtainedTransformationsAreNeutralWhenMatrixIsIdentity_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 IDENTITY = QTransformationMatrix3x3::GetIdentity();
    const QBaseVector2 EXPECTED_TRANSLATION(SQFloat::_0, SQFloat::_0);
    const QBaseVector2 EXPECTED_SCALE(SQFloat::_1, SQFloat::_1);
    const float_q EXPECTED_ROTATION = SQFloat::_0;

    // Execution
    QBaseVector2 vTranslationUT(SQFloat::_1, SQFloat::_1);
    QBaseVector2 vScaleUT;
    float_q fRotationUT = SQFloat::_10;
    IDENTITY.Decompose(vTranslationUT, fRotationUT, vScaleUT);

    // Verification
    BOOST_CHECK_EQUAL(vTranslationUT.x, EXPECTED_TRANSLATION.x);
    BOOST_CHECK_EQUAL(vTranslationUT.y, EXPECTED_TRANSLATION.y);
    BOOST_CHECK_EQUAL(vScaleUT.x, EXPECTED_SCALE.x);
    BOOST_CHECK_EQUAL(vScaleUT.y, EXPECTED_SCALE.y);
    BOOST_CHECK_EQUAL(fRotationUT, EXPECTED_ROTATION);
}

/// <summary>
/// Checks that transformations obtained from a common transformation matrix are correct.
/// </summary>
QTEST_CASE ( Decompose_CorrectTransformationsAreObtainedFromCommonMatrix_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3((float_q)2.8284271f,  (float_q)2.8284271f, SQFloat::_0,
                                                                         (float_q)-3.5355339f, (float_q)3.5355339f, SQFloat::_0,
                                                                         SQFloat::_2,          SQFloat::_3,         SQFloat::_1) );
    const QBaseVector2 EXPECTED_TRANSLATION(SQFloat::_2, SQFloat::_3);
    const QBaseVector2 EXPECTED_SCALE(SQFloat::_4, SQFloat::_5);

    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ROTATION = SQAngle::_45;
    #else
        const float_q EXPECTED_ROTATION = SQAngle::_QuarterPi;
    #endif

    // Execution
    QBaseVector2 vTranslationUT(SQFloat::_1, SQFloat::_1);
    QBaseVector2 vScaleUT;
    float_q fRotationUT = SQFloat::_10;
    TRANSFORMATION_MATRIX.Decompose(vTranslationUT, fRotationUT, vScaleUT);

    // Verification
    BOOST_CHECK(vTranslationUT == EXPECTED_TRANSLATION);
    BOOST_CHECK(vScaleUT == EXPECTED_SCALE);
    BOOST_CHECK( SQFloat::AreEquals(fRotationUT, EXPECTED_ROTATION) );
}

/// <summary>
/// Checks that the rotation obtained from a transformation matrix that only contains rotation data is correct.
/// </summary>
QTEST_CASE ( Decompose_CorrectRotationIsObtainedFromMatrixThatOnlyContainsRotation_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_0_5,        (float_q)0.8660254f, SQFloat::_0,
                                                                         (float_q)-0.8660254f, SQFloat::_0_5,       SQFloat::_0,
                                                                         SQFloat::_0,          SQFloat::_0,         SQFloat::_1) );
    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ROTATION = SQAngle::_60;
    #else
        const float_q EXPECTED_ROTATION = SQAngle::_ThirdPi;
    #endif

    // Execution
    QBaseVector2 vTranslationAux;
    QBaseVector2 vScaleAux;
    float_q fRotationUT = SQFloat::_0;
    TRANSFORMATION_MATRIX.Decompose(vTranslationAux, fRotationUT, vScaleAux);

    // Verification
    BOOST_CHECK( SQFloat::AreEquals(fRotationUT, EXPECTED_ROTATION) );
}

/// <summary>
/// Checks that an assertion fails when the scale was null (x and y equalled zero).
/// </summary>
QTEST_CASE ( Decompose_AssertionFailsWhenMatrixScaleIsNull_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseVector2(SQFloat::_1, SQFloat::_2),
                                                          SQFloat::_3,
                                                          QBaseVector2(SQFloat::_0, SQFloat::_0) );

    float_q fRotation;
    QBaseVector2 vTranslation;
    QBaseVector2 vScale;

    // Execution
    bool bAssertionFailed = false;

    try
    {
        TRANSFORMATION_MATRIX.Decompose(vTranslation, fRotation, vScale);
    }
    catch(...) // TODO [Thund]: Only must catch the proper exception class, not implemented yet
    {
        bAssertionFailed = true;
    }

    // Verification
    BOOST_CHECK(bAssertionFailed);
}

/// <summary>
/// Checks that the translation obtained from a transformation matrix that only contains translation data is correct.
/// </summary>
QTEST_CASE ( Decompose_CorrectTranslationIsObtainedFromMatrixThatOnlyContainsTranslation_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_1, SQFloat::_0, SQFloat::_0,
                                                                         SQFloat::_0, SQFloat::_1, SQFloat::_0,
                                                                         SQFloat::_2, SQFloat::_3, SQFloat::_1) );
    const QBaseVector2 EXPECTED_TRANSLATION(SQFloat::_2, SQFloat::_3);

    // Execution
    QBaseVector2 vTranslationUT;
    QBaseVector2 vScaleAux;
    float_q fRotationAux;
    TRANSFORMATION_MATRIX.Decompose(vTranslationUT, fRotationAux, vScaleAux);

    // Verification
    BOOST_CHECK(vTranslationUT == EXPECTED_TRANSLATION);
}

/// <summary>
/// Checks that the scale obtained from a transformation matrix that only contains scaling data is correct.
/// </summary>
QTEST_CASE ( Decompose_CorrectScaleIsObtainedFromMatrixThatOnlyContainsScaling_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_4, SQFloat::_0, SQFloat::_0,
                                                                         SQFloat::_0, SQFloat::_5, SQFloat::_0,
                                                                         SQFloat::_0, SQFloat::_0, SQFloat::_1) );
    const QBaseVector2 EXPECTED_SCALE(SQFloat::_4, SQFloat::_5);

    // Execution
    QBaseVector2 vTranslationAux;
    QBaseVector2 vScaleUT;
    float_q fRotationAux;
    TRANSFORMATION_MATRIX.Decompose(vTranslationAux, fRotationAux, vScaleUT);

    // Verification
    BOOST_CHECK(vScaleUT == EXPECTED_SCALE);
}

/// <summary>
/// Checks that the translation obtained from a transformation matrix that only contains translation data is correct.
/// </summary>
QTEST_CASE ( GetTranslation_CorrectTranslationIsObtainedFromMatrixThatOnlyContainsTranslation_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_1, SQFloat::_0, SQFloat::_0,
                                                                         SQFloat::_0, SQFloat::_1, SQFloat::_0,
                                                                         SQFloat::_2, SQFloat::_3, SQFloat::_1) );
    const QBaseVector2 EXPECTED_TRANSLATION(SQFloat::_2, SQFloat::_3);

    // Execution
    QBaseVector2 vTranslationUT;
    TRANSFORMATION_MATRIX.GetTranslation(vTranslationUT);

    // Verification
    BOOST_CHECK(vTranslationUT == EXPECTED_TRANSLATION);
}

/// <summary>
/// Checks that translation obtained from a common transformation matrix is correct.
/// </summary>
QTEST_CASE ( GetTranslation_CorrectTranslationIsObtainedFromCommonMatrix_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3((float_q)2.8284271f,  (float_q)2.8284271f, SQFloat::_0,
                                                                         (float_q)-3.5355339f, (float_q)3.5355339f, SQFloat::_0,
                                                                         SQFloat::_2,          SQFloat::_3,         SQFloat::_1) );
    const QBaseVector2 EXPECTED_TRANSLATION(SQFloat::_2, SQFloat::_3);

    // Execution
    QBaseVector2 vTranslationUT;
    TRANSFORMATION_MATRIX.GetTranslation(vTranslationUT);

    // Verification
    BOOST_CHECK(vTranslationUT == EXPECTED_TRANSLATION);
}

/// <summary>
/// Checks that the rotation obtained from a transformation matrix that only contains rotation data is correct.
/// </summary>
QTEST_CASE ( GetRotation_CorrectRotationIsObtainedFromMatrixThatOnlyContainsRotation_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_0_5,        (float_q)0.8660254f, SQFloat::_0,
                                                                         (float_q)-0.8660254f, SQFloat::_0_5,       SQFloat::_0,
                                                                         SQFloat::_0,          SQFloat::_0,         SQFloat::_1) );
    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ROTATION = SQAngle::_60;
    #else
        const float_q EXPECTED_ROTATION = SQAngle::_ThirdPi;
    #endif

    // Execution
    float_q fRotationUT = SQFloat::_0;
    TRANSFORMATION_MATRIX.GetRotation(fRotationUT);

    // Verification
    BOOST_CHECK( SQFloat::AreEquals(fRotationUT, EXPECTED_ROTATION) );
}

/// <summary>
/// Checks that rotation obtained from a common transformation matrix is correct.
/// </summary>
QTEST_CASE ( GetRotation_CorrectRotationIsObtainedFromCommonMatrix_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::SQAngle;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3((float_q)2.8284271f,  (float_q)2.8284271f, SQFloat::_0,
                                                                         (float_q)-3.5355339f, (float_q)3.5355339f, SQFloat::_0,
                                                                         SQFloat::_2,          SQFloat::_3,         SQFloat::_1) );
    #if QE_CONFIG_ANGLENOTATION_DEFAULT == QE_CONFIG_ANGLENOTATION_DEGREES
        const float_q EXPECTED_ROTATION = SQAngle::_45;
    #else
        const float_q EXPECTED_ROTATION = SQAngle::_QuarterPi;
    #endif

    // Execution
    float_q fRotationUT = SQFloat::_0;
    TRANSFORMATION_MATRIX.GetRotation(fRotationUT);

    // Verification
    BOOST_CHECK( SQFloat::AreEquals(fRotationUT, EXPECTED_ROTATION) );
}

/// <summary>
/// Checks that an assertion fails when the scale was null (x and y equalled zero).
/// </summary>
QTEST_CASE ( GetRotation_AssertionFailsWhenMatrixScaleIsNull_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseVector2(SQFloat::_1, SQFloat::_2),
                                                          SQFloat::_3,
                                                          QBaseVector2(SQFloat::_0, SQFloat::_0) );

    float_q fRotation;

    // Execution
    bool bAssertionFailed = false;

    try
    {
        TRANSFORMATION_MATRIX.GetRotation(fRotation);
    }
    catch(...) // TODO [Thund]: Only must catch the proper exception class, not implemented yet
    {
        bAssertionFailed = true;
    }

    // Verification
    BOOST_CHECK(bAssertionFailed);
}

/// <summary>
/// Checks that the scale obtained from a transformation matrix that only contains scaling data is correct.
/// </summary>
QTEST_CASE ( GetScale_CorrectScaleIsObtainedFromMatrixThatOnlyContainsScaling_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_4, SQFloat::_0, SQFloat::_0,
                                                                         SQFloat::_0, SQFloat::_5, SQFloat::_0,
                                                                         SQFloat::_0, SQFloat::_0, SQFloat::_1) );
    const QBaseVector2 EXPECTED_SCALE(SQFloat::_4, SQFloat::_5);

    // Execution
    QBaseVector2 vScaleUT;
    TRANSFORMATION_MATRIX.GetScale(vScaleUT);

    // Verification
    BOOST_CHECK(vScaleUT == EXPECTED_SCALE);
}

/// <summary>
/// Checks that scale obtained from a common transformation matrix is correct.
/// </summary>
QTEST_CASE ( GetScale_CorrectScaleIsObtainedFromCommonMatrix_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QBaseVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3((float_q)2.8284271f,  (float_q)2.8284271f, SQFloat::_0,
                                                                         (float_q)-3.5355339f, (float_q)3.5355339f, SQFloat::_0,
                                                                         SQFloat::_2,          SQFloat::_3,         SQFloat::_1) );
    const QBaseVector2 EXPECTED_SCALE(SQFloat::_4, SQFloat::_5);

    // Execution
    QBaseVector2 vScaleUT;
    TRANSFORMATION_MATRIX.GetScale(vScaleUT);

    // Verification
    BOOST_CHECK(vScaleUT == EXPECTED_SCALE);
}

/// <summary>
/// Checks that resulting matrix matches the expected value, when using a common transformation matrix.
/// </summary>
QTEST_CASE ( SwitchHandConvention_CorrectResultIsObtainedWhenUsingCommonMatrix_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QBaseMatrix3x3 EXPECTED_VALUE(SQFloat::_1,  -SQFloat::_2, SQFloat::_3,
                                        -SQFloat::_4, SQFloat::_5,  SQFloat::_6,
                                        SQFloat::_7,  SQFloat::_8,  SQFloat::_9);

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_1, SQFloat::_2, SQFloat::_3,
                                                                         SQFloat::_4, SQFloat::_5, SQFloat::_6,
                                                                         SQFloat::_7, SQFloat::_8, SQFloat::_9) );

    // Execution
    QTransformationMatrix3x3 matrixUT = TRANSFORMATION_MATRIX.SwitchHandConvention();

    // Verification
    BOOST_CHECK_EQUAL(matrixUT.ij[0][0], EXPECTED_VALUE.ij[0][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][1], EXPECTED_VALUE.ij[0][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[0][2], EXPECTED_VALUE.ij[0][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][0], EXPECTED_VALUE.ij[1][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][1], EXPECTED_VALUE.ij[1][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[1][2], EXPECTED_VALUE.ij[1][2]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][0], EXPECTED_VALUE.ij[2][0]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][1], EXPECTED_VALUE.ij[2][1]);
    BOOST_CHECK_EQUAL(matrixUT.ij[2][2], EXPECTED_VALUE.ij[2][2]);
}

/// <summary>
/// Checks that the rotation is inverted, as hand-convention conversion requires.
/// </summary>
QTEST_CASE ( SwitchHandConvention_RotationIsInverted_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;
    using Kinesis::QuimeraEngine::Tools::Math::QVector2;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_0_5,        (float_q)0.8660254f, SQFloat::_0,
                                                                         (float_q)-0.8660254f, SQFloat::_0_5,       SQFloat::_0,
                                                                         SQFloat::_0,          SQFloat::_0,         SQFloat::_1) );
    const QVector2 ROTATED_POINT = QVector2(SQFloat::_1, SQFloat::_0).Transform(TRANSFORMATION_MATRIX);

    // Execution
    QTransformationMatrix3x3 matrix = TRANSFORMATION_MATRIX.SwitchHandConvention();
    QVector2 vRotatedPointAfterConversion = QVector2(SQFloat::_1, SQFloat::_0).Transform(matrix);

    // Verification
    BOOST_CHECK_EQUAL( vRotatedPointAfterConversion.x, ROTATED_POINT.x );
    BOOST_CHECK_EQUAL( vRotatedPointAfterConversion.y, -ROTATED_POINT.y );
}

/// <summary>
/// Checks that the rotation angle is the same even after inverting the rotation due to hand-convention conversion.
/// </summary>
QTEST_CASE ( SwitchHandConvention_RotationAngleRemainsTheSame_Test )
{
    // Preparation
    using Kinesis::QuimeraEngine::Tools::Math::QBaseMatrix3x3;

    const QTransformationMatrix3x3 TRANSFORMATION_MATRIX( QBaseMatrix3x3(SQFloat::_0_5,        (float_q)0.8660254f, SQFloat::_0,
                                                                         (float_q)-0.8660254f, SQFloat::_0_5,       SQFloat::_0,
                                                                         SQFloat::_0,          SQFloat::_0,         SQFloat::_1) );
    float_q ORIGINAL_ROTATION;
    TRANSFORMATION_MATRIX.GetRotation(ORIGINAL_ROTATION);

    // Execution
    QTransformationMatrix3x3 matrix = TRANSFORMATION_MATRIX.SwitchHandConvention();
    float_q fRotationUT;
    matrix.GetRotation(fRotationUT);

    // Verification
    BOOST_CHECK_EQUAL( fRotationUT, ORIGINAL_ROTATION );
}

// End - Test Suite: QTransformationMatrix3x3
QTEST_SUITE_END()