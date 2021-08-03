#pragma once
#include "CitizenArray.h"
#include "PartyArray.h"
#include "DistrictArray.h"
#include "Elections.h"
class RegularElections: public Elections
{
	private:
		DistrictArray districts;
	
	public:
		RegularElections(int day, int month, int year);
		RegularElections(istream& in);
		virtual ~RegularElections();
		virtual const DistrictArray& get_districts() const { return districts; }
		virtual DistrictArray& get_districts() { return districts; }
		void init_results_sum();
		virtual void show() const override { cout << "Regular elections" << endl<<"-----------------" <<endl; }
		virtual bool build_representative_list();
		virtual void sort_results_sum();
		virtual void init_representives();
		virtual District* get_district_by_id(int dis_num) { return districts[dis_num]; }
		virtual bool save(ostream& out) const;
		virtual bool load(istream& in);
};

