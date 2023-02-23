#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QJSONMAKER_LIB)
#  define QJSONMAKER_EXPORT Q_DECL_EXPORT
# else
#  define QJSONMAKER_EXPORT Q_DECL_IMPORT
# endif
#else
# define QJSONMAKER_EXPORT
#endif
