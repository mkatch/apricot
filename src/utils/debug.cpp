#include "debug.hpp"

#include <QMetaEnum>

QDebug operator << (QDebug debug, const QEvent *event)
{
   static int eventEnumIndex = QEvent::staticMetaObject.indexOfEnumerator("Type");
   debug << "QEvent";

   if (event != nullptr) {
      QString name = QEvent::staticMetaObject.enumerator(eventEnumIndex).valueToKey(event->type());
      if (!name.isEmpty()) {
        debug << name;
      } else {
        debug << event->type();
      }
   } else {
      debug << (void*)event;
   }

   return debug.maybeSpace();
}
