#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class StateMachine1
{
protected:
	string str;

public:
	virtual void start(string s) = 0;

	virtual ~StateMachine1() {};

	void start_from_file(const char* filename_)
	{
		ifstream in(filename_);
		string str;
		if (!in)
		{
			cout << "Can't open file" << filename_ << endl;
			return;
		}

		while (!in.eof())
		{
			getline(in, str);
			cout << str << endl;
			start(str);
		}

		in.close();
	}
};

class Avto1 : public StateMachine1
{
	void(Avto1::* q)(char ch);
	int position;
public:
	Avto1()
	{
		q = &Avto1::q0;
	}

	void start(string s)
	{
		str = s;
		position = 0;
		q = &Avto1::q0;

		try
		{
			while (position < str.size())
			{
				(this->*q)(str[position]);
				cout << str[position] << endl;
				++position;
			}
		}
		catch(int e)
		{
			cout << "Bad" << endl;
			return;
		}

		if (q == &Avto1::q1)
			cout << "Good" << endl;
		else
			cout << "Bad" << endl;
	}

	void q0(char ch)
	{
		cout << "q0 is work" << endl;
		switch (ch)
		{
		case '0': 
			q = &Avto1::q1;
			break;
		case '1':
			q = &Avto1::q2;
			break;
		default:
			Error();
		}
	}
	void q1(char ch)
	{
		cout << "q1 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto1::q0;
			if (str[position + 1] == '1')
			{
				Error3();
			}
			else if (str[position + 1] != '0')
				Error1();
			break;
		case '1':
			Error3();
			break;
		default:
		{
			Error();
		}
		}
	}
	void q2(char ch)
	{
		cout << "q2 is work" << endl;
		switch (ch)
		{
		case '0':
			Error2();
			break;
		case '1':
			q = &Avto1::q3;
			break;
		default:
			Error();
		}
	}
	void q3(char ch)
	{
		cout << "q3 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto1::q1;
			break;
		case '1':
			q = &Avto1::q2;
			break;
		default:
			Error();
		}
	}

	void Error1()
	{
		cout << "error: 0 - even" << endl;
		throw 1;
	}
	void Error2()
	{
		cout << "error: 1 - odd" << endl;
		throw 1;
	}
	void Error3()
	{
		cout << "error: after 0 there 1" << endl;
		throw 1;
	}
	void Error()
	{
		cout << "error: invalid symbol" << endl;
		throw 1;
	}
};

class Avto2 : public StateMachine1
{
	void(Avto2::* q)(char ch);
	int position;

public:
	Avto2()
	{
		q = &Avto2::q0;
	}

	void start(string s)
	{
		str = s;
		position = 0;
		q = &Avto2::q0;

		try
		{
			while (position < str.size())
			{
				(this->*q)(str[position]);
				cout << str[position] << endl;
				++position;
			}
		}
		catch (int e)
		{
			cout << "Bad" << endl;
			return;
		}

		if (q == &Avto2::q1)
			cout << "Good" << endl;
		else
			cout << "Bad" << endl;
	}

	void q0(char ch)
	{
		cout << "q0 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto2::q0;;
			break;
		case '1':
			q = &Avto2::q1;
			break;
		default:
			Error();
		}
	}

	void q1(char ch)
	{
		cout << "q1 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto2::q2;
			if (str[position + 1] != '0' && str[position + 1] != '1')
			{
				Error2();
			}
			break;
		case '1':
			q = &Avto2::q1;
			break;
		default:
			Error();
		}
	}
	void q2(char ch)
	{
		cout << "q2 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto2::q1;
			if (str[position + 1] != '1')
			{
				Error2();
			}
			break;
		case '1':
			Error1();
			break;
		default:
			Error();
		}
	}

	void Error1()
	{
		cout << "error: odd number of '0' between '1's" << endl;
		throw 1;
	}

	void Error2()
	{
		cout << "error: '0' not between '1's" << endl;
		throw 1;
	}

	void Error()
	{
		cout << "error: invalid symbol" << endl;
		throw 1;
	}
};

class Avto3 : public StateMachine1
{
	void(Avto3::* q)(char ch);
	int position;

public:
	Avto3()
	{
		q = &Avto3::q0;
	}

	void start(string s)
	{
		str = s;
		position = 0;
		q = &Avto3::q0;

		try
		{
			while (position < str.size())
			{
				(this->*q)(str[position]);
				cout << str[position] << endl;
				++position;
			}
		}
		catch (int e)
		{
			cout << "Bad" << endl;
			return;
		}

		if (q == &Avto3::q1 || q == &Avto3::q0)
			cout << "Good" << endl;
		else
			cout << "Bad" << endl;
	}

	void q0(char ch)
	{
		cout << "q0 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto3::q0;
			break;
		case '1':
			q = &Avto3::q1;
			break;
		default:
			Error();
		}
	}

	void q1(char ch)
	{
		cout << "q1 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto3::q0;
			break;
		case '1':
			q = &Avto3::q2;
			if (str[position + 1] != '0' && str[position + 1] != '1')
			{
				Error1();
			}
			break;
		default:
			Error();
		}
	}
	void q2(char ch)
	{
		cout << "q2 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto3::q0;
			break;
		case '1':
			Error1();
			break;
		default:
			Error();
		}
	}

	void Error1()
	{
		cout << "error: after a pair of '1's there is no '0'" << endl;
		throw 1;
	}

	void Error()
	{
		cout << "error: invalid symbol" << endl;
		throw 1;
	}
};

class Avto4 : public StateMachine1
{
	void(Avto4::* q)(char ch);
	int position;

public:
	Avto4()
	{
		q = &Avto4::q0;
	}

	void start(string s)
	{
		str = s;
		position = 0;
		q = &Avto4::q0;

		try
		{
			while (position < str.size())
			{
				(this->*q)(str[position]);
				cout << str[position] << endl;
				++position;
			}
		}
		catch (int e)
		{
			cout << "Bad" << endl;
			return;
		}

		if (q == &Avto4::q1 || q == &Avto4::q0 || q == &Avto4::q2)
			cout << "Good" << endl;
		else
			cout << "Bad" << endl;
	}

	void q0(char ch)
	{
		cout << "q0 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto4::q1;
			break;
		case '1':
			q = &Avto4::q1;
			break;
		default:
			Error();
		}
	}

	void q1(char ch)
	{
		cout << "q0 is work" << endl;
		switch (ch)
		{
		case '0':
			q = &Avto4::q2;
			break;
		case '1':
			q = &Avto4::q2;
			break;
		default:
			Error();
		}
	}

	void q2(char ch)
	{
		cout << "q0 is work" << endl;
		switch (ch)
		{
		case '0':
			Error1();
			break;
		case '1':
			q = &Avto4::q0;
			break;
		default:
			Error();
		}
	}

	void Error1()
	{
		cout << "error: not every third character is '1'" << endl;
		throw 1;
	}

	void Error()
	{
		cout << "error: invalid symbol" << endl;
		throw 1;
	}
};