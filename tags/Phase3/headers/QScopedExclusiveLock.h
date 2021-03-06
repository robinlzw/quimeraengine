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

#ifndef __QSCOPEDEXCLUSIVELOCK__
#define __QSCOPEDEXCLUSIVELOCK__

#include "SystemDefinitions.h"
#include "QMutex.h"
#include "Assertions.h"
#include "StringsDefinitions.h"
#include <boost/thread/locks.hpp>

namespace Kinesis
{
namespace QuimeraEngine
{
namespace System
{
namespace Threading
{

/// <summary>
/// Wraps a mutex which will be unlocked when the control flow leaves the scope where the wrapping instance was declared. Mutexes are locked/unlocked in an exclusive way.
/// </summary>
/// <remarks>
/// This class is not thread-safe.<br/>
/// It is a wrapper for the unique_lock class, from Boost libraries.
/// </remarks>
/// <typeparam name="MutexT">Optional. The type of the mutex to be locked. By default, it is QMutex.</typeparam>
template<class MutexT=QMutex>
class QScopedExclusiveLock
{
    // TYPEDEFS
    // ---------------
public:

    typedef boost::unique_lock<typename MutexT::WrappedType> WrappedType;


    // CONSTRUCTORS
    // ---------------
public:

    /// <summary>
    /// Constructor that receives a mutex, optionally locking it.
    /// </summary>
    /// <param name="mutex">[IN] A mutex to be wrapped into a scope.</param>
    /// <param name="bLock">[Optional][IN] Indicates whether the mutex has to be locked when the wrapper is created; if not, it can be locked later. 
    /// By default, the mutex is locked.</param>
    QScopedExclusiveLock(MutexT &mutex, bool bLock=true) : m_lock(mutex.GetWrappedObject(), boost::defer_lock_t())
    {
        if(bLock)
            this->Lock();
    }
    

    // DESTRUCTOR
    // ---------------
public:

    /// <summary>
    /// Destructor. If the mutex was locked, it will be unlocked.
    /// </summary>
    ~QScopedExclusiveLock()
    {
    }


    // METHODS
    // ---------------
public:
    
    /// <summary>
    /// Locks the mutex in an exclusive way.
    /// </summary>
    /// <remarks>
    /// If the mutex is already locked by the calling thread, this method cannot be called again.
    /// </remarks>
    void Lock()
    {
        QE_ASSERT_ERROR(!this->IsOwner(), "The lock already owns the mutex. This operation could lead to a dead-lock.");

        m_lock.lock();
    }
    
    /// <summary>
    /// Unlocks the mutex in an exclusive way.
    /// </summary>
    /// <remarks>
    /// Locks can only unlock mutexes they locked before.
    /// </remarks>
    void Unlock()
    {
        QE_ASSERT_ERROR(this->IsOwner(), "The lock does not own the mutex. Locks can only unlock mutexes they locked before.");

        m_lock.unlock();
    }
    
    /// <summary>
    /// Locks the mutex in an exclusive way. If the mutex is already locked by another thread, it returns immediately.
    /// </summary>
    /// <remarks>
    /// If the mutex is already locked by the calling thread, this method cannot be called again.
    /// </remarks>
    /// <returns>
    /// True if the mutex has been locked by the calling thread; False otherwise.
    /// </returns>
    bool TryLock()
    {
        QE_ASSERT_ERROR(!this->IsOwner(), "The lock already owns the mutex. This operation could lead to a dead-lock.");

        return m_lock.try_lock();
    }


    // PROPERTIES
    // ---------------
public:
    
    /// <summary>
    /// Indicates whether the lock owns the mutex.
    /// </summary>
    /// <returns>
    /// True if the lock owns the mutex; False otherwise.
    /// </returns>
    bool IsOwner() const
    {
        return m_lock.owns_lock();
    }

    /// <summary>
    /// Gets the wrapped lock instance.
    /// </summary>
    /// <remarks>
    /// The usage of this method is discouraged unless it is absolutely necessary. It may not be available in future versions.
    /// </remarks>
    /// <returns>
    /// The wrapped instance.
    /// </returns>
    typename QScopedExclusiveLock::WrappedType& GetWrappedObject()
    {
        return m_lock;
    }
    
    /// <summary>
    /// Gets the wrapped lock instance.
    /// </summary>
    /// <remarks>
    /// The usage of this method is discouraged unless it is absolutely necessary. It may not be available in future versions.
    /// </remarks>
    /// <returns>
    /// The wrapped instance.
    /// </returns>
    const typename QScopedExclusiveLock::WrappedType& GetWrappedObject() const
    {
        return m_lock;
    }


    // ATTRIBUTES
    // ---------------
protected:

    /// <summary>
    /// The wrapped lock instance.
    /// </summary>
    typename QScopedExclusiveLock::WrappedType m_lock;
};

} //namespace Threading
} //namespace System
} //namespace QuimeraEngine
} //namespace Kinesis

#endif // __QSCOPEDEXCLUSIVELOCK__
