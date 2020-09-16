// test_mysql.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <fstream>
#include <io.h>
#include <vector>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <vector>
#include <string>
#include <Python.h>
#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/mysql_error.h"
#include "jdbc/cppconn/config.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/resultset.h"
#include "jdbc/cppconn/sqlstring.h"
#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))

using namespace std;
#define DBHOST "192.168.100.121"
#define DBUSER "pc"
#define DBPASS ""

int pow_1 = 0;
double a_1 = 0;
double b_1 = 0;
double c_1 = 0;
double d_1 = 0;
double e_1 = 0;
int pow_2 = 0;
double a_2 = 0;
double b_2 = 0;
double c_2 = 0;
double d_2 = 0;
double e_2 = 0;
int pow_3 = 0;
double a_3 = 0;
double b_3 = 0;
double c_3 = 0;
double d_3 = 0;
double e_3 = 0;
int pow_4 = 0;
double a_4 = 0;
double b_4 = 0;
double c_4 = 0;
double d_4 = 0;
double e_4 = 0;
int pow_5 = 0;
double a_5 = 0;
double b_5 = 0;
double c_5 = 0;
double d_5 = 0;
double e_5 = 0;


std::vector<std::string> splint(std::string str, char del) {
	int first = 0;
	int last = str.find_first_of(del);

	std::vector<std::string> result;

	while (first < str.size()) {
		std::string subStr(str, first, last - first);

		result.push_back(subStr);

		first = last + 1;
		last = str.find_first_of(del, first);

		if (last == std::string::npos) {
			last = str.size();
		}
	}

	return result;
}

void start_device() {
	//system("python C:/Users/buffa/source/repos/start_device/start_device/start_device.py");
	system("python start_device.py");
}

void get_locdata(sql::SQLString DBNAME, sql::SQLString tablename, const char* filename) {
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	unique_ptr<sql::Connection> con(driver->connect(DBHOST, DBUSER, DBPASS));
	unique_ptr<sql::Statement> stmt(con->createStatement());

	sql::SQLString usecommand = "USE ";
	usecommand += DBNAME;

	sql::SQLString selectcommand = "SELECT * FROM  ";
	selectcommand += tablename;
	selectcommand += " ORDER BY id DESC LIMIT 10;";

	stmt->execute(usecommand);
	unique_ptr<sql::ResultSet> res(stmt->executeQuery(selectcommand));

	std::ofstream ofs(filename);

	//ゾーンの範囲
	int max_x = 0;
	int min_x = 0;
	int max_y = 0;
	int min_y = 0;

	//消費電力
	int pow = 0;
	if (filename == "loc_data_1.txt") 
	{
		pow = pow_1;
		max_x = 4000;
		min_x = 2900;
		max_y = 3600;
		min_y = 2600;
	}
	else if (filename == "loc_data_2.txt")
	{
		pow = pow_2;
		max_x = 5400;
		min_x = 4300;
		max_y = 3600;
		min_y = 2600;
	}
	else if (filename == "loc_data_2.txt")
	{
		pow = pow_3;
		max_x = 4000;
		min_x = 2900;
		max_y = 2300;
		min_y = 1300;
	}
	else if (filename == "loc_data_4.txt")
	{
		pow = pow_4;
		max_x = 5400;
		min_x = 4300;
		max_y = 2300;
		min_y = 1300;
	}
	else if (filename == "loc_data_5.txt") 
	{
		pow = pow_5;
		max_x = 4000;
		min_x = 2900;
		max_y = 1000;
		min_y = 0;
	}

	//各状態
	int loc_a = 0;
	int loc_b = 0;
	int loc_c = 0;


	while (res->next())
	{
		//ファイルに書き込み
		ofs << res->getInt(1) << ",";
		ofs << res->getInt(2) << ",";
		ofs << res->getDouble(3) << ",";
		ofs << res->getDouble(4) << ",";
		ofs << res->getDouble(5) << ",";
		ofs << res->getString(6).c_str() << endl;


		int id = res->getInt(1);
		int device_id = res->getInt(2);
		double x = res->getDouble(3);
		double y = res->getDouble(4);
		double z = res->getDouble(5);
		string date = res->getString(6).c_str();

		printf("x:%lf, Y:%lf\n", x, y);

		//位置情報評価
		//
		//室外にいるか？
		if ((x < 0) || (y < 0) ) 
		{
			///室外
			loc_c += 1;
		}
		else 
		{
			///室内
			/// 
			//ゾーン内にいるか
			if ((min_x < x  && x< max_x) && (min_y < y && y < max_y)) 
			{
				//ゾーン内
				loc_a += 1;
			}
			else 
			{
				//ゾーン外
				loc_b += 1;
			}
		}
	}

	//最終的な評価
	int max;
	max = loc_a;
	if (max < loc_b) 
	{
		max = loc_b;
	}
	if (max < loc_c) 
	{
		max = loc_c;
	}

	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0;
	double e = 0;
	if (max == loc_a) 
	{
		//室内・ゾーン内
		a += 1;
	}
	else if(max == loc_b)
	{
		//室内・ゾーン外
		//
		//機器の稼働
		if (400 < pow)
		{
			b += 1;
		}
		else 
		{
			c += 1;
		}
	} 
	else if(max == loc_c)
	{
		//室外
		//
		//機器の稼働
		if (400 < pow) 
		{
			d += 1;
		}
		else
		{
			e += 1;
		}
	}


	if (filename == "loc_data_1.txt") 
	{
		a_1 += a;
		b_1 += b;
		c_1 += c;
		d_1 += d;
		e_1 += e;
	}
	else if (filename == "loc_data_2.txt") 
	{
		a_2 += a;
		b_2 += b;
		c_2 += c;
		d_2 += d;
		e_2 += e;
	}
	else if (filename == "loc_data_3.txt") 
	{
		a_3 += a;
		b_3 += b;
		c_3 += c;
		d_3 += d;
		e_3 += e;
	}
	else if (filename == "loc_data_4.txt") 
	{
		a_4 += a;
		b_4 += b;
		c_4 += c;
		d_4 += d;
		e_4 += e;
	}
	else if (filename == "loc_data_5.txt") 
	{
		a_5 += a;
		b_5 += b;
		c_5 += c;
		d_5 += d;
		e_5 += e;
	}

}

