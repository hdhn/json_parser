#include<string>
#include<fstream>
#include<iostream> 
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

int main()
{
    string book;
    ifstream in("qqzl.json", ios::binary);
 
	if (!in.is_open())
	{
		cout << "Error opening file\n";
		return 0;
	}
    else{
        in >> book;
        // cout << book;
        cout << formatJson(book);
    }
    return 0;
}