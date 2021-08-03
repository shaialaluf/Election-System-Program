#pragma once
#include "CitizenArray.h"
#include "PartyArray.h"
#include "DistrictArray.h"

static bool is_file_good = true;

class Elections
{

	protected:
		int day;
		int month;
		int year;
		CitizenArray citizens;
		PartyArray parties;
		
		struct sumOfResult {

			int party_id;
			int sum_of_representatives;
			int sum_of_votes;
		};

		sumOfResult* results_sum = nullptr;
		 
		
	

	public:
		Elections(int day, int month, int year);
		Elections(istream& in);
		virtual ~Elections();
		virtual const DistrictArray& get_districts() const = 0;
		const PartyArray& get_parties() const { return parties; }
		const CitizenArray& get_citizens() const { return citizens; }
		virtual DistrictArray& get_districts() = 0;
		PartyArray& get_parties() { return parties; }
		CitizenArray& get_citizens() { return citizens; }
		void set_Year(int year);
		void set_Day(int day);
		void set_Month(int month);

		int get_Year() const { return year; }
		int get_Day() const { return day; }
		int get_Month() const { return month; }

		bool build_representative_list(DistrictArray& d_array);
		virtual bool build_representative_list()=0;
		virtual void init_results_sum()=0;
		virtual void sort_results_sum()=0;
		sumOfResult* get_results_sum() const { return results_sum; }
		virtual void show() const = 0;
		virtual District* get_district_by_id(int dis_num)=0;
		virtual void init_representives() = 0;
		virtual bool save(ostream& out) const;
		virtual bool load(istream& in);

		enum class ElectionsType { REGULAR = 1, SIMPLE };
	};




