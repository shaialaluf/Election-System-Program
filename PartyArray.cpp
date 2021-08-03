#include "PartyArray.h"

PartyArray::PartyArray(int phs) :
	phs(phs), party_arr(new Party[phs]), logs(0) // PartyArray constructor
{

}

PartyArray::PartyArray(istream& in, DistrictArray* da) : phs(0), party_arr(nullptr), logs(0) // load constructor
{

	load(in, da);

}

bool PartyArray::save(ostream& out) const
{
	out.write(rcastcc(&phs), sizeof(phs));
	out.write(rcastcc(&logs), sizeof(logs));

	if (!out.good())
		return false;

	for (int i = 0; i < logs; i++)
	{
		if (!party_arr[i].save(out))
			return false;
	}
		
	return out.good();
}

bool PartyArray::load(istream& in, DistrictArray* da)
{
	in.read(rcastc(&phs), sizeof(phs));
	in.read(rcastc(&logs), sizeof(logs));

	if (party_arr != nullptr)
	{
		delete[] party_arr;
	}

	if (!in.good())
		return false;

	party_arr = new Party[phs];

	for (int i = 0; i < logs; i++)
	{
		if (!party_arr[i].load(in, da))
			return false;
	}
	return in.good();
}

PartyArray::~PartyArray() // PartyArray destructor
{
	delete[] party_arr;
}

PartyArray::PartyArray(const PartyArray& pa) // PartyArray copy constructor
{
	*this = pa;
}

const PartyArray& PartyArray::operator=(const PartyArray& other) // PartyArray operator=
{
	if (this != &other)
	{
		delete[] party_arr;
		logs = other.logs;
		phs = other.phs;
		party_arr = new Party[phs];
		for (int i = 0; i < other.logs; i++)
			party_arr[i] = other.party_arr[i];
	}

	return *this;
}

ostream& operator<<(ostream& os, const PartyArray& pa) // PartyArray operator<<
{
	for (int i = 0; i < pa.logs; i++)
	{
		os << i << ". " << pa.party_arr[i];
		cout << endl;
	}
	return os;
}


void PartyArray::addParty(const char* party_name,  Citizen& chief, DistrictArray* size) //  gets party details and add the party to the array and resize if the place is over
{
	Party p(party_name, chief, size);
	if (logs < phs)
		party_arr[logs] = p;
	else
		resize(phs,p);

	logs++;
}

void PartyArray::resize(int phs, const Party& p) // resize the array to a double size
{
	Party* temp = new Party[phs*2];
	for (int i = 0; i < phs; i++)
	{
		temp[i] = party_arr[i];
	}

	delete[] party_arr;
	party_arr = temp;
	this->phs *= 2;
	party_arr[logs] = p;
}

void PartyArray::add_district_to_parties() // add the new district to all the parties
{
	for (int i = 0; i < logs; i++)
	{
		if (party_arr[i].get_num_of_districts()+1 >= party_arr[i].get_phs())
		{
			party_arr[i].resize_representatives_by_district();	
		}

		
	}
}

int PartyArray::getParty_by_name(char* name) // gets name and return the index of the party id it is exist
{
	int i = 0;
	bool flag = false;
	int res=-1;
	while (i < logs && !flag)
	{
		if (strcmp(name, party_arr[i].get_name()) == 0)
		{
			res = i;
			flag = true;
		}

		i++;
	}

	return res;
}
bool PartyArray::Is_Representative(int id) // gets id and return true if this is a representative of the party or false if it's not
{
	int i,j;
	for (i = 0; i < logs; i++)
	{
		if (id== party_arr[i].get_chief_party().getId())
			return false;
		for (j = 0; j < party_arr[i].get_num_of_districts(); j++)
		{
			if (party_arr[i].get_representatives_by_district()[j].FindCitizenById(id) != nullptr)
				return false;
		}
	}
	return true;
}

bool PartyArray::is_citizen_chief(int id) // gets id and return true if this citizen is a chief in one of the parties
{
	for (int i = 0; i < logs; i++)
	{
		if (id == party_arr[i].get_chief_party().getId())
			return true;
	}

	return false;
}