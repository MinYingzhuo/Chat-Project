#include "friendmodel.hpp"
#include "db.h"

  // 添加好友关系
bool FriendModel::insert(int userid, int friendid)
{
  //1.组装sql语句
  char sql[1024] = {0};
  sprintf(sql,"insert into Friend values('%d', '%d');",userid,friendid);

    MySQL mysql;
    if(mysql.connect()) {
      if(mysql.update(sql)) {

        return true;
      }
    }
  return false;
}

  //返回用户的好友列表
vector<User> FriendModel::query(int userid)
{
  //1.组装sql语句
  char sql[1024] = {0};
  sprintf(sql,"select User.id,User.name,User.state from User inner join Friend on Friend.friendid = User.id where Friend.userid = %d;",userid);
  vector<User> vec;
    MySQL mysql;
    if(mysql.connect()) {
      MYSQL_RES *res = mysql.query(sql);
      if(res != NULL) {
        //将userid用户的所有离线消息放入vec中返回
        MYSQL_ROW row ;
        while((row = mysql_fetch_row(res)) != nullptr)
        {
          User user;
          user.setId(atoi(row[0]));
          user.setName(row[1]);
          user.setState(row[2]);
          vec.push_back(user);
        }
        mysql_free_result(res);
        return vec;       
      }
    }
  return vec;
}


