#include "DistrictArray.h"
#include "UnitedDistrict.h"
#include "DividedDistrict.h"
#include <typeinfo>
#include "DistrictGenerator.h"


DistrictArray::DistrictArray(int phs) :  // DistrictArray constructor
	phs(phs), arr(new District*[phs]), logs(0)
{

}
DistrictArray::DistrictArray(istream& in, PartyArray* pa) : phs(0), arr(nullptr) , logs(0) // load constructor
{

	load(in,pa);

}

bool DistrictArray::save(ostream& out) const
{
	out.write(rcastcc(&phs), sizeof(phs));
	out.write(rcastcc(&logs), sizeof(logs));

	if (!out.good())
		return false;

	for (int i = 0; i < logs; i++)
	{
		DistrictGenerator::DistrictType type = DistrictGenerator::district_generator(arr[i]);
		out.write(rcastcc(&type), sizeof(type));
		if (!arr[i]->save(out))
			return false;
	}
	return out.good();
}



bool DistrictArray::load(istream& in, PartyArray* pa)
{
	
	if (arr != nullptr)
	{
		for (int i = 0; i < logs; i++)
			delete arr[i];

		delete[] arr;
	}

	in.read(rcastc(&phs), sizeof(phs));
	in.read(rcastc(&logs), sizeof(logs));

	if (!in.good())
		return false;
	arr = new District*[phs];

	for (int j = 0; j < logs; j++)
	{
		arr[j] =nullptr;
	}

	for (int i = 0; i < logs; i++)
	{
		arr[i] = DistrictGenerator::load(in,this,pa);
		if (arr[i] == nullptr)
			return false;
	}
		
	return in.good();
}

DistrictArray::~DistrictArray() // DistrictArray destrructor
{
	for (int i = 0; i < logs; i++)
		delete arr[i];

	delete[] arr;

}


ostream& operator<<(ostream& os, const DistrictArray& da) // DistrictArray operator<<
{
	for (int i = 0; i < da.logs; i++)
		os << *(da.arr[i]);

	return os;
}

void DistrictArray::addDistrict( District* const d) // add new district the array and resize it when place is over
{
	District* district=nullptr;
	if (typeid(*d) == typeid(UnitedDistrict))
	{
	
		district = new UnitedDistrict(*(dynamic_cast<UnitedDistrict*>(d)));
		
	}

	else if (typeid(*d) == typeid(DividedDistrict))
	{
	
		district = new DividedDistrict(*(dynamic_cast<DividedDistrict*>(d)));
		
	}
	
	if (logs < phs)
	{
		arr[logs] = district;
	}
	else 
		resize(phs, district);

	logs++;
}

void DistrictArray::resize(int phs,  District* const d) // resize the district array to a double size
{
	District** temp = new District*[phs * 2];
	for (int i = 0; i < phs; i++)
	{
		temp[i] = arr[i];
	}

	delete[] arr;
	arr = temp;
	this->phs *= 2;
	arr[logs] = d;



}

DistrictArray::DistrictArray(const DistrictArray& da) // DistrictArray copy constructor
{
	*this = da;
}

const DistrictArray& DistrictArray::operator=(const DistrictArray& other)// DistrictArray operator=
{
	if (this != &other)
	{
		delete[] arr;
		logs = other.logs;
		phs = other.phs;
		arr = new District*[phs];
		for (int i = 0; i < other.logs; i++)
			arr[i] = other.arr[i];
	}

	return *this;
}

void DistrictArray::add_party_to_districts() // add a new party to all the districts 
{
	for (int i = 0; i < logs; i++)
	{
		if (arr[i]->get_parties_number() + 1 >= arr[i]->get_results_phs())
		{
			arr[i]->resize_results();
		}

		arr[i]->update_num_of_parties();
	}
}