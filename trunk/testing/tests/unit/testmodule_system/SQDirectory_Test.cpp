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

// Note: In these unit tests we are no fulfilling the principle of reproducibility. We are running out of time at the moment.
// Note2: These unit tests make use of some artifacts located at /testing/bin/artifacts/artifacts/SQDirectory/ (copied to the cwd before execution).

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
using namespace boost::unit_test;

#include "../../testsystem/TestingExternalDefinitions.h"

#include "SQDirectory.h"

// These includes are needed by WaitForCreationOrDeletion function
#include "QStopwatch.h"
#include "QArrayResult.h"
#include "EQTextEncoding.h"
#include "SQFile.h"
#include "SQThisThread.h"
#include "QArrayDynamic.h"
#include "QAssertException.h"
#include <fstream>

using Kinesis::QuimeraEngine::Common::Exceptions::QAssertException;
using Kinesis::QuimeraEngine::System::IO::FileSystem::SQDirectory;
using Kinesis::QuimeraEngine::System::IO::FileSystem::QPath;
using Kinesis::QuimeraEngine::System::IO::FileSystem::EQFileSystemError;
using Kinesis::QuimeraEngine::Common::DataTypes::QArrayBasic;
using Kinesis::QuimeraEngine::Common::DataTypes::QArrayResult;

// The base path to all the artifacts used by these tests
static const string_q PATH_TO_ARTIFACTS("./artifacts/SQDirectory/");

// Function that waits for a predefined amount of time for a file system operation to complete
bool WaitForCreationOrDeletion_SQDirectoryTestHelper(const QPath &directoryOrFile, const bool bTDeletionFCreation)
{
    using Kinesis::QuimeraEngine::System::Timing::QStopwatch;
    using Kinesis::QuimeraEngine::Common::DataTypes::u64_q;
    using Kinesis::QuimeraEngine::Common::DataTypes::i8_q;
    using Kinesis::QuimeraEngine::Common::DataTypes::EQTextEncoding;
    using Kinesis::QuimeraEngine::System::Threading::SQThisThread;

    static const u64_q MAXIMUM_WAIT_TIME = 600ULL; // milliseconds

#if defined(QE_OS_LINUX) || defined(QE_OS_MAC)
    static const EQTextEncoding PATH_ENCODING = EQTextEncoding::E_UTF8;
#elif defined(QE_OS_WINDOWS)
    static const EQTextEncoding PATH_ENCODING = string_q::GetLocalEncodingUTF16();
#endif

    QArrayResult<i8_q> arBytesDirectory = directoryOrFile.ToString().ToBytes(PATH_ENCODING);
    boost::filesystem::path::value_type* szPath = (boost::filesystem::path::value_type*)arBytesDirectory.Get();
    boost::filesystem::path directoryOrFilePath(szPath);

    QStopwatch elapsedTime;
    elapsedTime.Set();
    bool bTooMuchTime = false;

    while(boost::filesystem::exists(directoryOrFilePath) == bTDeletionFCreation && !bTooMuchTime)
    {
        SQThisThread::Yield();
        bTooMuchTime = elapsedTime.GetElapsedTimeAsTimeSpan().GetMilliseconds() < MAXIMUM_WAIT_TIME;
    }

    QE_ASSERT_ERROR(!bTooMuchTime, "The timeout expired, the file or directory could not be created or deleted fast enough.");

    return !bTooMuchTime;
}

// This function checks the existence of the content in a concrete directory
bool CheckDirectoryContent(const QArrayBasic<const QPath> arContents, const string_q &strExpectedFileContent)
{
    using Kinesis::QuimeraEngine::Common::DataTypes::EQTextEncoding;
    using Kinesis::QuimeraEngine::Common::DataTypes::pointer_uint_q;
    using Kinesis::QuimeraEngine::Common::DataTypes::i8_q;

    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    bool bContentMatchesExpectations = true;
    pointer_uint_q uContentIndex = 0;

    while(bContentMatchesExpectations && uContentIndex < arContents.GetCount())
    {
        if(arContents[uContentIndex].IsDirectory())
        {
            // Checks that the directory exists
            bContentMatchesExpectations = bContentMatchesExpectations && SQDirectory::Exists(arContents[uContentIndex], eErrorCode);
        }
        else
        {
            // Checks that the file exists
            bContentMatchesExpectations = bContentMatchesExpectations && 
                                          Kinesis::QuimeraEngine::System::IO::FileSystem::SQFile::Exists(arContents[uContentIndex], eErrorCode);

            if(bContentMatchesExpectations)
            {
                // [TODO] Thund: Replace this stuff with QTextStreamReader<QFileStream> when they both exist
                // Opens the file
                QArrayResult<i8_q> arFilePath = arContents[uContentIndex].ToString().ToBytes(EQTextEncoding::E_ASCII);
                std::ifstream inputFile(arFilePath.Get());

                if(inputFile.is_open())
                {
                    std::string strContent;
                    bool bEOF = false;

                    while(!bEOF)
                    {
                        bEOF = (inputFile >> strContent).eof();
                    }

                    string_q strFileContent = strContent.c_str();

                    // Checks the content of the file
                    bContentMatchesExpectations = bContentMatchesExpectations && strFileContent == strExpectedFileContent;
                }
                else
                {
                    bContentMatchesExpectations = false;
                }
            }
        }

        ++uContentIndex;
    }

    return bContentMatchesExpectations;
}



QTEST_SUITE_BEGIN( SQDirectory_TestSuite )

