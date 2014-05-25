#ifndef OPERATE_MYAQL_H
#define OPERATE_MYAQL_H

#include <Windows.h>
#include <iostream>
#include "mysql.h"
#include <string>
#include <vector>
#include <WinSock.h>

using namespace std;
typedef struct TableType{

	string Schaool_year;//学年
	string Tream;//学期
	string Student_name;//学生姓名
	string Class_name;//课程名称
	string College_name;//学院名称
	string Class_type;//课程类型

	int Db_id;//数据库编号-----------------这个文件中提取不出来的-------
	string Student_id;//学生学号
	string Test_score;//考试成绩
	string Credit;//学分
};
// typedef struct TableQueue{
// 
// 		TableQueue* p_Head;
// 		int i_QueueSize;
// };
int ReadFileToBuf(string &s_buf,const char *c_FileName);
void GetTableQueue(vector<TableType> &T_Haed,string s_buf,string s_StudentId,int &i_DbId);
void  c_EndSql_insert(TableType v_Head,char *c_sql_temp);//返回sql语句
#endif
class COperateMysql
{
private:
	MYSQL *con;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char tmp[400];
	int i_ReturnValue;
	char *c_Query;
	int t;
public:
	COperateMysql(void);
	~COperateMysql(void);
	int MakeConnect(char * c_Name,char *c_Passwd,char *c_Host,char *c_dbName);//创建数据库连接 
	int Query(char * c_SqlQuery,int i_flg);//输入sql语言进行查询
	void CloseConnect();
};
/************************************************************************/
/* #include <Windows.h>
#include <iostream>
#include "mysql.h"
#include <WinSock.h>
using namespace std;                                                           */
/************************************************************************/

/****
MYSQL *con;
MYSQL_RES *res;
MYSQL_ROW row;

char tmp[400];

//配置mysql
char dbuser[30]="root";
char dbpasswd[30]="asd8956656";
char dbip[30]="localhost";
char dbname[30]="my_school";
char tablename[30]="score_info";
char *query=NULL;

int x;
int y;
int rt;//返回值
unsigned int t;
int count=0;

// 	cout<<"输入x,y"<<endl;
// 	cin>>x;
// 	cin>>y;
// 	cout<<"输入结束"<<endl;
con=mysql_init((MYSQL*)0);
if (con!=NULL&&mysql_real_connect(con,dbip,dbuser,dbpasswd,dbname,3306,NULL,0))
{
if (!mysql_select_db(con,dbname))
{
cout<<"数据库选择成功"<<endl;
con->reconnect=1;
query="set names GBK";
rt=mysql_real_query(con,query,strlen(query));
if (rt)
{
cout<<"出错了query"<<mysql_error(con)<<endl;
}
else{
cout<<"query 成功"<<endl;
}
}
}
else{
cout<<"无法打开数据库"<<endl;
}
sprintf(tmp,"select * from %s",tablename);
rt=mysql_real_query(con,tmp,strlen(tmp));
if (rt)
{
cout<<"query出错"<<endl;
}
else{
cout<<"query成功"<<endl;
}
res=mysql_store_result(con);

while(row=mysql_fetch_row(res)){


for (t=0;t<mysql_num_fields(res);t++)
{	
cout<<row[t]<<" ";
}
cout<<endl;
count++;
}
cout<<"行数:"<<count<<endl;
cout<<"mysql_free_result"<<endl;
mysql_free_result(res);
mysql_close(con);
*****/


