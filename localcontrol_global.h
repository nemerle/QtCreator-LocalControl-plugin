#pragma once

#include <QtGlobal>

#if defined(LOCALCONTROL_LIBRARY)
#  define LOCALCONTROLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOCALCONTROLSHARED_EXPORT Q_DECL_IMPORT
#endif