void get_pow(sql::SQLString DBNAME, sql::SQLString tablename, const char* filename)
{
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	unique_ptr<sql::Connection> con(driver->connect(DBHOST, DBUSER, DBPASS));
	unique_ptr<sql::Statement> stmt(con->createStatement());

	sql::SQLString usecommand = "USE ";
	usecommand += DBNAME;
	stmt->execute(usecommand);
	std::ofstream ofs(filename);

	int pow_5_1 = 0;

	for (int i = 1; i < 18; i++) 
	{
		std::string No = std::to_string(i);
		sql::SQLString plug_No = sql::SQLString(No.c_str());

		sql::SQLString selectcommand = "SELECT id, device_id, watt FROM  ";
		selectcommand += tablename;
		selectcommand += " WHERE device_id = ";
		selectcommand += plug_No;
		selectcommand += "  ORDER BY id DESC LIMIT 1;";
		unique_ptr<sql::ResultSet> res(stmt->executeQuery(selectcommand));

		//ファイルに書き込み
		while (res->next()) {
			ofs << res->getInt(1) << ",";
			ofs << res->getInt(2) << ",";
			ofs << res->getInt(3) << endl;

			int id = res->getInt(1);
			int device_id = res->getInt(2);
			int watt = res->getInt(3);


			//①の消費電を格納
			if (device_id == 13) 
			{
				pow_1 = watt;
				printf("watt①:%d\n", pow_1);
			}
			//②の消費電を格納
			if (device_id == 8) 
			{
				pow_2 = watt;
				printf("watt②:%d\n", pow_2);
			}
			//③の消費電を格納
			if (device_id == 15) 
			{
				pow_3 = watt;
				printf("watt③:%d\n", pow_3);
			}
			//④の消費電を格納
			if (device_id == 14) 
			{
				pow_4 = watt;
				printf("watt④:%d\n", pow_4);
			}
			
			//⑤の消費電を格納
			if ((device_id == 1)||(device_id == 2)||(device_id == 3)) 
			{
				pow_5_1 += watt;
			}
		}
	}
	//⑤の消費電を格納
	pow_5 = pow_5_1;
	printf("watt⑤:%d\n", pow_5);
}


void read_file(const char* filename, int num_data) {
	char file_data[100];
	std::ifstream ifs(filename);
	std::string *datas;
	datas = new std::string[num_data];
//	std::vector<string> datas(num_data);
	int line = 0;
	while (ifs.getline(file_data, 100)) {
		datas[line] = file_data;
		line += 1;
	}


	//逆順に並べ替え
	std::reverse(datas, datas + SIZE_OF_ARRAY(datas));

	for (int i = 0; i < num_data; i++) {
		std::string str = datas[i];
		char del = ',';

		//文字列を分解
		for (const auto subStr : splint(str, del)) {
			std::cout << subStr << endl;
		}

	}
}


