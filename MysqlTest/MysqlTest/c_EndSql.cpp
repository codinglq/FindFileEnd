#include "OperateMysql.h"

void c_EndSql_insert(TableType v_Head,char *c_Sql_insert){
	//sprintf(c_Sql_insert,"insert into score_info (College_name,Student_id,School_year,Tream,Class_name,Class_type,Teat_score,Credit,Student_name) values(\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,\'%s\' )",
	/*1 char college_name ,2 int Student_id,3 char School_year,4 int Tream,5 char Class_name,6 char Class_type,7 char Test_score,8 float Credit,9 int Db_id,10 char Student_name */
	sprintf(c_Sql_insert,"insert into score_info values(\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%d,\'%s\' )",
		v_Head.College_name.c_str(),
		v_Head.Student_id.c_str(),
		v_Head.Schaool_year.c_str(),
		v_Head.Tream.c_str(),
		v_Head.Class_name.c_str(),
		v_Head.Class_type.c_str(),
		v_Head.Test_score.c_str(),
		v_Head.Credit.c_str(),
		/*v_Head.Db_id*/NULL,
		v_Head.Student_name.c_str());
}