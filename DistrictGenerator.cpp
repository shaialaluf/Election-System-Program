#include "DistrictGenerator.h"
#include "DividedDistrict.h"
#include "UnitedDistrict.h"

DistrictGenerator::DistrictType DistrictGenerator::district_generator(District* const d) // gets a district and return its type
{
	if (typeid(*d) == typeid(DividedDistrict))
		return DistrictType::DIVIDED;

	else if (typeid(*d) == typeid(UnitedDistrict))
		return DistrictType::UNITED;
}

District* DistrictGenerator::load(istream& in, DistrictArray* da, PartyArray* pa) // gets a file and load a new divided/united district from the file
{
	DistrictType type;
	in.read(rcastc(&type), sizeof(type));
	District* d=nullptr;

	if (type == DistrictType::DIVIDED)
	{
		d=new DividedDistrict(nullptr,0,pa);
		if (!d->load(in, da, pa))
			return nullptr;
	}
		

	else if (type == DistrictType::UNITED)
	{
		d = new UnitedDistrict(nullptr, 0, pa);
		if (!d->load(in, da, pa))
			return nullptr;
	}
	return d;

	
}

