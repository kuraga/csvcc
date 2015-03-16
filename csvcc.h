// CSVcc 0.1.0
// https://github.com/kuraga/csvcc
// Copyright (C) 2015, by Kurakin Alexander (kuraga333@mail.ru)
//
// The MIT License (MIT)
// http://opensource.org/licenses/MIT

// Minimalistic CSV Streams 1.4
// https://code.google.com/p/minicsv/
// Copyright (C) 2014, by Wong Shao Voon (shaovoon@yahoo.com)
//
// The MIT License (MIT)
// http://opensource.org/licenses/MIT

#ifndef _CSV_H
#define _CSV_H

#include <string>
#include <iostream>

namespace csv
{

	class istream
	{
	public:
		istream(std::istream &istm, char delimeter = ',')
      : _istream(istm), _str(""), _pos(0), _delimeter(delimeter)
		{
		}

		bool eof() const
		{
			return (_istream.eof()&&_str=="");
		}

		bool eol() const
		{
			return (_pos>=_str.size());
		}

		char get_delimeter() const
		{
			return _delimeter;
		}

    std::istream& get_istream()
    {
      return _istream;
    }

		void skip_line()
		{
			if(!_istream.eof())
			{
				std::getline(_istream, _str);
				_pos = 0;
			}
		}
		std::string get_delimited_str()
		{
			std::string str = "";
			char ch = '\0';
			do
			{
				if(_pos>=_str.size())
				{
					if(!_istream.eof())
					{
						std::getline(_istream, _str);
						_pos = 0;
					}
					else
					{
						_str = "";
						break;
					}

					if(!str.empty())
						return str;
				}

				ch = _str[_pos];
				++(_pos);
				if(ch==_delimeter||ch=='\r'||ch=='\n')
					break;

				str += ch;
			}
			while(true);

			return str;
		}

	private:
		std::istream& _istream;
		std::string _str;
		size_t _pos;
		char _delimeter;
	};

	class ostream
	{
	public:

		ostream(std::ostream& ostr, char delimeter = ',', bool after_newline = true)
      : _ostream(ostr), _after_newline(after_newline), _delimeter(delimeter)
		{
		}

		char get_delimeter() const
		{
			return _delimeter;
		}

		bool get_after_newline() const
		{
			return _after_newline;
		}

		std::ostream& get_ostream()
		{
			return _ostream;
		}

		void set_after_newline(bool after_newline)
		{
			_after_newline = after_newline;
		}

	private:
		std::ostream& _ostream;
		bool _after_newline;
		char _delimeter;
	};


}

template<typename T>
inline csv::istream& operator >> (csv::istream& _istream, T& val)
{
	std::string str = _istream.get_delimited_str();
	
	return _istream;
}

template<>
inline csv::istream& operator >> (csv::istream& _istream, std::string& val)
{
	val = _istream.get_delimited_str();

	return _istream;
}

template<typename T>
inline csv::ostream& operator << (csv::ostream& _ostream, const T& val)
{
	if(!_ostream.get_after_newline())
		_ostream.get_ostream() << _ostream.get_delimeter();

	_ostream.get_ostream() << val;
	_ostream.set_after_newline(false);

	return _ostream;
}

template<>
inline csv::ostream& operator << (csv::ostream& _ostream, const char& val)
{
	if(val=='\n')
	{
		_ostream.get_ostream() << std::endl;
		_ostream.set_after_newline(true);
	}
	else
		_ostream.get_ostream() << val;

	return _ostream;
}

#endif
