#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "User.hpp"




class GroupUser : public User
{
public:
 void setRole(string role) { this->role = role; }
 string getRole() { return this->role; }


private:
  string role;
};




#endif