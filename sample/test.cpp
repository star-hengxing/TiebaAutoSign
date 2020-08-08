#include"Headers.hpp"

using namespace std;

int main()
{
	TiebaAutoSign x;
	if (!x.GetTiebaMsg())
	{
		cout << "fail!" << endl;
	}
	else
	{
		if (!x.sign_tieba())
		{
			cout << "贴吧签到失败" << endl;
		}
	}
	system("pause");
	return 0;
}