string evaluation_pow(int id) {
	string evaluation;

	double probability_a = 0;
	double probability_b = 0;
	double probability_c = 0;
	double probability_d = 0;
	double probability_e = 0;

	if(id == 1)
	{
        //存在確率
		probability_a = a_1 / 1800;
		probability_b = b_1 / 1800;
		probability_c = c_1 / 1800;
		probability_d = d_1 / 1800;
		probability_e = e_1 / 1800;

		printf("probability_a:%lf probability_b:%lf, probability_c;%lf probability_d:%lf probability_e:%lf\n", probability_a, probability_b, probability_c, probability_d, probability_e);

		//リセット
		a_1 = 0;
		b_1 = 0;
		c_1 = 0;
		d_1 = 0;
		e_1 = 0;
	}
	else if (id == 2) 
	{
		//存在確率
		probability_a = a_2 / 1800;
		probability_b = b_2 / 1800;
		probability_c = c_2 / 1800;
		probability_d = d_2 / 1800;
		probability_e = e_2 / 1800;

		printf("probability_a:%lf probability_b:%lf, probability_c;%lf probability_d:%lf probability_e:%lf\n", probability_a, probability_b, probability_c, probability_d, probability_e);

		//リセット
		a_2 = 0;
		b_2 = 0;
		c_2 = 0;
		d_2 = 0;
		e_2 = 0;
	}
	else if (id == 3) 
	{
		//存在確率
		probability_a = a_3 / 1800;
		probability_b = b_3 / 1800;
		probability_c = c_3 / 1800;
		probability_d = d_3 / 1800;
		probability_e = e_3 / 1800;

		printf("probability_a:%lf probability_b:%lf, probability_c;%lf probability_d:%lf probability_e:%lf\n", probability_a, probability_b, probability_c, probability_d, probability_e);

		//リセット
		a_3 = 0;
		b_3 = 0;
		c_3 = 0;
		d_3 = 0;
		e_3 = 0;
	}
	else if (id == 4) 
	{
		//存在確率
		probability_a = a_4 / 1800;
		probability_b = b_4 / 1800;
		probability_c = c_4 / 1800;
		probability_d = d_4 / 1800;
		probability_e = e_4 / 1800;

		printf("probability_a:%lf probability_b:%lf, probability_c;%lf probability_d:%lf probability_e:%lf\n", probability_a, probability_b, probability_c, probability_d, probability_e);

		//リセット
		a_4 = 0;
		b_4 = 0;
		c_4 = 0;
		d_4 = 0;
		e_4 = 0;
	}
	else if (id == 5) 
	{
		//存在確率
		probability_a = a_5 / 1800;
		probability_b = b_5 / 1800;
		probability_c = c_5 / 1800;
		probability_d = d_5 / 1800;
		probability_e = e_5 / 1800;

		printf("probability_a:%lf probability_b:%lf, probability_c;%lf probability_d:%lf probability_e:%lf\n", probability_a, probability_b, probability_c, probability_d, probability_e);

		//リセット
		a_5 = 0;
		b_5 = 0;
		c_5 = 0;
		d_5 = 0;
		e_5 = 0;
	}

	double probability_max = probability_a;

	printf("a\n");
	if (probability_max < probability_b) 
	{
		probability_max = probability_b;
		printf("b\n");
	}
	if (probability_max < probability_c)
	{
		probability_max = probability_c;
		printf("c\n");
	}
	if (probability_max < probability_d)
	{
		probability_max = probability_d;
		printf("d\n");
	}
	if (probability_max < probability_e)
	{
		probability_max = probability_e;
		printf("e\n");
	}

	//結果
	if ((probability_max == probability_b) || (probability_max == probability_d))
	{
		printf("%dのPCは止めてよい\n", id);
		std::string str_id = to_string(id);
		evaluation = str_id + "のPCは止めてよい";
	}
	else {
		printf("%dのPCは止めるべきではない\n", id);
		std::string str_id = to_string(id);
		evaluation = str_id + "のPCは止めるべきではない";
	}
	return evaluation;
}

struct Foo {
	void operator()() {
		//printf("別スレッドで実行されています\n");
		//system("python C:/Users/buffa/source/repos/open_window/open_window/open_window.py");
		system("python open_window.py");
	}
};




