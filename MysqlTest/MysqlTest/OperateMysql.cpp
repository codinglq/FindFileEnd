#include "OperateMysql.h"
/************************************************************************/
/* 顾名思义这个类是操作数据库的哦                                         */
/************************************************************************/

COperateMysql::COperateMysql(void)
{
		res=NULL;
}
COperateMysql::~COperateMysql(void)
{
}
int COperateMysql::MakeConnect(char * c_Name,char *c_Passwd,char *c_Host,char *c_dbName){

	con=mysql_init((MYSQL*)0);
	if (con!=NULL&&mysql_real_connect(con,c_Host,c_Name,c_Passwd,c_dbName,3306,NULL,0))
	{	
		if (!mysql_select_db(con,c_dbName))
		{
			cout<<"数据库选择成功"<<endl;
			con->reconnect=1;
			c_Query="set names GBK";
			i_ReturnValue=mysql_real_query(con,c_Query,strlen(c_Query));
			if (i_ReturnValue)
			{
				cout<<"编码设置失败"<<mysql_error(con)<<endl;
			}
			else{
				cout<<"编码设置成功"<<endl;
			}
		}
	}
	else{
		cout<<"无法打开数据库"<<endl;
	}
	return 0;
}

void COperateMysql::CloseConnect(){

	mysql_free_result(res);
	mysql_close(con);
	cout<<"数据库已经关闭"<<endl;
}

int COperateMysql::Query(char * c_SqlQuery,int i_flg){

	i_ReturnValue=mysql_real_query(con,c_SqlQuery,strlen(c_SqlQuery));
	if (i_ReturnValue)
	{
			cout<<"sql语句执行失败"<<mysql_error(con)<<endl;
			return 0;
	}
	else{
		//cout<<"sql语句执行成功："<<c_SqlQuery<<endl;
	}
	if (i_flg)
	{
		res=mysql_store_result(con);
		if (res==0)
		{
				return 0;
		}
		row=mysql_fetch_row(res);
		while(row=mysql_fetch_row(res)){
			for (t=0;t<mysql_num_fields(res);t++)
			{	
				cout<<row[t]<<" ";
			}
			cout<<endl;
		}
	}
}