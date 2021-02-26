module;

#include "defines.h"
#include <algorithm>

export module Saigon.Algorithms;

SAIGON_NAMESPACE_BEGIN
template <typename Container, typename Seperator, typename Callback>
void split(Container const& con, Seperator sep, Callback f)
{
	auto it = std::find_if(std::cbegin(con), std::cend(con), sep);
	if (std::cend(con) == it) {

	}
}
SAIGON_NAMESPACE_END