/// <summary>
/// Checks that it returns DoesNotExist when the directory does not exist.
/// </summary>
QTEST_CASE ( Delete_DirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Delete(DIRECTORY);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns Success when the directory exists and the directory along with its content are removed.
/// </summary>
QTEST_CASE ( Delete_DirectoryAndContentAreDeleted_Test )
{
    // Assuming the existence of:
    // -./DirectoryToDelete/
    // -./DirectoryToDelete/Content1/
    // -./DirectoryToDelete/Content2.txt

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToDelete/");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_Success;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Delete(DIRECTORY);
    
    // [Verification]
    WaitForCreationOrDeletion_SQDirectoryTestHelper(DIRECTORY, true);

    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDirectoryExists = SQDirectory::Exists(DIRECTORY, existsErrorCode);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(!bDirectoryExists);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( Delete_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Delete(DIRECTORY);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns DoesNotExist when the source directory does not exist.
/// </summary>
QTEST_CASE ( Move_SourceDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns DoesNotExist when the destination directory does not exist.
/// </summary>
QTEST_CASE ( Move_DestinationDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
}

/// <summary>
/// Checks that source directory and its content are moved when the destination has a directory with the same name and the replace flag is True.
/// </summary>
QTEST_CASE ( Move_SourceDirectoryAndContentAreMovedWhenDestinationHasContentWithSameNameAndReplaceIsTrue_Test )
{
    // Assuming the existence of:
    // -./DirectoryToMove/
    // -./DirectoryToMove/Content1/
    // -./DirectoryToMove/Content2.txt
    // -./DirectoryToMove/Content1/Content3.txt
    // -./MoveDestinationWithSameDirectory/
    // -./MoveDestinationWithSameDirectory/DirectoryToMove/
    // -./MoveDestinationWithSameDirectory/DirectoryToMove/Content1/
    // -./MoveDestinationWithSameDirectory/DirectoryToMove/Content2.txt
    // -./MoveDestinationWithSameDirectory/DirectoryToMove/Content1/Content3.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToMove/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory/");
    const QPath EXPECTED_DIRECTORY(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory/DirectoryToMove/");
    const QPath EXPECTED_CONTENT[] = { QPath(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory/DirectoryToMove/Content1/"),
                                       QPath(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory/DirectoryToMove/Content2.txt"),
                                       QPath(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory/DirectoryToMove/Content1/Content3.txt") };
    const string_q EXPECTED_FILE_CONTENT("MOVED");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_Success;
    const bool INPUT_REPLACE = true;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, INPUT_REPLACE);
    
    // [Verification]
    WaitForCreationOrDeletion_SQDirectoryTestHelper(EXPECTED_DIRECTORY, false);

    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDestinationDirectoryExists = SQDirectory::Exists(EXPECTED_DIRECTORY, existsErrorCode);
    bool bSourceDirectoryDoesntExist = !SQDirectory::Exists(SOURCE_DIRECTORY, existsErrorCode);
    bool bContentExists = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_CONTENT, 3), EXPECTED_FILE_CONTENT);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(bDestinationDirectoryExists);
    BOOST_CHECK(bSourceDirectoryDoesntExist);
    BOOST_CHECK(bContentExists);
}

/// <summary>
/// Checks that source directory is not moved when the destination has a directory with the same name and the replace flag is False.
/// </summary>
QTEST_CASE ( Move_SourceDirectoryIsNotMovedWhenDestinationHasContentWithSameNameAndReplaceIsFalse_Test )
{
    // Assuming the existence of:
    // -./DirectoryToMove2/
    // -./DirectoryToMove2/Content1/
    // -./DirectoryToMove2/Content2.txt
    // -./DirectoryToMove2/Content1/Content3.txt
    // -./MoveDestinationWithSameDirectory2/
    // -./MoveDestinationWithSameDirectory2/DirectoryToMove/
    // -./MoveDestinationWithSameDirectory2/DirectoryToMove/Content1/
    // -./MoveDestinationWithSameDirectory2/DirectoryToMove/Content1/Content3.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToMove2/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory2/");
    const QPath EXPECTED_CONTENT[] = { QPath(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory2/DirectoryToMove2/Content1/Content3.txt") };
    const QPath NOT_EXPECTED_CONTENT[] = {  QPath(PATH_TO_ARTIFACTS + "./MoveDestinationWithSameDirectory2/DirectoryToMove2/Content2.txt") };
    const string_q EXPECTED_FILE_CONTENT = string_q::GetEmpty();
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_AlreadyExists;
    const bool INPUT_REPLACE = false;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, INPUT_REPLACE);
    
    // [Verification]
    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDestinationFileContentHasNotChanged = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_CONTENT, 1), EXPECTED_FILE_CONTENT);
    bool bContentIsNotMixed = !CheckDirectoryContent(QArrayBasic<const QPath>(NOT_EXPECTED_CONTENT, 1), EXPECTED_FILE_CONTENT);
    bool bSourceDirectoryStillExists = SQDirectory::Exists(SOURCE_DIRECTORY, existsErrorCode);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(bSourceDirectoryStillExists);
    BOOST_CHECK(bContentIsNotMixed);
    BOOST_CHECK(bDestinationFileContentHasNotChanged);
}

/// <summary>
/// Checks that source directory and its content are moved when the destination has a directory with the same name and the replace flag is True.
/// </summary>
QTEST_CASE ( Move_SourceDirectoryAndContentAreMovedWhenDestinationIsEmptyAndReplaceIsFalse_Test )
{
    // Assuming the existence of:
    // -./DirectoryToMove3/
    // -./DirectoryToMove3/Content1/
    // -./DirectoryToMove3/Content2.txt
    // -./DirectoryToMove3/Content1/Content3.txt
    // -./MoveDestinationEmpty/

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToMove3/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./MoveDestinationEmpty/");
    const QPath EXPECTED_DIRECTORY(PATH_TO_ARTIFACTS + "./MoveDestinationEmpty/DirectoryToMove3/");
    const QPath EXPECTED_CONTENT[] = { QPath(PATH_TO_ARTIFACTS + "./MoveDestinationEmpty/DirectoryToMove3/Content1/"),
                                       QPath(PATH_TO_ARTIFACTS + "./MoveDestinationEmpty/DirectoryToMove3/Content2.txt"),
                                       QPath(PATH_TO_ARTIFACTS + "./MoveDestinationEmpty/DirectoryToMove3/Content1/Content3.txt") };
    const string_q EXPECTED_FILE_CONTENT("MOVED");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_Success;
    const bool INPUT_REPLACE = false;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, INPUT_REPLACE);
    
    // [Verification]
    WaitForCreationOrDeletion_SQDirectoryTestHelper(EXPECTED_DIRECTORY, false);

    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDestinationDirectoryExists = SQDirectory::Exists(EXPECTED_DIRECTORY, existsErrorCode);
    bool bSourceDirectoryDoesntExist = !SQDirectory::Exists(SOURCE_DIRECTORY, existsErrorCode);
    bool bContentExists = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_CONTENT, 3), EXPECTED_FILE_CONTENT);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(bDestinationDirectoryExists);
    BOOST_CHECK(bSourceDirectoryDoesntExist);
    BOOST_CHECK(bContentExists);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the source path does not refer to a directory.
/// </summary>
QTEST_CASE ( Move_AssertionFailsWhenSourcePathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

/// <summary>
/// Checks that an assertion fails when the destination path does not refer to a directory.
/// </summary>
QTEST_CASE ( Move_AssertionFailsWhenDestinationPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Move(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns DoesNotExist when the source directory does not exist.
/// </summary>
QTEST_CASE ( Rename_SourceDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath EXPECTED_NEW_DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    QPath sourceDirectory = EXPECTED_NEW_DIRECTORY;
    const string_q NEW_NAME("RenamedDirectory");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Rename(sourceDirectory, NEW_NAME);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(sourceDirectory == EXPECTED_NEW_DIRECTORY);
}

/// <summary>
/// Checks that it returns AlreadyExists when there is already a directory with the same name.
/// </summary>
QTEST_CASE ( Rename_ADirectoryWithSameNameAlreadyExists_Test )
{
    // Assuming the existence of:
    // -./DirectoryToRename/
    // -./ExistingRenamedDirectory/

    // [Preparation]
    const QPath EXPECTED_NEW_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToRename/");
    QPath sourceDirectory = EXPECTED_NEW_DIRECTORY;
    const string_q NEW_NAME("ExistingRenamedDirectory");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_AlreadyExists;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Rename(sourceDirectory, NEW_NAME);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(sourceDirectory == EXPECTED_NEW_DIRECTORY);
}

/// <summary>
/// Checks that it returns the new path whent he directory is successfully renamed.
/// </summary>
QTEST_CASE ( Rename_ItReturnsNewPathWhenDirectoryIsRenamed_Test )
{
    // Assuming the existence of:
    // -./DirectoryToRename2/

    // [Preparation]
    const QPath EXPECTED_NEW_DIRECTORY(PATH_TO_ARTIFACTS + "./RenamedDirectory2/");
    QPath sourceDirectory(PATH_TO_ARTIFACTS + "./DirectoryToRename2/");
    const string_q NEW_NAME("RenamedDirectory2");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_Success;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Rename(sourceDirectory, NEW_NAME);
    
    // [Verification]
    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bRenamedDirectoryExists = SQDirectory::Exists(sourceDirectory, existsErrorCode);
    BOOST_CHECK(bRenamedDirectoryExists);
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(sourceDirectory == EXPECTED_NEW_DIRECTORY);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the source path does not refer to a directory.
/// </summary>
QTEST_CASE ( Rename_AssertionFailsWhenSourcePathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    QPath sourceDirectory(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    const string_q NEW_NAME("RenamedDirectory");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Rename(sourceDirectory, NEW_NAME);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns DoesNotExist when the source directory does not exist.
/// </summary>
QTEST_CASE ( Copy_SourceDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
}

/// <summary>
/// Checks that it returns DoesNotExist when the destination directory does not exist.
/// </summary>
QTEST_CASE ( Copy_DestinationDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
}

/// <summary>
/// Checks that source directory and its content are copied when the destination has a directory with the same name and the replace flag is True.
/// </summary>
QTEST_CASE ( Copy_SourceDirectoryAndContentAreCopiedWhenDestinationHasContentWithSameNameAndReplaceIsTrue_Test )
{
    // Assuming the existence of:
    // -./DirectoryToCopy/
    // -./DirectoryToCopy/Content1/
    // -./DirectoryToCopy/Content2.txt
    // -./DirectoryToCopy/Content1/Content3.txt
    // -./CopyDestinationWithSameDirectory/
    // -./CopyDestinationWithSameDirectory/DirectoryToCopy/
    // -./CopyDestinationWithSameDirectory/DirectoryToCopy/Content1/
    // -./CopyDestinationWithSameDirectory/DirectoryToCopy/Content2.txt
    // -./CopyDestinationWithSameDirectory/DirectoryToCopy/Content1/Content3.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToCopy/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory/");
    const QPath EXPECTED_DIRECTORY(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory/DirectoryToCopy/");
    const QPath EXPECTED_CONTENT[] = { QPath(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory/DirectoryToCopy/Content1/"),
                                       QPath(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory/DirectoryToCopy/Content2.txt"),
                                       QPath(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory/DirectoryToCopy/Content1/Content3.txt") };
    const string_q EXPECTED_FILE_CONTENT("COPIED");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_Success;
    const bool INPUT_REPLACE = true;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, INPUT_REPLACE);
    
    // [Verification]
    WaitForCreationOrDeletion_SQDirectoryTestHelper(EXPECTED_DIRECTORY, false);

    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDestinationDirectoryExists = SQDirectory::Exists(EXPECTED_DIRECTORY, existsErrorCode);
    bool bSourceDirectoryStillExists = SQDirectory::Exists(SOURCE_DIRECTORY, existsErrorCode);
    bool bContentExists = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_CONTENT, 3), EXPECTED_FILE_CONTENT);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(bDestinationDirectoryExists);
    BOOST_CHECK(bSourceDirectoryStillExists);
    BOOST_CHECK(bContentExists);
}

/// <summary>
/// Checks that source directory is copied without replacing content when the destination has a directory with the same name and the replace flag is False.
/// </summary>
QTEST_CASE ( Copy_SourceDirectoryIsCopiedWithoutReplacingWhenDestinationHasContentWithSameNameAndReplaceIsFalse_Test )
{
    // Assuming the existence of:
    // -./DirectoryToCopy2/
    // -./DirectoryToCopy2/Content1/
    // -./DirectoryToCopy2/Content2.txt
    // -./DirectoryToCopy2/Content1/Content3.txt
    // -./CopyDestinationWithSameDirectory2/
    // -./CopyDestinationWithSameDirectory2/DirectoryToCopy/
    // -./CopyDestinationWithSameDirectory2/DirectoryToCopy/Content1/
    // -./CopyDestinationWithSameDirectory2/DirectoryToCopy/Content1/Content3.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToCopy2/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory2/");
    const QPath EXPECTED_CONTENT[] = { QPath(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory2/DirectoryToCopy2/Content1/Content3.txt") };
    const string_q EXPECTED_NOT_REPLACED_FILE_CONTENT = string_q::GetEmpty();
    const QPath EXPECTED_NEW_CONTENT[] = {  QPath(PATH_TO_ARTIFACTS + "./CopyDestinationWithSameDirectory2/DirectoryToCopy2/Content2.txt") };
    const string_q EXPECTED_COPIED_FILE_CONTENT("COPIED");
    
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_AlreadyExists;
    const bool INPUT_REPLACE = false;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, INPUT_REPLACE);
    
    // [Verification]
    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDestinationFileContentHasNotChanged = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_CONTENT, 1), EXPECTED_NOT_REPLACED_FILE_CONTENT);
    bool bNotExistingFilesHaveBeenCopied = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_NEW_CONTENT, 1), EXPECTED_COPIED_FILE_CONTENT);
    bool bSourceDirectoryStillExists = SQDirectory::Exists(SOURCE_DIRECTORY, existsErrorCode);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(bSourceDirectoryStillExists);
    BOOST_CHECK(bNotExistingFilesHaveBeenCopied);
    BOOST_CHECK(bDestinationFileContentHasNotChanged);
}

/// <summary>
/// Checks that source directory and its content are copied when the destination has a directory with the same name and the replace flag is False.
/// </summary>
QTEST_CASE ( Copy_SourceDirectoryAndContentAreCopydWhenDestinationIsEmptyAndReplaceIsFalse_Test )
{
    // Assuming the existence of:
    // -./DirectoryToCopy3/
    // -./DirectoryToCopy3/Content1/
    // -./DirectoryToCopy3/Content2.txt
    // -./DirectoryToCopy3/Content1/Content3.txt
    // -./CopyDestinationEmpty/

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryToCopy3/");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./CopyDestinationEmpty/");
    const QPath EXPECTED_DIRECTORY(PATH_TO_ARTIFACTS + "./CopyDestinationEmpty/DirectoryToCopy3/");
    const QPath EXPECTED_CONTENT[] = { QPath(PATH_TO_ARTIFACTS + "./CopyDestinationEmpty/DirectoryToCopy3/Content1/"),
                                       QPath(PATH_TO_ARTIFACTS + "./CopyDestinationEmpty/DirectoryToCopy3/Content2.txt"),
                                       QPath(PATH_TO_ARTIFACTS + "./CopyDestinationEmpty/DirectoryToCopy3/Content1/Content3.txt") };
    const string_q EXPECTED_FILE_CONTENT("COPIED");
    const EQFileSystemError EXPECTED_RESULT = EQFileSystemError::E_Success;
    const bool INPUT_REPLACE = true;

    // [Execution]
    EQFileSystemError eErrorCode = SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, INPUT_REPLACE);
    
    // [Verification]
    WaitForCreationOrDeletion_SQDirectoryTestHelper(EXPECTED_DIRECTORY, false);

    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDestinationDirectoryExists = SQDirectory::Exists(EXPECTED_DIRECTORY, existsErrorCode);
    bool bSourceDirectoryStillExists = SQDirectory::Exists(SOURCE_DIRECTORY, existsErrorCode);
    bool bContentExists = CheckDirectoryContent(QArrayBasic<const QPath>(EXPECTED_CONTENT, 3), EXPECTED_FILE_CONTENT);

    BOOST_CHECK(eErrorCode == EXPECTED_RESULT);
    BOOST_CHECK(bDestinationDirectoryExists);
    BOOST_CHECK(bSourceDirectoryStillExists);
    BOOST_CHECK(bContentExists);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the source path does not refer to a directory.
/// </summary>
QTEST_CASE ( Copy_AssertionFailsWhenSourcePathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

/// <summary>
/// Checks that an assertion fails when the destination path does not refer to a directory.
/// </summary>
QTEST_CASE ( Copy_AssertionFailsWhenDestinationPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath SOURCE_DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const QPath DESTINATION_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Copy(SOURCE_DIRECTORY, DESTINATION_DIRECTORY, true);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns an existing path.
/// </summary>
QTEST_CASE ( GetCurrentWorkingDirectory_ReturnsExistingPath_Test )
{
    // [Preparation]
    
    // [Execution]
    QPath currentDir = SQDirectory::GetCurrentWorkingDirectory();
    
    // [Verification]
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    bool bExists = SQDirectory::Exists(currentDir, eErrorCode);
    BOOST_CHECK(bExists);
}

/// <summary>
/// Checks that it returns an absolute path.
/// </summary>
QTEST_CASE ( GetCurrentWorkingDirectory_ReturnsAbsolutePath_Test )
{
    // [Preparation]
    
    // [Execution]
    QPath currentDir = SQDirectory::GetCurrentWorkingDirectory();
    
    // [Verification]
    bool bIsAbsolute = currentDir.IsAbsolute();
    BOOST_CHECK(bIsAbsolute);
}

/// <summary>
/// Checks that the path is correctly stored when it exists.
/// </summary>
QTEST_CASE ( SetCurrentWorkingDirectory_PathIsCorrectlyStoredWhenItExists_Test )
{
    // [Preparation]
    const QPath ORIGINAL_CWD = SQDirectory::GetCurrentWorkingDirectory();
    const QPath NEW_CWD(ORIGINAL_CWD.ToString() + PATH_TO_ARTIFACTS + "./ExistingPath/");
    const EQFileSystemError EXPECTED_ERROR_CODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    SQDirectory::SetCurrentWorkingDirectory(NEW_CWD, eErrorCode);
    
    // [Verification]
    QPath currentDir = SQDirectory::GetCurrentWorkingDirectory();
    BOOST_CHECK(currentDir == NEW_CWD);
    BOOST_CHECK(eErrorCode == EXPECTED_ERROR_CODE);

    // [Cleaning]
    SQDirectory::SetCurrentWorkingDirectory(ORIGINAL_CWD, eErrorCode);
}

/// <summary>
/// Checks that the path is not stored when it does not exist.
/// </summary>
QTEST_CASE ( SetCurrentWorkingDirectory_PathIsNotStoredWhenItDoesNotExist_Test )
{
    // [Preparation]
    const QPath ORIGINAL_CWD = SQDirectory::GetCurrentWorkingDirectory();
    const QPath NEW_CWD(PATH_TO_ARTIFACTS + "./NotExistingPath/");
    const EQFileSystemError EXPECTED_ERROR_CODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    SQDirectory::SetCurrentWorkingDirectory(NEW_CWD, eErrorCode);
    
    // [Verification]
    QPath currentDir = SQDirectory::GetCurrentWorkingDirectory();
    BOOST_CHECK(currentDir == ORIGINAL_CWD);
    BOOST_CHECK(eErrorCode == EXPECTED_ERROR_CODE);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( SetCurrentWorkingDirectory_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::SetCurrentWorkingDirectory(NOT_A_DIRECTORY, eErrorCode);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns False when the directory does not exist.
/// </summary>
QTEST_CASE ( Exists_ReturnsFalseWhenDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");
    const bool EXPECTED_VALUE = false;
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    bool bReturnedValue = SQDirectory::Exists(DIRECTORY, eErrorCode);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK_EQUAL(bReturnedValue, EXPECTED_VALUE);
}

/// <summary>
/// Checks that it returns True when the directory exists.
/// </summary>
QTEST_CASE ( Exists_ReturnsTrueWhenDirectoryExists_Test )
{
    // Assumes the existence of:
    // -./ExistingPath/

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./ExistingPath/");
    const bool EXPECTED_VALUE = true;
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    bool bReturnedValue = SQDirectory::Exists(DIRECTORY, eErrorCode);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK_EQUAL(bReturnedValue, EXPECTED_VALUE);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( Exists_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
        SQDirectory::Exists(NOT_A_DIRECTORY, eErrorCode);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns DoesNotExist when the directory does not exist.
/// </summary>
QTEST_CASE ( GetDirectoryInfo_DirectoryDoesNotExist_Test )
{
    using Kinesis::QuimeraEngine::System::IO::FileSystem::QDirectoryInfo;
    using Kinesis::QuimeraEngine::Tools::Time::QDateTime;

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./XXXThisDirectoryDoesNotExistXXX/");

    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_DoesNotExist;

    // [Execution]
    QDirectoryInfo dirInfo = SQDirectory::GetDirectoryInfo(DIRECTORY, eErrorCode);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that it returns expected information when the directory exists.
/// </summary>
QTEST_CASE ( GetDirectoryInfo_ReturnsExpectedInformationWhenDirectoryExists_Test )
{
    using Kinesis::QuimeraEngine::System::IO::FileSystem::QDirectoryInfo;
    using Kinesis::QuimeraEngine::Tools::Time::QDateTime;

    // Assumes the existence of:
    // -./ExistingPath/

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./ExistingPath/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    QDirectoryInfo dirInfo = SQDirectory::GetDirectoryInfo(DIRECTORY, eErrorCode);
    
    // [Verification]
    bool bContainsCorrectPath = dirInfo.GetPath() == DIRECTORY;
    bool bCreationDateTimeIsNotUndefined = !dirInfo.GetCreationDate().IsUndefined();
    
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(bContainsCorrectPath);
    BOOST_CHECK(bCreationDateTimeIsNotUndefined);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( GetDirectoryInfo_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
        SQDirectory::GetDirectoryInfo(NOT_A_DIRECTORY, eErrorCode);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns Success and the directory is created when the input directory exists and the new directory does not exist.
/// </summary>
QTEST_CASE ( Create_ReturnsSuccessWhenInputDirectoryExistsAndNewDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const string_q NEW_DIRECTORY_NAME("NewDirectory");
    QPath EXPECTED_DIRECTORY(DIRECTORY);
    EXPECTED_DIRECTORY.AppendDirectory(NEW_DIRECTORY_NAME);
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    eErrorCode = SQDirectory::Create(DIRECTORY, NEW_DIRECTORY_NAME);
    
    // [Verification]
    WaitForCreationOrDeletion_SQDirectoryTestHelper(EXPECTED_DIRECTORY, false);
    
    EQFileSystemError existsErrorCode = EQFileSystemError::E_Success;
    bool bDirectoryWasCreated = SQDirectory::Exists(EXPECTED_DIRECTORY, existsErrorCode);
    BOOST_CHECK(bDirectoryWasCreated);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that it returns Success and the directory is created when the input directory exists and the new directory already exists.
/// </summary>
QTEST_CASE ( Create_ReturnsSuccessWhenInputDirectoryExistsAndNewDirectoryAlreadyExists_Test )
{
    // Assumes the existence of:
    // -./ExistingPath/

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./");
    const string_q NEW_DIRECTORY_NAME("ExistingPath");
    QPath EXPECTED_DIRECTORY(DIRECTORY);
    EXPECTED_DIRECTORY.AppendDirectory(NEW_DIRECTORY_NAME);
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    eErrorCode = SQDirectory::Create(DIRECTORY, NEW_DIRECTORY_NAME);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that it returns DoesNotExist when the input directory does not exist.
/// </summary>
QTEST_CASE ( Create_ReturnsDoesNotExistWhenInputDirectoryExistsAndNewDirectoryAlreadyExists_Test )
{
    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./NonExistingPath/");
    const string_q NEW_DIRECTORY_NAME("Whatever");
    QPath EXPECTED_DIRECTORY(DIRECTORY);
    EXPECTED_DIRECTORY.AppendDirectory(NEW_DIRECTORY_NAME);
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;

    // [Execution]
    eErrorCode = SQDirectory::Create(DIRECTORY, NEW_DIRECTORY_NAME);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( Create_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::Create(NOT_A_DIRECTORY, "DirectoryName");
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns Success and fills the container with expected file paths when the input directory exists.
/// </summary>
QTEST_CASE ( GetFiles1_ReturnsSuccessAndExpectedFilesWhenInputDirectoryExists_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/
    // -./DirectoryWithContent/Directory1/File3.txt
    // -./DirectoryWithContent/Directory2/
    // -./DirectoryWithContent/File1.txt
    // -./DirectoryWithContent/File2.txt
    // -./DirectoryWithContent/File4.log

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryWithContent/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<string_q> EXPECTED_FILES;
    EXPECTED_FILES.Add("File1.txt");
    EXPECTED_FILES.Add("File2.txt");
    EXPECTED_FILES.Add("File4.log");

    QArrayDynamic<QPath> arFiles;

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles);
    
    // [Verification]
    bool bAllExpectedFilesRetrieved = true;

    for(pointer_uint_q i = 0; i < sizeof(EXPECTED_FILES) / sizeof(string_q); ++i)
        bAllExpectedFilesRetrieved = bAllExpectedFilesRetrieved && EXPECTED_FILES.Contains(arFiles[i].GetFilename());
    
    BOOST_CHECK(bAllExpectedFilesRetrieved);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that found file paths are appended to the existing elements in the container.
/// </summary>
QTEST_CASE ( GetFiles1_PathsAreAppended_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/
    // -./DirectoryWithContent/Directory1/File3.txt
    // -./DirectoryWithContent/Directory2/
    // -./DirectoryWithContent/File1.txt
    // -./DirectoryWithContent/File2.txt
    // -./DirectoryWithContent/File4.log

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryWithContent/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<string_q> EXPECTED_FILES;
    EXPECTED_FILES.Add("ExistingElement1");
    EXPECTED_FILES.Add("ExistingElement2");
    EXPECTED_FILES.Add("File1.txt");
    EXPECTED_FILES.Add("File2.txt");
    EXPECTED_FILES.Add("File4.log");

    QArrayDynamic<QPath> arFiles;
    arFiles.Add(QPath("ExistingElement1"));
    arFiles.Add(QPath("ExistingElement2"));

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles);
    
    // [Verification]
    bool bAllExpectedFilesRetrieved = true;

    for(pointer_uint_q i = 0; i < sizeof(EXPECTED_FILES) / sizeof(string_q); ++i)
        bAllExpectedFilesRetrieved = bAllExpectedFilesRetrieved && EXPECTED_FILES.Contains(arFiles[i].GetFilename());
    
    BOOST_CHECK(bAllExpectedFilesRetrieved);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that it returns Success when the directory is empty.
/// </summary>
QTEST_CASE ( GetFiles1_ReturnsSuccessWhenDirectoryIsEmpty_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./EmptyDirectory/

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./EmptyDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<QPath> arFiles;

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(arFiles.IsEmpty());
}

/// <summary>
/// Checks that it returns DoesNotExist when the directory does not exist.
/// </summary>
QTEST_CASE ( GetFiles1_ReturnsDoesNotExistWhenTheDirectoryDoesNotExist_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./NonExistentDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<QPath> arFiles;

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( GetFiles1_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    QArrayDynamic<QPath> arFiles;
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::GetFiles(NOT_A_DIRECTORY, arFiles);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns Success and fills the container with expected file paths when the input directory exists.
/// </summary>
QTEST_CASE ( GetFiles2_ReturnsSuccessAndExpectedFilesWhenInputDirectoryExists_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/
    // -./DirectoryWithContent/Directory1/File3.txt
    // -./DirectoryWithContent/Directory2/
    // -./DirectoryWithContent/File1.txt
    // -./DirectoryWithContent/File2.txt
    // -./DirectoryWithContent/File4.log

    // [Preparation]
    const string_q EXTENSION_FILTER("txt");
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryWithContent/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<string_q> EXPECTED_FILES;
    EXPECTED_FILES.Add("File1.txt");
    EXPECTED_FILES.Add("File2.txt");

    QArrayDynamic<QPath> arFiles;

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles, EXTENSION_FILTER);
    
    // [Verification]
    bool bAllExpectedFilesRetrieved = true;

    for(pointer_uint_q i = 0; i < sizeof(EXPECTED_FILES) / sizeof(string_q); ++i)
        bAllExpectedFilesRetrieved = bAllExpectedFilesRetrieved && EXPECTED_FILES.Contains(arFiles[i].GetFilename());
    
    BOOST_CHECK(bAllExpectedFilesRetrieved);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that found file paths are appended to the existing elements in the container.
/// </summary>
QTEST_CASE ( GetFiles2_PathsAreAppended_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/
    // -./DirectoryWithContent/Directory1/File3.txt
    // -./DirectoryWithContent/Directory2/
    // -./DirectoryWithContent/File1.txt
    // -./DirectoryWithContent/File2.txt
    // -./DirectoryWithContent/File4.log

    // [Preparation]
    const string_q EXTENSION_FILTER("txt");
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryWithContent/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<string_q> EXPECTED_FILES;
    EXPECTED_FILES.Add("ExistingElement1");
    EXPECTED_FILES.Add("ExistingElement2");
    EXPECTED_FILES.Add("File1.txt");
    EXPECTED_FILES.Add("File2.txt");

    QArrayDynamic<QPath> arFiles;
    arFiles.Add(QPath("ExistingElement1"));
    arFiles.Add(QPath("ExistingElement2"));

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles, EXTENSION_FILTER);
    
    // [Verification]
    bool bAllExpectedFilesRetrieved = true;

    for(pointer_uint_q i = 0; i < sizeof(EXPECTED_FILES) / sizeof(string_q); ++i)
        bAllExpectedFilesRetrieved = bAllExpectedFilesRetrieved && EXPECTED_FILES.Contains(arFiles[i].GetFilename());
    
    BOOST_CHECK(bAllExpectedFilesRetrieved);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that it returns Success when the directory is empty.
/// </summary>
QTEST_CASE ( GetFiles2_ReturnsSuccessWhenDirectoryIsEmpty_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./EmptyDirectory/

    // [Preparation]
    const string_q EXTENSION_FILTER("txt");
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./EmptyDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<QPath> arFiles;

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles, EXTENSION_FILTER);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(arFiles.IsEmpty());
}

/// <summary>
/// Checks that it returns DoesNotExist when the directory does not exist.
/// </summary>
QTEST_CASE ( GetFiles2_ReturnsDoesNotExistWhenTheDirectoryDoesNotExist_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // [Preparation]
    const string_q EXTENSION_FILTER("txt");
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./NonExistentDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<QPath> arFiles;

    // [Execution]
    eErrorCode = SQDirectory::GetFiles(DIRECTORY, arFiles, EXTENSION_FILTER);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( GetFiles2_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const string_q EXTENSION_FILTER("txt");
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    QArrayDynamic<QPath> arFiles;
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::GetFiles(NOT_A_DIRECTORY, arFiles, EXTENSION_FILTER);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns Success and fills the container with expected directory paths when the input directory exists.
/// </summary>
QTEST_CASE ( GetSubdirectories_ReturnsSuccessAndExpectedDirectoriesWhenInputDirectoryExists_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/
    // -./DirectoryWithContent/Directory1/File3.txt
    // -./DirectoryWithContent/Directory2/
    // -./DirectoryWithContent/File1.txt
    // -./DirectoryWithContent/File2.txt
    // -./DirectoryWithContent/File4.log

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryWithContent/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<string_q> EXPECTED_DIRS;
    EXPECTED_DIRS.Add("Directory1");
    EXPECTED_DIRS.Add("Directory2");

    QArrayDynamic<QPath> arDirectories;

    // [Execution]
    eErrorCode = SQDirectory::GetSubdirectories(DIRECTORY, arDirectories);
    
    // [Verification]
    bool bAllExpectedDirectoriesRetrieved = true;

    for(pointer_uint_q i = 0; i < sizeof(EXPECTED_DIRS) / sizeof(string_q); ++i)
        bAllExpectedDirectoriesRetrieved = bAllExpectedDirectoriesRetrieved && EXPECTED_DIRS.Contains(arDirectories[i].GetLastDirectory());
    
    BOOST_CHECK(bAllExpectedDirectoriesRetrieved);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that found directory paths are appended to the existing elements in the container.
/// </summary>
QTEST_CASE ( GetSubdirectories_PathsAreAppended_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/
    // -./DirectoryWithContent/Directory1/File3.txt
    // -./DirectoryWithContent/Directory2/
    // -./DirectoryWithContent/File1.txt
    // -./DirectoryWithContent/File2.txt
    // -./DirectoryWithContent/File4.log

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./DirectoryWithContent/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<string_q> EXPECTED_DIRS;
    EXPECTED_DIRS.Add("ExistingElement1");
    EXPECTED_DIRS.Add("ExistingElement2");
    EXPECTED_DIRS.Add("Directory1");
    EXPECTED_DIRS.Add("Directory2");

    QArrayDynamic<QPath> arDirectories;
    arDirectories.Add(QPath("/ExistingElement1/"));
    arDirectories.Add(QPath("/ExistingElement2/"));

    // [Execution]
    eErrorCode = SQDirectory::GetSubdirectories(DIRECTORY, arDirectories);
    
    // [Verification]
    bool bAllExpectedDirectoriesRetrieved = true;

    for(pointer_uint_q i = 0; i < sizeof(EXPECTED_DIRS) / sizeof(string_q); ++i)
        bAllExpectedDirectoriesRetrieved = bAllExpectedDirectoriesRetrieved && EXPECTED_DIRS.Contains(arDirectories[i].GetLastDirectory());
    
    BOOST_CHECK(bAllExpectedDirectoriesRetrieved);
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

/// <summary>
/// Checks that it returns Success when the directory is empty.
/// </summary>
QTEST_CASE ( GetSubdirectories_ReturnsSuccessWhenDirectoryIsEmpty_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assumes the existence of:
    // -./EmptyDirectory/

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./EmptyDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<QPath> arDirectories;

    // [Execution]
    eErrorCode = SQDirectory::GetSubdirectories(DIRECTORY, arDirectories);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(arDirectories.IsEmpty());
}

/// <summary>
/// Checks that it returns DoesNotExist when the directory does not exist.
/// </summary>
QTEST_CASE ( GetSubdirectories_ReturnsDoesNotExistWhenTheDirectoryDoesNotExist_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./NonExistentDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    QArrayDynamic<QPath> arDirectories;

    // [Execution]
    eErrorCode = SQDirectory::GetSubdirectories(DIRECTORY, arDirectories);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( GetSubdirectories_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    QArrayDynamic<QPath> arDirectories;
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::GetSubdirectories(NOT_A_DIRECTORY, arDirectories);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

/// <summary>
/// Checks that it returns Success when the directory exists and has parent.
/// </summary>
QTEST_CASE ( GetParentDirectory_ReturnsSuccessWhenDirectoryExistsAndHasParent_Test )
{
    // Assumes the existence of:
    // -./DirectoryWithContent/
    // -./DirectoryWithContent/Directory1/

    // [Preparation]
    const QPath DIRECTORY(QPath(PATH_TO_ARTIFACTS + "./DirectoryWithContent/Directory1/").GetAbsolutePath());
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    const string_q EXPECTED_RELATIVE_PATH("./Directory1/");

    // [Execution]
    QPath parentPath = SQDirectory::GetParentDirectory(DIRECTORY, eErrorCode);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(parentPath.GetRelativePathTo(DIRECTORY) == EXPECTED_RELATIVE_PATH);
}

/// <summary>
/// Checks that it returns Success and the same path when the directory exists and does not have parent.
/// </summary>
QTEST_CASE ( GetParentDirectory_ReturnsSuccessAndSamePathWhenDirectoryExistsAndDoesNotHaveParent_Test )
{
    // [Preparation]
    const QPath DIRECTORY = SQDirectory::GetCurrentWorkingDirectory().GetRoot();
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_Success;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    const QPath EXPECTED_PATH = DIRECTORY;

    // [Execution]
    QPath parentPath = SQDirectory::GetParentDirectory(DIRECTORY, eErrorCode);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(parentPath == EXPECTED_PATH);
}

/// <summary>
/// Checks that it returns DoesNotExist when the directory does not exist.
/// </summary>
QTEST_CASE ( GetParentDirectory_ReturnsDoesNotExistWhenDirectoryDoesNotExist_Test )
{
    // [Preparation]
    const QPath DIRECTORY(PATH_TO_ARTIFACTS + "./NonExistentDirectory/");
    const EQFileSystemError EXPECTED_ERRORCODE = EQFileSystemError::E_DoesNotExist;
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    const QPath EXPECTED_PATH("./");

    // [Execution]
    QPath parentPath = SQDirectory::GetParentDirectory(DIRECTORY, eErrorCode);
    
    // [Verification]
    BOOST_CHECK(eErrorCode == EXPECTED_ERRORCODE);
    BOOST_CHECK(parentPath == EXPECTED_PATH);
}

#if QE_CONFIG_ASSERTSBEHAVIOR_DEFAULT == QE_CONFIG_ASSERTSBEHAVIOR_THROWEXCEPTIONS

/// <summary>
/// Checks that an assertion fails when the input path does not refer to a directory.
/// </summary>
QTEST_CASE ( GetParentDirectory_AssertionFailsWhenInputPathIsNotDirectory_Test )
{
    using Kinesis::QuimeraEngine::Tools::Containers::QArrayDynamic;

    // Assuming the existence of:
    // -./NotADirectory.txt

    // [Preparation]
    const QPath NOT_A_DIRECTORY(PATH_TO_ARTIFACTS + "./NotADirectory.txt");
    EQFileSystemError eErrorCode = EQFileSystemError::E_Success;
    bool bAssertionFailed = false;

    // [Execution]
    try
    {
        SQDirectory::GetParentDirectory(NOT_A_DIRECTORY, eErrorCode);
    }
    catch(const QAssertException&)
    {
        bAssertionFailed = true;
    }
    
    // [Verification]
    BOOST_CHECK(bAssertionFailed);
}

#endif

// End - Test Suite: SQDirectory
QTEST_SUITE_END()
