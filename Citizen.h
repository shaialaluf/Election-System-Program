#pragma once
#include <iostream>
#include "DynamicArray.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

class District;
class DistrictArray;
using namespace std;
class Citizen
{
private:

	char* name;
	int id;
	District* d;
	int birth_year;
	bool isVoted;
public:
	Citizen(const char* name, int id, int birth_year, District* d, bool isVoted = false);
	Citizen(istream& in, DistrictArray* da);
	~Citizen();
	Citizen() { name = nullptr; d = nullptr; }
	Citizen(const Citizen& c);
	const Citizen& operator=(const Citizen& other);
	friend ostream& operator<<(ostream& os, const Citizen& c);
	char* getName() const { return name; }
	int getId()  const { return id; }
	int getBirth_Year() const { return birth_year; }
	District* getDistrict_Citizen() const { return d; }
	bool getIs_Voted() const { return isVoted; }
	bool set_Birth_Year(int birth_year);
	bool set_id(int id);
	void set_isVoted(bool isvoted);
	void set_district(District* district);
	bool save(ostream& out) const;
	bool load(istream& in, DistrictArray* da);
	bool load(istream& in, District* d);
	int setlength(istream& in);

};