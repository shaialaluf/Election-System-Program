#pragma once
#include "District.h"
#include <iostream>
using namespace std;



class DistrictArray
{
private:
	District** arr = nullptr;
	int logs = 0;
	int phs = 0; 

public:
	DistrictArray(int phs = 2);
	DistrictArray(istream& in, PartyArray* pa);
	~DistrictArray();
	DistrictArray(const DistrictArray& da);
	const DistrictArray& operator=(const DistrictArray& other);
	friend ostream& operator<<(ostream& os, const DistrictArray& da);
	void resize(int phs, District* const d);
	int get_logs() const { return logs; }
	int get_phs() const { return phs; }
	District* operator[](int i) { return arr[i]; }
	void addDistrict( District* const d);
	void add_party_to_districts();
	bool save(ostream& out) const;
	bool load(istream& in, PartyArray* pa);
	
};

