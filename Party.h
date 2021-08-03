#pragma once
#include "CitizenArray.h"
#include "DistrictArray.h"

class Party
{
	private:
		char* party_name;
		Citizen chief_party;
		CitizenArray* representatives_by_district;
		DistrictArray* size;


	public:
		Party(const char* party_name, Citizen& chief_party, DistrictArray* size);
		Party() { party_name = nullptr; representatives_by_district = nullptr; }
		Party(istream& in, DistrictArray* da);
		~Party();
		Party(const Party& p);
		const Party& operator=(const Party& other);
		friend ostream& operator<<(ostream& os, const Party& p);
		void addRepresentative(const Citizen& representative, int district_num);
		int get_num_of_districts() const { return size->get_logs(); }
		int get_phs() const { return size->get_phs(); }
		char* get_name() const { return party_name; }
		void resize_representatives_by_district();
		bool is_chief_representative(int id);
		CitizenArray* get_representatives_by_district() const { return representatives_by_district; }
		const Citizen& get_chief_party() const { return chief_party; }
		bool save(ostream& out) const;
		bool load(istream& in, DistrictArray* da);
};

