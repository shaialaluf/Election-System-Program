#include "DividedDistrict.h"
#include "District.h"
#include "PartyArray.h"

DividedDistrict::DividedDistrict(const char* name, int number_of_representatives, PartyArray* results_size) : District(name, number_of_representatives, results_size) // regular constructor 
{
	sorted_results = nullptr;
}


DividedDistrict::DividedDistrict(istream& in, DistrictArray* da, PartyArray* pa) : District(nullptr,0,pa) // load constructor 
{
	sorted_results = nullptr;
	load(in, da, pa);

}

 bool DividedDistrict::save(ostream& out) const
{
	 if (!District::save(out))
		 return false;
	 int sorted_phs = 0;
	 int sorted_logs = 0;

	 if (sorted_results != nullptr)
	 {
		  sorted_phs = this->results_size->get_phs();
		  sorted_logs = this->results_size->get_logs();

		  out.write(rcastcc(&sorted_phs), sizeof(sorted_phs));
		  out.write(rcastcc(&sorted_logs), sizeof(sorted_logs));

		  if (!out.good())
			  return false;

		 for (int i = 0; i < sorted_logs; i++)
		 {
			 out.write(rcastcc(&(sorted_results[i].party_id)), sizeof(int));
			 out.write(rcastcc(&sorted_results[i].party_res.votes_number),sizeof(int));
			 if (!sorted_results[i].party_res.elected_representatives.save(out))
				 return false;

		 }
	 }

	 else
	 {
		 out.write(rcastcc(&sorted_phs), sizeof(sorted_phs));
	 }

	 return out.good();

}

 bool DividedDistrict::load(istream& in, DistrictArray* da, PartyArray* pa)
{
	 if (!District::load(in, da, pa))
		 return false;
	 int sorted_phs;
	 int sorted_logs;


	 if (sorted_results != nullptr)
	 {
		 delete[] sorted_results;
	 }

	 in.read(rcastc(&sorted_phs), sizeof(sorted_phs));
	 if (!in.good())
		 return false;

	 if (sorted_phs != 0)
	 {
		 sorted_results = new PartyResult_sorted[sorted_phs];
		 in.read(rcastc(&sorted_logs), sizeof(sorted_logs));

		 if (!in.good())
			 return false;

		 for (int i = 0; i < sorted_logs; i++)
		 {
			 in.read(rcastc(&(sorted_results[i].party_id)), sizeof(int));
			 in.read(rcastc(&sorted_results[i].party_res.votes_number), sizeof(int));
			 if (!sorted_results[i].party_res.elected_representatives.load(in, da))
				 return false;
		 }
	 }

	 else
		 sorted_results = nullptr;

	 return in.good();
}
 DividedDistrict::~DividedDistrict() // DividedDistrict destructor 
{
	 delete[] sorted_results;
}
DividedDistrict::DividedDistrict(const DividedDistrict& dd) // DividedDistrict copy constructor 
{
	*this = dd;
}

const DividedDistrict&  DividedDistrict:: operator=(const DividedDistrict& other) // DividedDistrict operator= 
{

	if (this != &other)
	{
		District:: operator=(other);
		delete[] sorted_results;

		sorted_results = new PartyResult_sorted[other.results_size->get_phs()];
		int size = other.results_size->get_logs();
		if (other.sorted_results != nullptr)
		{
			for (int i = 0; i < size; i++)
			{
				sorted_results[i].party_id = other.sorted_results[i].party_id;
				sorted_results[i].party_res.elected_representatives = other.sorted_results[i].party_res.elected_representatives;
				sorted_results[i].party_res.votes_number = other.sorted_results[i].party_res.votes_number;
			}
		}
	}

	return *this;
}

void DividedDistrict::calc_winner_party() // create sorted result array - to see the winner party and the rest of the results after the election round was done
{
	if (sorted_results != nullptr)
		delete[] sorted_results;

	sorted_results = new PartyResult_sorted[results_size->get_logs()];
	int size = results_size->get_logs();

	for (int i = 0; i < size; i++)
	{
		sorted_results[i].party_res.elected_representatives=results[i].elected_representatives;
		sorted_results[i].party_res.votes_number = results[i].votes_number;
		sorted_results[i].party_id = i;
	}

	PartyResult temp;

	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - i - 1; j++)
		{
			if (sorted_results[j].party_res.elected_representatives.get_logs() < sorted_results[j + 1].party_res.elected_representatives.get_logs())
			{

				temp.elected_representatives = sorted_results[j].party_res.elected_representatives;
				temp.votes_number = sorted_results[j].party_res.votes_number;


				sorted_results[j].party_res.elected_representatives = sorted_results[j + 1].party_res.elected_representatives;
				sorted_results[j].party_res.votes_number = sorted_results[j + 1].party_res.votes_number;

				sorted_results[j + 1].party_res.elected_representatives = temp.elected_representatives;
				sorted_results[j + 1].party_res.votes_number = temp.votes_number;
			}
		}
}