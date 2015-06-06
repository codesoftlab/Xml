/*
 * XmlObject.cpp
 *
 *  Created on: 30 maj 2015
 *      Author: karol
 */
#include "XmlObject.h"

XmlObject::XmlObject(string xelem):element(xelem){}

void XmlObject::set_elem(string xelem)
{
	element=xelem;
}

XmlObject::~XmlObject(){}

XmlObject_string::XmlObject_string(string xelem,string xname):XmlObject(xelem),name(xname){}

string XmlObject_string::to_xml()
{
	stringstream tmp;
	string strm;
	if(element=="\0")
	{
		cerr<<"Pusty element w XmlObject_string."<<endl;
		return 0;
	}
	tmp<<'<'<<XmlObject::element<<'>'<<name<<"</"<<XmlObject::element<<'>';
	strm=tmp.str();
	return strm;
}

void XmlObject_string::from_xml(ifstream& File)
{
	int j=0;
	char c=0;
	string tmp='<'+element+'>';
	for(int i=0;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}
	for(c=0; File>>c && c!='<';++j)
		tmp[j]=c;
	tmp[j]='\0';
	tmp.resize(j);
	name=tmp;
	tmp="</"+element+'>';
	for(int i=1;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}

}

string XmlObject_string::ret_elem()
{
	return XmlObject::element;
}

void XmlObject_string::set_elem(string xelem)
{
	XmlObject::set_elem(xelem);
}

ifstream& operator >>(ifstream& File, XmlObject_string& x)
{
	if(!File.is_open())
		cerr<<"File not open";
	x.from_xml(File);
	return File;
}


ofstream& operator <<(ofstream& File, XmlObject_string x)
{
	if(!File.is_open())
		cerr<<"File not open";
	File<<x.to_xml();
	return File;
}

ostream& operator <<(ostream& os, XmlObject_string x)
{
	os<<x.to_xml();
	return os;
}

XmlObject_int::XmlObject_int(string xelem, int x):XmlObject(xelem), value(x){}

string XmlObject_int::to_xml()
{
	string strm;
	stringstream tmp;
	tmp<<'<'<<XmlObject::element<<'>'<<value<<"</"<<XmlObject::element<<'>';
	strm=tmp.str();
	return strm;
}

void XmlObject_int::from_xml(ifstream& File)
{
	value=0;
	char c=0;
	int j=0;
	string tmp='<'+element+'>';
	for(int i=0;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}
	tmp.clear();
	for(j=0; File>>c && c!='<';++j)
	{
		tmp[j]=c;
	}
	--j;
	for(int i=0; i<=j; ++i)
	{
		value+=pow(10,(j-i))*((int)tmp[i]-48);
	}
	tmp="</"+element+'>';
	for(int i=1;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}


}

string XmlObject_int::ret_elem()
{
	return XmlObject::element;
}

int XmlObject_int::ret_value()
{
	return value;
}

void XmlObject_int::set_value(int x)
{
	value=x;
}

ifstream& operator >>(ifstream& File, XmlObject_int& x)
{
	if(!File.is_open())
		cerr<<"File not open";
	x.from_xml(File);
	return File;
}

ofstream& operator <<(ofstream& File, XmlObject_int x)
{
	if(!File.is_open())
		cerr<<"File not open";
	File<<x.to_xml();
	return File;
}

ostream& operator <<(ostream& os, XmlObject_int x)
{
	os<<x.to_xml();
	return os;
}

XmlObject_char::XmlObject_char(string xelem, char xmark):XmlObject(xelem), mark(xmark){}

string XmlObject_char::to_xml()
{
	string strm;
	stringstream tmp;
	tmp<<'<'<<XmlObject::element<<'>'<<mark<<"</"<<XmlObject::element<<'>';
	strm=tmp.str();
	return strm;
}

void XmlObject_char::from_xml(ifstream& File)
{

	char c=0;
	string tmp='<'+element+'>';
	for(int i=0;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}
	File>>mark;
	tmp="</"+element+'>';
	for(int i=0;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}
}

string XmlObject_char::ret_elem()
{
	return XmlObject::element;
}

ifstream& operator >>(ifstream& File, XmlObject_char& x)
{
	if(!File.is_open())
		cerr<<"File not open";
	x.from_xml(File);
	return File;
}

ofstream& operator <<(ofstream& File, XmlObject_char x)
{
	if(!File.is_open())
		cerr<<"File not open";
	File<<x.to_xml();
	return File;
}

ostream& operator <<(ostream& os, XmlObject_char x)
{
	os<<x.to_xml();
	return os;
}

template <int size>
XmlVect<size>:: XmlVect()
					:element(NULL),count(0),SIZE("SIZE", size), state(empty)
{
	Tab=new XmlObject_string[size];
}

template <int size>
XmlVect<size>::~XmlVect()
{
	delete Tab;
}

template <int size>
void XmlVect<size>::set_elem(string xelem, string elem_list)
{
	element=xelem;
	for(int i=0; i<SIZE.ret_value(); ++i)
	{
		Tab[i].set_elem(elem_list);
	}
}

template < int size>
bool XmlVect<size>::push(XmlObject_string no)
{
	if(state==full)
	{
		cerr<<"pelny vector"<<endl;
		return 0;
	}
	else
	{
		++count;
		Tab[count]=no;
		if(count==SIZE.ret_value())
			state=full;
		if(state==empty)
			state=some_elements;
		return 1;
	}

}

template <int size>
bool XmlVect<size>::remove(int x)
{
	if(state==empty)
	{
		cerr<<"pusty vector"<<endl;
		return 0;
	}
	Tab[count]=NULL;
	--count;
	if(state==full)
		state==some_elements;
	if(count==0)
		state=empty;
	return 1;
}

template <int size>
string XmlVect<size>::to_xml()
{
	stringstream tmp=NULL;
	string output=NULL;
	int size_tmp=SIZE.ret_value();
	tmp<<"<"+element+'>';
	SIZE.set_value(count);
	tmp<<SIZE.to_xml();
	SIZE.set_value(size_tmp);
	if(state==empty)
	{
		cerr<<"Empty vector, nothing to parse."<<endl;
		return 0;
	}
	for(int i=0; i<=count; ++i)
	{
		tmp<<Tab[i].to_xml();
	}
	tmp<<"</"+element+'>';
	output=tmp.str();
	return output;
}

template <int size>
void XmlVect<size>::from_xml(ifstream& File)
{
	char c=0;
	string tmp='<'+element+'>';
	for(int i=0;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}
	SIZE.from_xml(File);
	delete Tab;
	Tab=new XmlObject_string[SIZE.ret_value()];
	for(int i=0; i<=SIZE.ret_value(); ++i)
	{
		Tab[i].from_xml(File);
	}
	tmp="</"+element+'>';
	for(int i=0;i<tmp.length() && File>>c;i++)
	{
		if(tmp[i]!=c)
		{
			cerr<<"Blad pliku. Niepoprawny Xml.Element: "<<element<<endl;
			return;
		}
	}


}
