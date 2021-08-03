#pragma once
#include "CitizenArray.h"
#include "PartyArray.h"
#include "DistrictArray.h"
#include "Elections.h"
#include "DividedDistrict.h"
class SimpleElections: public Elections
{

	private:

	DistrictArray district_hidden;


	public:
		SimpleElections(int day, int month, int year, int num_of_representatives);
		virtual ~SimpleElections();
		SimpleElections(istream& in);
		virtual void show() const override { cout << "Simlpe elections" << endl << "-----------------" << endl;}
		void init_results_sum();
		virtual bool build_representative_list ();
		virtual const DistrictArray& get_districts() const { return district_hidden; }
		virtual DistrictArray& get_districts() { return district_hidden; }
		virtual void sort_results_sum();
		virtual void init_representives();
		virtual  District* get_district_by_id(int dis_num) { return district_hidden[0]; }
		virtual bool save(ostream& out) const;
		virtual bool load(istream& in);

	};



