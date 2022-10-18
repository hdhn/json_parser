#include<string>
#include<fstream>
#include<iostream> 
#include<bits/stdc++.h>
using namespace std;
#define INT 0
#define DOU 1
#define STR 2
#define BOOL 3
#define LIST 4
#define DICT 5
using namespace std;

string formatJson(string json)
{
	string result = "";
    string getLevelStr(int level);
	int level = 0;
	for (string::size_type index = 0; index < json.size(); index++)
	{
		char c = json[index];

		if (level > 0 && '\n' == json[json.size() - 1])
		{
			result += getLevelStr(level);
		}

		switch (c)
		{
		case '{':
		case '[':
			result = result + c + "\n";
			level++;
			result += getLevelStr(level);
			break;
		case ',':
			result = result + c + "\n";
			result += getLevelStr(level);
			break;
		case '}':
		case ']':
			result += "\n";
			level--;
			result += getLevelStr(level);
			result += c;
			break;
		default:
			result += c;
			break;
		}

	}
	return result;
}

string getLevelStr(int level)
{
	string levelStr = "";
	for (int i = 0; i < level; i++)
	{
		levelStr += "\t"; //这里可以\t换成你所需要缩进的空格数
	}
	return levelStr;

}
class Val {
public:
	int INT_VAL;
	double DOU_VAL;
	string STR_VAL;
	bool BOOL_VAL;
	vector<Val>List;
	map<Val, Val>dict;

	int type;

	void del() {
		if (type == STR)STR_VAL.clear();
		if (type == LIST)List.clear();
		if (type == DICT)dict.clear();
	}
	Val() {}
	~Val() {
		del();
	}
	Val(const int &x) {
		del();
		INT_VAL = x;
		type = INT;
	}

	Val(const double & x) {
		del();
		DOU_VAL = x;
		type = DOU;
	}

	Val(const string & x) {
		del();
		STR_VAL = x;
		type = STR;
	}

	Val(const char *x) {
		del();
		type = STR;
		STR_VAL = string(x);
	}

	Val(const bool & x) {
		del();
		BOOL_VAL = x;
		type = BOOL;
	}

	Val(initializer_list<Val> l) {
		del();
		type = LIST;
		for (Val x : l)List.push_back(x);
	}

	void add(Val x) {
		if (type == LIST)List.push_back(x);
	}

	void put(Val key, Val val) {
		if (type == DICT)dict[key] = val;
	}

	Val& operator [](Val i) {
		if (type == LIST)return List[i.INT_VAL];
		else return dict[i];
	}

	string str();
};



ostream& operator << (ostream & out, const Val & v) {
	if (v.type == INT)out << v.INT_VAL;
	if (v.type == DOU)out << v.DOU_VAL;
	if (v.type == STR)out << "\"" << v.STR_VAL << "\"";
	if (v.type == BOOL) {
		if (v.BOOL_VAL)out << "true";
		else out << "false";
	}
	if (v.type == LIST) {
		out << "[";
		for (int i = 0; i < v.List.size(); i++) {
			if (i)out << ",";
			out << v.List[i];
		}
		out << "]";
	}
	if (v.type == DICT) {
		out << "{";
		for (auto it = v.dict.begin(); it != v.dict.end(); it++) {
			if (it != v.dict.begin())out << ",";
			out << it->first << ":" << it->second;
		}
		out << "}";
	}
	return out;
}

string Val::str() {
	stringstream ss; ss << (*this);
	return ss.str();
}

bool operator < (const Val &a, const Val &b) {
	if (a.type != b.type)return a.type < b.type;
	else {
		if (a.type == INT)return a.INT_VAL < b.INT_VAL;
		if (a.type == DOU)return a.DOU_VAL < b.DOU_VAL;
		if (a.type == STR)return a.STR_VAL < b.STR_VAL;
		if (a.type == LIST)return a.List < b.List;
		if (a.type == DICT)return a.dict < b.dict;
		return true;
	}
}

stringstream ss;
Val parser_val(); 
Val parser_num(); //解析数字
Val parser_str(); // 解析字符
Val parser_bool(); //解析布尔
Val parser_arr();
Val parser_map();

Val parser_num() {
	string s;
	while (isdigit(ss.peek()) || ss.peek() == 'e' || ss.peek() == '-' || ss.peek() == '+')s.push_back(ss.get());
	if (count(s.begin(), s.end(), '.') || count(s.begin(), s.end(), 'e')) {
		return stof(s);
	}
	else {
		return stoi(s);
	}
}

Val parser_str() {
	ss.get();
	string s;
	while (ss.peek() != '"')s.push_back(ss.get());
	ss.get();
	return Val(s);
}

Val parser_bool() {
	if (ss.peek() == 'f') {
		ss.get(); ss.get(); ss.get(); ss.get(); ss.get();
		return Val(false);
	}
	else {
		ss.get(); ss.get(); ss.get(); ss.get();
		return Val(true);
	}
}
Val parser_arr() {
	ss.get();//吃掉  '['
	Val vec;vec.type = LIST;
	while (ss.peek() != ']') {
		vec.add(parser_val());
		while (ss.peek() != ']' && (ss.peek() == ' ' || ss.peek() == '\t' || ss.peek() == '\n' || ss.peek() == ','))ss.get();
	}
	ss.get();
	return vec;
}

Val parser_map() {
	ss.get();
	Val dict; dict.type = DICT;
	while (ss.peek() != '}') {
		Val key = parser_val();
		while (ss.peek() == ' ' || ss.peek() == ':')ss.get();
		Val val = parser_val();
		dict.put(key, val);
		while (ss.peek() != '}' && (ss.peek() == ' ' || ss.peek() == '\t' || ss.peek() == '\n' || ss.peek() == ','))ss.get();
	}
	ss.get();
	return dict;
}
Val parser_val() {
	while (ss.peek() != -1) {
		if (ss.peek() == ' ' || ss.peek() == '\n' || ss.peek() == '\t')ss.get();
		else if (ss.peek() == '"') {
			return parser_str();
		}
		else if (ss.peek() == 'f' || ss.peek() == 't') {
			return parser_bool();
		}
		else if (ss.peek() == '[') {
			return  parser_arr();
		}
		else if (ss.peek() == '{') {
			return parser_map();
		}
		else {
			return parser_num();
		}
	}
	return 0;
}

Val parser(string s) {
	ss.clear();
	ss << s;
	return parser_val();
}

int main()
{
	ifstream fin("qqzl.json");
	if (!fin.is_open())
	{
		cout << "Error opening file\n";
		return 0;
	}
	string ss;
	fin >>ss;
	// stringstream ss; ss << fin.rdbuf();
	cout << formatJson(ss);
	Val x = parser(ss);
	cout << x["members"] << endl;
	return 0;
}