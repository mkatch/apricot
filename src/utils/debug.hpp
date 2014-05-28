#ifndef UTILS_DEBUG_HPP
#define UTILS_DEBUG_HPP

#include <QDebug>
#include <QEvent>

QDebug operator << (QDebug debug, const QEvent *event);

#endif // UTILS_DEBUG_HPP
