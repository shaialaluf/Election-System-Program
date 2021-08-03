#include "Elections.h"
#include "UnitedDistrict.h"
#include <math.h> 

Elections::Elections(int day, int month, int year) // Elections constructor
{
	set_Year(year);
	set_Month(month);
	set_Day(day);

	results_sum = nullptr;
}

Elections::Elections(istream& in) : day(0), month(0), year(0) // load constructor
{

	results_sum = nullptr;

}

bool Elections::save(ostream& out) const
{
	if (!citizens.save(out))
		return false;
	if (!parties.save(out))
		return false;

	int results_phs = 0;
	int results_logs = 0;

	if (results_sum != nullptr)
	{
		int results_phs = parties.get_phs();
		int results_logs = parties.get_logs();
		out.write(rcastcc(&(results_phs)), sizeof(results_phs));
		out.write(rcastcc(&(results_logs)), sizeof(results_logs));

		if (!out.good())
			return false;

		for (int i = 0; i < results_logs; i++)
		{
			out.write(rcastcc(&(results_sum[i])), sizeof(results_sum[i]));
		}
	}

	else
		out.write(rcastcc(&(results_phs)), sizeof(results_phs));

	return out.good();
}
 bool Elections::load(istream& in)
{

	 if (results_sum != nullptr)
	 {
		 delete[] results_sum;
	 }

	 int results_phs;
	 int results_logs;
	 in.read(rcastc(&(results_phs)), sizeof(results_phs));

	 if (!in.good())
		 return false;

	 if (results_phs != 0)
	 {
		 results_sum = new sumOfResult[results_phs];
		 in.read(rcastc(&(results_logs)), sizeof(results_logs));

		 if (!in.good())
			 return false;

		 for (int i = 0; i < results_logs; i++)
		 {
			 in.read(rcastc(&(results_sum[i])), sizeof(results_sum[i]));
		 }
	 }

	 else
		 results_sum = nullptr;

	 return in.good();
}

Elections::~Elections() // Elections destructor
{
	delete[] results_sum;
}


void Elections::set_Year(int year) // set the year that elections occur. Year must be bigger than 1000.
{
		this->year = year;

}

void Elections::set_Day(int day) // set the day that elections occur. 
{

		this->day = day;

}

void Elections::set_Month(int month) // set the month that elections occur.
{

		this->month = month;

}


bool Elections::build_representative_list(DistrictArray& d_array) // init and update the results array when the elections is over. It creates the representatives list for each party according to the results
{														
	float ratio;
	int number_of_representatives;
	int count = 0;
	if (d_array.get_logs() == 0)
	{
		cout << "ERROR: there are no districts" << endl;
		return false;
	}

	if (this->parties.get_logs() == 0)
	{
		cout << "ERROR: there are no parties" << endl;
		return false;
	}
		

	for (int i = 0; i < d_array.get_logs(); i++)
	{
		count = 0;
		for (int j = 0; j < d_array[i]->get_parties_number() - 1; j++)
		{
			if (d_array[i]->get_number_of_voters() == 0)								
				ratio = 0;																
			else
			{
				ratio = static_cast<float>
					(d_array[i]->get_results()[j].votes_number) /
					d_array[i]->get_number_of_voters();
			}

			number_of_representatives = ceil(ratio * static_cast<float>(d_array[i]->get_number_of_representatives())); // up round for number of representatives
			count += number_of_representatives;

			CitizenArray* ca = parties.getParty_by_index(j).get_representatives_by_district();

			if (number_of_representatives > ca[i].get_logs())// there is an error when the party have less representatives than the district gave her
			{
				cout << " ERROR:  party have less representatives than the district gave her" << endl;
				return false;
			} 
				
			int cur = d_array[i]->get_results()[j].elected_representatives.get_logs();
			if (number_of_representatives < cur)
			{
				d_array[i]->get_results()[j].elected_representatives.set_logs(number_of_representatives);
			}
			else
			{
				for (int k = 0; k < number_of_representatives; k++)
					if (ca[i].get_logs() - 1 >= k)
					{
						if (d_array[i]->get_results()[j].elected_representatives.FindCitizenById(ca[i].get_citizen_by_index(k).getId()) == nullptr)
							d_array[i]->get_results()[j].elected_representatives.addCitizen(ca[i].get_citizen_by_index(k));
					}
			}	



		}
		// the last party gets the remain representatives to avoid losing representatives
		int j = d_array[i]->get_parties_number() - 1;
		number_of_representatives = d_array[i]->get_number_of_representatives() - count;
		CitizenArray* ca = parties.getParty_by_index(j).get_representatives_by_district();
		if (number_of_representatives > ca[i].get_logs())
		{
			cout << " ERROR:  party have less representatives than the district gave her" << endl;
			return false;
		}

		int cur = d_array[i]->get_results()[j].elected_representatives.get_logs();
		if (number_of_representatives < cur)
		{
			d_array[i]->get_results()[j].elected_representatives.set_logs(number_of_representatives);
		}
		else
		{
			for (int k = 0; k < number_of_representatives; k++)
				if (ca[i].get_logs() - 1 >= k)
				{
					if (d_array[i]->get_results()[j].elected_representatives.FindCitizenById(ca[i].get_citizen_by_index(k).getId()) == nullptr)
						d_array[i]->get_results()[j].elected_representatives.addCitizen(ca[i].get_citizen_by_index(k));
				}
		}
	}
	return true;
}




