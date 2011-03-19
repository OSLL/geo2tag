#ifndef QT_SIGNALS_FIX_H
#define QT_SIGNALS_FIX_H

#ifdef REDEFINE_QT_SIGNALS

//#undef Q_SLOTS
//#undef  Q_SIGNALS   protected
#undef  Q_SLOT
#undef  Q_SIGNAL

//#define Q_SLOTS
//#define Q_SIGNALS   protected
#define Q_SLOT(a)     "1"#a
#define Q_SIGNAL(a)   "2"#a

#endif // REDEFINE_QT_SIGNALS


#endif // QT_SIGNALS_FIX_H
