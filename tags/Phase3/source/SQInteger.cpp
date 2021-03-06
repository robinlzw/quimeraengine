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

#include "SQInteger.h"

#include <stdlib.h>


namespace Kinesis
{
namespace QuimeraEngine
{
namespace Common
{
namespace DataTypes
{

//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |  CONSTANTS INITIALIZATION  |               ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

const u8_q SQInteger::MaxU8_Q = 0xFF;
const u16_q SQInteger::MaxU16_Q = 0xFFFF;
const u32_q SQInteger::MaxU32_Q = 0xFFFFFFFF;
const u64_q SQInteger::MaxU64_Q = 0xFFFFFFFFFFFFFFFFULL;
const i8_q SQInteger::MaxPositiveI8_Q = 127;
const i8_q SQInteger::MaxNegativeI8_Q = -128;
const i16_q SQInteger::MaxPositiveI16_Q = 32767;
const i16_q SQInteger::MaxNegativeI16_Q = -32768;
const i32_q SQInteger::MaxPositiveI32_Q = 2147483647;
const i32_q SQInteger::MaxNegativeI32_Q = -2147483647 - 1;// Defined this way to avoid compiler warnings due to size overflow, see http://bytes.com/topic/c/answers/674140-int-2147483648-a
const i64_q SQInteger::MaxPositiveI64_Q = 9223372036854775807LL;
const i64_q SQInteger::MaxNegativeI64_Q = -9223372036854775807LL - 1LL;// Defined this way to avoid compiler warnings due to size overflow, see http://bytes.com/topic/c/answers/674140-int-2147483648-a


//##################=======================================================##################
//##################             ____________________________              ##################
//##################            |                            |             ##################
//##################            |           METHODS          |             ##################
//##################           /|                            |\            ##################
//##################             \/\/\/\/\/\/\/\/\/\/\/\/\/\/              ##################
//##################                                                       ##################
//##################=======================================================##################

i8_q SQInteger::Abs(const i8_q nValue)
{
    return abs(nValue);
}

i16_q SQInteger::Abs(const i16_q nValue)
{
    return abs(nValue);
}

i32_q SQInteger::Abs(const i32_q nValue)
{
    return abs(nValue);
}

i64_q SQInteger::Abs(const i64_q nValue)
{
    return llabs(nValue);
}


} //namespace DataTypes
} //namespace Common
} //namespace QuimeraEngine
} //namespace Kinesis
