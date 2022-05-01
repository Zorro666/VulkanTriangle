//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Foundation/NSDate.hpp
//
// See LICENSE.txt for this project licensing information.
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "NSDefines.hpp"
#include "NSObject.hpp"
#include "NSPrivate.hpp"
#include "NSTypes.hpp"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace NS
{

using TimeInterval = double;

class Date : public Copying<Date>
{
public:
    static Date* dateWithTimeIntervalSinceNow(TimeInterval secs);
    static Date* distantPast();
};

} // NS

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE NS::Date* NS::Date::dateWithTimeIntervalSinceNow(NS::TimeInterval secs)
{
    return NS::Object::sendMessage<NS::Date*>(_NS_PRIVATE_CLS(NSDate), _NS_PRIVATE_SEL(dateWithTimeIntervalSinceNow_), secs);
}

_NS_INLINE NS::Date* NS::Date::distantPast()
{
    return NS::Object::sendMessage<NS::Date*>(_NS_PRIVATE_CLS(NSDate), _NS_PRIVATE_SEL(distantPast) );
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
