#include "Citizen.h"
#include <string>
#include "District.h"
#include "DistrictArray.h"
#pragma warning(disable: 4996) // disable strcpy error

Citizen::Citizen(const char* name, int id, int birth_year,  District* d, bool isVoted) : name(nullptr),
	 isVoted(isVoted),d(nullptr) // citizen constructor
{
	if (!set_Birth_Year(birth_year))
		this->birth_year = -1;

	if (!set_id(id))
		this->id = -2;

	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->d = d;

}

Citizen::Citizen(istream& in, DistrictArray* da):name(nullptr), id(0), d(nullptr), birth_year(0), isVoted(false) // load constructor
{
	load(in,da);

}


Citizen::~Citizen() // citizen destructor
{
	delete[] name;
}

bool Citizen::save(ostream& out) const
{
	int dis_num = d->get_serial_number();
	int length = strlen(this->name);
	out.write(rcastcc(&length), sizeof(length));
	out.write(rcastcc(this->name), length);
	out.write(rcastcc(&id), sizeof(id));
	out.write(rcastcc(&dis_num), sizeof(int)); // write dis_num 
	out.write(rcastcc(&birth_year), sizeof(birth_year));
	out.write(rcastcc(&isVoted), sizeof(isVoted));

	return out.good();

}
bool Citizen::load(istream& in, DistrictArray* da)
{

	int dis_num=setlength(in);
	if (dis_num == -1)
		return false;
	else
	{
		this->d = (*da)[dis_num];
		return true;
	}

}
bool Citizen::load(istream& in, District* d)
{
	if (setlength(in) == -1)
		return false;
	else
	{
		this->d = d;
		return true;
	}
	
}

int Citizen::setlength(istream& in) // set new length and return district of a citizen
{
	int dis_num;
	int length;
	in.read(rcastc(&length), sizeof(length));
	if (!in.good())
		return -1; // error reading length
	if (name != nullptr)
		delete[] name;
	name = new char[length + 1];
	in.read(rcastc(name), length);
	if (!in.good())
		return -1; // error reading name
	name[length] = '\0';
	in.read(rcastc(&id), sizeof(id));
	in.read(rcastc(&dis_num), sizeof(int)); // write dis_num 
	in.read(rcastc(&birth_year), sizeof(birth_year));
	in.read(rcastc(&isVoted), sizeof(isVoted));
	if (!in.good())
		return -1;
	return dis_num;
}

Citizen::Citizen(const Citizen& c) // citizen copy constructor
{
	*this = c;
}

const Citizen& Citizen::operator=(const Citizen& other) // citizen operator= 
{
	if (this != &other)
	{
		delete[] name;
		id = other.id;
		birth_year = other.birth_year;
		d = other.d;
		isVoted = other.isVoted;
		name = new char[strlen(other.name)+1];
		strcpy(this->name, other.name);
	}

	return *this;

}

ostream& operator<<(ostream& os, const Citizen& c) // citizen operator<< 
{
	os << "name: "<<c.name << " , id: " << c.id << " , birth year: " << c.birth_year << " , district num: " << (c.d)->get_serial_number() << endl;
	return os;
}

bool Citizen::set_Birth_Year(int birth_year) // set birth year of a citizen. return false if the year is smaller than 1900 (and not set the year)
{
	if (birth_year > 0 && birth_year < 2020)
	{
		this->birth_year = birth_year;
		return true;
	}
		
	else
		return false;
		
}


bool Citizen::set_id(int id) // set id of a citizen. return false if the id is smaller than 0 (and not set the id)
{
	if (id > 0)
	{
		this->id = id;
		return true;
	}

	else
		return false;
		
}

void Citizen::set_isVoted(bool isvoted) // setter of isVoted data member
{
	this->isVoted = isvoted;
}

void Citizen::set_district(District* district) 
{
	this->d = district;
}