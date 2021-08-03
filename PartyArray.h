#pragma once
#include "Party.h"

class PartyArray
{
	private:
		Party* party_arr = nullptr;
		int logs = 0;
		int phs = 0; 
	public:
		PartyArray(int phs=2);
		~PartyArray();
		PartyArray(const PartyArray& s);
		PartyArray(istream& in, DistrictArray* da);
		const PartyArray& operator=(const PartyArray& other);
		friend ostream& operator<<(ostream& os, const PartyArray& pa);
		void addParty(const char* party_name, Citizen& chief, DistrictArray* size);
		void resize(int phs, const Party& p);
		int get_logs() const { return logs; }
		int get_phs() const { return phs; }
		void add_district_to_parties();
		int getParty_by_name(char* name);
		bool Is_Representative(int id);
		Party& getParty_by_index(int index) const { return party_arr[index]; }
		bool is_citizen_chief(int id);
		bool save(ostream& out) const;
		bool load(istream& in, DistrictArray* da);
};

