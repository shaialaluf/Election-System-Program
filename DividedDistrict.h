#pragma once
#include "District.h"
class DividedDistrict: public District
{
	private:
		struct PartyResult_sorted
		{
			int party_id;
			PartyResult party_res;
		};

		PartyResult_sorted* sorted_results = nullptr;

	public:
		DividedDistrict(const char* name, int number_of_representatives, PartyArray* results_size);
		DividedDistrict(istream& in, DistrictArray* da, PartyArray* pa);
		virtual ~DividedDistrict();
		DividedDistrict(const DividedDistrict& ud);
		virtual const DividedDistrict& operator=(const DividedDistrict& other);
		void calc_winner_party() override;
		virtual void show() const { cout << " Divided District "; }
		PartyResult_sorted* get_sorted_results() const { return sorted_results; }
		virtual bool save(ostream& out) const;
		virtual bool load(istream& in, DistrictArray* da, PartyArray* pa);
		

};

