#pragma once
#include <iostream>
#include <fstream>
#include "District.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

class DistrictArray;


class DistrictGenerator
{
	public:
		enum class DistrictType { DIVIDED, UNITED };
		static DistrictType district_generator(District* const d);
		static District* load(istream& in, DistrictArray* da, PartyArray* pa);

};

