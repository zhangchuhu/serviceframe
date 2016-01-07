#ifndef FORM_ENTRIES_EXT_H_
#define FORM_ENTRIES_EXT_H_
#include "common/core/ibase.h"

// multiple form entries support
// id: [a-zA-Z0-9]+
#define GET_FORM_ENTRIES_EXT(id, theClass) theClass::getFormEntries_##id()

#define DECLARE_FORM_MAP_EXT(id) static core::FormEntry *getFormEntries_##id(); \
						static core::FormEntry formEntries_##id[];

#define BEGIN_FORM_MAP_EXT(id, theClass) \
	FormEntry* theClass::getFormEntries_##id()\
{ return theClass::formEntries_##id; } \
	FormEntry theClass::formEntries_##id[] = \
{ \

// end of multiple form entries support
#endif
