#pragma once
#include "Citizen.h"
#include <iostream>
using namespace std;

class CitizenArray
{
	private:
		Citizen* arr=nullptr;
		int logs=0;
		int phs=0; 

	public:
		CitizenArray(int phs = 2);
		CitizenArray(istream& in, DistrictArray* da);
		~CitizenArray();
		CitizenArray(const CitizenArray& pa);
		const CitizenArray& operator=(const CitizenArray& other);
		friend ostream& operator<<(ostream& os, const CitizenArray& ca);
		Citizen* FindCitizenById(int id);
		void resize(int phs, const Citizen& c);
		void addCitizen(const Citizen& c);
		int get_logs() const { return logs; }
		int get_phs() const { return phs; }
		bool set_logs(int size);
		Citizen& get_citizen_by_index(int index) const { return arr[index]; }
		bool save(ostream& out) const;
		bool load(istream& in, DistrictArray* da);
		bool load(istream& in, District* d);
		bool setlength(istream& in);
};