int main() 
{

	start_device();

	const char* filename = "int_test.txt";

	const char* filename_loc1 = "loc_data_1.txt";
	const char* filename_loc2 = "loc_data_2.txt";
	const char* filename_loc3 = "loc_data_3.txt";
	const char* filename_loc4 = "loc_data_4.txt";
	const char* filename_loc5 = "loc_data_5.txt";
	const char* filename_pow = "pow_data.txt";

	sql::SQLString DBNAME_loc = "uwb_db";
	sql::SQLString DBNAME_pow = "test";


	sql::SQLString tablename_loc1 = "tag1_result_location";
	sql::SQLString tablename_loc2 = "tag2_result_location";
	sql::SQLString tablename_loc3 = "tag3_result_location";
	sql::SQLString tablename_loc4 = "tag4_result_location";
	sql::SQLString tablename_loc5 = "tag5_result_location";

	sql::SQLString tablename_pow = "powerconsumption_table";

	int num_loc_data = 10;
	int num_pow_data = 17;

	int time_count = 0;

	while (true) 
	{

		get_pow(DBNAME_pow, tablename_pow, filename_pow);

		//get_locdata(DBNAME_loc, tablename_loc1, filename_loc1);
		//get_locdata(DBNAME_loc, tablename_loc2, filename_loc2);
		//get_locdata(DBNAME_loc, tablename_loc3, filename_loc3);
		get_locdata(DBNAME_loc, tablename_loc4, filename_loc4);
		//get_locdata(DBNAME_loc, tablename_loc5, filename_loc5);
		//get_pow(DBNAME_pow, tablename_pow, filename_pow);

		//read_file(filename, num_pow_data);
		//read_file(filename, num_loc_data);

		time_count += 1;


		printf("%d\n", time_count);

		if (time_count == 30) {
			const char* filename = "evalution.txt";
			std::ofstream ofs(filename);
			string evatution;
			//消費電力評価
		//	evatution = evaluation_pow(1);
		//	ofs << evatution.c_str() << endl;
			//evatution = evaluation_pow(2);
			//ofs << evatution.c_str() << endl;
			//evatution = evaluation_pow(3);
			//ofs << evatution.c_str() << endl;
			evatution = evaluation_pow(4);
			ofs << evatution.c_str() << endl;
			//evatution = evaluation_pow(5);
			//ofs << evatution.c_str() << endl;
			time_count = 0;
			Foo foo;
			thread th(foo);
			th.detach();
		}
		
		
		Sleep(2000);
	}
}











void test()
{
	sql::SQLString tablename = "six";

	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	unique_ptr<sql::Connection> con(driver->connect(DBHOST, DBUSER, DBPASS));
	unique_ptr<sql::Statement> stmt(con->createStatement());

	sql::SQLString usecommand = "USE ";
//	usecommand += DBNAME;

	sql::SQLString confirmationcommand = "DROP TABLE IF EXISTS ";
	confirmationcommand += tablename;

	sql::SQLString createcommand = "CREATE TABLE ";
	createcommand += tablename;
	createcommand += "(cid int, name varchar(10))";

	sql::SQLString selectcommand = "SELECT * FROM  ";
	selectcommand += tablename;
	selectcommand += " ORDER BY id DESC LIMIT 10;";


	stmt->execute(usecommand);
//	stmt->execute(confirmationcommand);
//	stmt->execute(createcommand);
//	cout << "\"conference\" table has been created." << endl;

	sql::SQLString insert = "INSERT INTO  ";
	insert += tablename;
	insert += " VALUES(2, 'ASDF')";
//    stmt->execute(insert);

	unique_ptr<sql::ResultSet> res(stmt->executeQuery(selectcommand));
	size_t row = 0;

	const char* filename = "data.txt";
	std::ofstream ofs(filename);
	std::ifstream ifs(filename);
	
	while (res->next())
	{
		

		//ファイルに書き込み
		ofs << res->getInt(1) << ", ";
		ofs << res->getInt(2) << ", ";
		ofs << res->getInt(3) << ", ";
		ofs << res->getInt(4) << ", ";
		ofs << res->getInt(5) << ", ";
		ofs << res->getString(6).c_str() << endl;



		int id = res->getInt(1);
		int device_id = res->getInt(2);
		int x = res->getDouble(3);
		int y = res->getDouble(4);
		int z = res->getDouble(5);
		string date = res->getString(6).c_str();
	}



	//ファイルの読み込み
	char file_data[100];
	std::string datas[10];
	int line = 0;
	while (ifs.getline(file_data, 100)) {
		//std::cout << filedata << std::endl;
		datas[line] = file_data;
		line += 1;
	}
	//逆順に並べ替え
	std::reverse(datas, datas + SIZE_OF_ARRAY(datas));
	for (int i = 0; i < 10; i++) {
	//	std::cout << datas[i] << std::endl;
		std::string str = datas[i];
		char del = ',';

		//文字列を分解
		for (const auto subStr : splint(str, del)) {
			std::cout << subStr << endl;
		}

	}

	

}


