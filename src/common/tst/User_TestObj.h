#ifndef USER_TESTOBJ_H
#define USER_TESTOBJ_H

#include "../inc/User.h"

namespace Test
{
  class UserTestImpl : public User
  {
    Q_OBJECT;

    public:

      UserTestImpl(const QString& login, const QString& passw) : User(login,passw)
      {

      }

      virtual qlonglong getId() const
      {
        return 0;
      }

      friend class User_Test;
  };

}


// USER_TESTOBJ_H
#endif
