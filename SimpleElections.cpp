#include "SimpleElections.h"
#include "DividedDistrict.h"
#include "UnitedDistrict.h"
#include <math.h> 

SimpleElections::SimpleElections(int day, int month, int year, int num_of_representatives) : Elections(day,month, year),
							district_hidden(1)// SimpleElections constructor
{
	DividedDistrict divided(nullptr, num_of_representatives, &(this->get_parties()));
	district_hidden.addDistrict(&divided); 

}


SimpleElections::~SimpleElections() // SimpleElections destructor
{
	
}

SimpleElections::SimpleElections(istream& in) : Elections(in) // load constructor
{

	load(in);

}

bool SimpleElections::save(ostream& out) const
{
	ElectionsType type = ElectionsType::SIMPLE;
	out.write(rcastcc(&type), sizeof(type));
	out.write(rcastcc(&day), sizeof(day));
	out.write(rcastcc(&month), sizeof(month));
	out.write(rcastcc(&year), sizeof(year));

	if (!out.good())
		return false;

	if (!district_hidden.save(out))
		return false;
	if (!Elections::save(out))
		return false;

	return out.good();

}

bool SimpleElections::load(istream& in)
{
	in.read(rcastc(&day), sizeof(day));
	in.read(rcastc(&month), sizeof(month));
	in.read(rcastc(&year), sizeof(year));

	if (!in.good())
		return false;

	if (!district_hidden.load(in, &parties))
		return false;
	if (!citizens.load(in, &district_hidden))
		return false;
	if (!parties.load(in, &district_hidden))
		return false;
	init_representives();
	if (!Elections::load(in))
		return false;

	return in.good();
}



void SimpleElections::init_results_sum() // creates new array for the results and init it
{
	int size = parties.get_logs();
	if (results_sum != nullptr)
		delete[] results_sum;
	results_sum = new sumOfResult[size];
	for (int i = 0; i < size; i++)
		results_sum[i].party_id = i;

		for (int j = 0; j < size; j++)
		{
			results_sum[j].sum_of_votes = district_hidden[0]->get_results()[j].votes_number;
			results_sum[j].sum_of_representatives = district_hidden[0]->get_results()[j].elected_representatives.get_logs();
		}



	}


bool SimpleElections::build_representative_list() //  init the elected representatives for each party according to the results, using the base class
{
	return Elections::build_representative_list(district_hidden);
}


void SimpleElections::sort_results_sum() // sort the new array of results - from the party with the most sum of votes to the least one
{
	sumOfResult temp;
	int size = parties.get_logs();
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - i - 1; j++)
		{
			if (results_sum[j].sum_of_votes < results_sum[j + 1].sum_of_votes)
			{
				temp.party_id = results_sum[j].party_id;
				temp.sum_of_representatives = results_sum[j].sum_of_representatives;
				temp.sum_of_votes = results_sum[j].sum_of_votes;

				results_sum[j].party_id = results_sum[j + 1].party_id;
				results_sum[j].sum_of_representatives = results_sum[j + 1].sum_of_representatives;
				results_sum[j].sum_of_votes = results_sum[j + 1].sum_of_votes;

				results_sum[j + 1].party_id = temp.party_id;
				results_sum[j + 1].sum_of_representatives = temp.sum_of_representatives;
				results_sum[j + 1].sum_of_votes = temp.sum_of_votes;

			}
		}

}

void SimpleElections::init_representives() // init_representives
{
	for (int j = 0; j < parties.get_logs(); j++)
	{
		for (int k = 0; k < district_hidden[0]->get_results()[j].elected_representatives.get_logs(); k++)
		{

			district_hidden[0]->get_results()[j].elected_representatives.get_citizen_by_index(k).set_district(district_hidden[0]);

		}
	}
}



