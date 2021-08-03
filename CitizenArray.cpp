#include "CitizenArray.h"


CitizenArray::CitizenArray(int phs) : // CitizenArray constrctor
	phs(phs), arr(nullptr), logs(0)
{
	arr = new Citizen[phs];
}
CitizenArray::CitizenArray(istream& in,DistrictArray* da) : arr(nullptr), phs(0) , logs(0) // load constructor
{

	load(in, da);

}

CitizenArray::~CitizenArray() // CitizenArray destructor
{
	delete[] arr;
}
bool CitizenArray::save(ostream& out) const
{
	out.write(rcastcc(&phs), sizeof(phs));
	out.write(rcastcc(&logs), sizeof(logs));
	if (!out.good())
		return false;

	for (int i = 0; i < logs; i++)
	{
		if (!arr[i].save(out))
			return false;
	}
		
	return true;
}
bool CitizenArray::load(istream& in,  DistrictArray* da)
{

	if (!setlength(in))
		return false;

	for (int i = 0; i < logs; i++)
	{
		if (!arr[i].load(in, da))
			return false;
	}
	return true;
}

bool CitizenArray::load(istream& in, District* d)
{
	if (!setlength(in))
		return false;

	for (int i = 0; i < logs; i++)
	{
		if(!arr[i].load(in, d))
			return false;
	}
	return true;
}

bool CitizenArray::setlength(istream& in) // set new length to the citizen array
{
	in.read(rcastc(&phs), sizeof(phs));
	in.read(rcastc(&logs), sizeof(logs));
	if (!in.good())
		return false;

	if (arr != nullptr)
	{
		delete[] arr;
	}


	arr = new Citizen[phs];
	return true;
}

void CitizenArray::addCitizen(const Citizen& c) // add new citizen to the array data member and resize the array when there's no more place
{
	if (logs < phs)
		arr[logs] = c;
	else
		resize(phs, c);

	logs++;
}

void CitizenArray::resize(int phs, const Citizen& c) // resize the array and double the array physical size
{
	Citizen* temp = new Citizen[phs * 2];
	for (int i = 0; i < phs; i++)
	{
		temp[i] = arr[i];
	}

	delete[] arr;
	arr = temp;
	this->phs *= 2;
	arr[logs] = c;
}



ostream& operator<<(ostream& os, const CitizenArray& ca) // CitizenArray operator<<
{
	for (int i = 0; i < ca.logs; i++)
			os << ca.arr[i];

	return os;
}

CitizenArray::CitizenArray(const CitizenArray& s) // CitizenArray copy constructor
{
	*this = s;
}

const CitizenArray& CitizenArray::operator=(const CitizenArray& other) // CitizenArray operator=
{
	if (this != &other)
	{
		delete[] arr;
		logs = other.logs;
		phs = other.phs;
		arr = new Citizen[phs];
		for (int i = 0; i < other.logs; i++)
			arr[i] = other.arr[i];
	}

	return *this;
}

Citizen* CitizenArray::FindCitizenById(int id) // gets id and return the address of a citizen if he is exist or nullptr if he is not exist
{
	int i = 0;
	bool is_found = false;
	
	while(i<logs && is_found==false)
	{
		if (arr[i].getId() == id)
			is_found = true;
		else
			i++;
	}

	if (is_found == false)
		return nullptr;

	return &(arr[i]);
}

bool CitizenArray::set_logs(int size)
{
	if (phs < size)
		return false;

	else
		logs = size;
}