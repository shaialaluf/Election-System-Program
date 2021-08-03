#include "Party.h"
#include "Citizen.h"
#include <string.h>
#pragma warning(disable: 4996) // disable strcpy error

Party::Party(const char* party_name, Citizen& chief_party, DistrictArray* size) : party_name(nullptr),
	chief_party(chief_party), size(nullptr) 	// Party constructor
{
	this->size = size;

	this->party_name = new char[strlen(party_name) + 1];
	strcpy(this->party_name, party_name);
	representatives_by_district = new CitizenArray[(this->size)->get_phs()];    
}

Party::Party(istream& in, DistrictArray* da) :party_name(nullptr), size(nullptr) // load constructor
{

	load(in, da);

}
bool Party::save(ostream& out) const
{
	int district_phs, district_logs;
	district_phs = size->get_phs();
	district_logs = size->get_logs();
	int length = strlen(this->party_name);
	out.write(rcastcc(&length), sizeof(length));
	out.write(rcastcc(this->party_name), length);
	if (!chief_party.save(out))
		return false;

	out.write(rcastcc(&district_phs), sizeof(district_phs));
	out.write(rcastcc(&district_logs), sizeof(district_logs));

	if (!out.good())
		return false;

	for (int i = 0; i < district_logs; i++)
	{
		if (!representatives_by_district[i].save(out))
			return false;
	}
	
	return out.good();
}
bool Party::load(istream& in, DistrictArray* da)
{
	int length;
	int district_phs, district_logs;
	in.read(rcastc(&length), sizeof(length));

	if (!in.good())
		return false;

	if (party_name != nullptr)
		delete[] party_name;
	party_name = new char[length+1];
	party_name[length] = '\0';

	in.read(rcastc(this->party_name), length);
	if (!chief_party.load(in, da))
		return false;

	in.read(rcastc(&district_phs), sizeof(district_phs));
	in.read(rcastc(&district_logs), sizeof(district_logs));

	if (representatives_by_district != nullptr)
		delete[] representatives_by_district;

	if (!in.good())
		return false;

	representatives_by_district = new CitizenArray[district_phs];

	for (int i = 0; i < district_logs; i++)
	{
		if (!representatives_by_district[i].load(in, da))
			return false;
	}
		
	size = da;

	return in.good();
}


Party::~Party() // Party destructor
{
	delete[] representatives_by_district;
	delete[] party_name;
}

Party::Party(const Party& p) // Party copy constructor
{
	*this = p;
}
const Party& Party::operator=(const Party& other) // Party operator=
{
	if (this != &other)
	{
		delete[] representatives_by_district;
		delete[] party_name;
		chief_party = other.chief_party;
		size = other.size;

		party_name = new char[strlen(other.party_name) + 1];
		strcpy(this->party_name, other.party_name);

		representatives_by_district = new CitizenArray[(other.size)->get_phs()];  
		for (int i = 0; i < (other.size)->get_logs(); i++)
			representatives_by_district[i] = other.representatives_by_district[i]; 
	}

	return *this;
}

ostream& operator<<(ostream& os, const Party& p) // Party operator<<
{
	os << p.party_name << ", " << p.chief_party << endl;
	for (int i = 0; i < (p.size)->get_logs(); i++)
	{
		os << "participants of the party from district "<< i << ": "<< endl;
		os << p.representatives_by_district[i] << endl;
	}
	return os;
}

void Party::addRepresentative(const Citizen& representative, int district_num) // add new representative to the representatives array in the current party and according the district number
{
	representatives_by_district[district_num].addCitizen(representative);
}

void Party::resize_representatives_by_district() // resize the representatives array and double the physical size
{
	CitizenArray* temp = new CitizenArray[size->get_phs()];
	for (int i = 0; i < size->get_logs()-1; i++)
	{
		temp[i] = representatives_by_district[i];
	}

	delete[] representatives_by_district;
	representatives_by_district = temp;
	


}


bool Party::is_chief_representative(int id) // gets id and return true if the chief is representative
{
	Citizen* c;
	for (int i = 0; i < size->get_logs(); i++)
	{
		c=representatives_by_district[i].FindCitizenById(id);
		if (c != nullptr)
			return true;
	}

	return false;
}

