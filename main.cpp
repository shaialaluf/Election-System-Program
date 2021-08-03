
#include "CitizenArray.h"
#include "PartyArray.h"
#include "DistrictArray.h"
#include "UnitedDistrict.h"
#include "DividedDistrict.h"
#include "RegularElections.h"
#include "SimpleElections.h"
#include <iostream>
#include <fstream>
using namespace std;


void main()
{
	int main_choice;
	int choice;
	Elections::ElectionsType election_choice;
	int election_ch_num;
	int day, month, year;
	const int MAX = 100;
	char file_name[MAX];

	ifstream infile; // load elections
	ofstream outfile; // save elections
	bool is_succeeded;
	Elections* elections=nullptr;
	cout << "Please choose a number between 1-3: "<<endl;
	cout << "1. new elections round " << endl;
	cout << "2. load elections from file " << endl;
	cout << "3. exit " << endl;
	cin >> main_choice; 

	if (main_choice == 1) // pre menu
	{
		cout << "Please enter date of the elections by day, month, year: ";
		cin >> day >> month >> year;
		cout << endl;
		cout << "Please enter the type of the elections: 1-for regular, 2-for simple: ";
		cin >> election_ch_num;
		cout << endl;
		election_choice = static_cast<Elections::ElectionsType>(election_ch_num);

		if (election_choice == Elections::ElectionsType::REGULAR)
			elections = new RegularElections(day, month, year);

		else if (election_choice == Elections::ElectionsType::SIMPLE)
		{
			int num_of_rep;
			cout << "Please enter number of representatives for the simple elections: " << endl;
			cin >> num_of_rep;
			while (num_of_rep <= 0)
			{
				cout << "ERROR: number of representatives cannot be 0 or negative!" << endl;
				cout << "please enter positive number: ";
				cin >> num_of_rep;
			}

			elections = new SimpleElections(day, month, year, num_of_rep);
			cout << endl;
		}
	}

	else if (main_choice == 2)
	{
		cout << "Please enter file name to load from: ";
		cin >> file_name;
		infile.open(file_name, ios::binary | ios::_Nocreate);
		if (!infile)
		{
			cout << "Error with opening infile" << endl;
			exit(1);
		}

		else
		{
			Elections::ElectionsType type;
			infile.read(rcastc(&type), sizeof(type));
			if (!infile.good())
			{
				cout << "Error with reading from infile" << endl;
				infile.close();
				exit(1);
			}

			if (type == Elections::ElectionsType::REGULAR)
			{

				elections = new RegularElections(0,0,0);
				election_choice = Elections::ElectionsType::REGULAR;

			}

			else if (type == Elections::ElectionsType::SIMPLE)
			{
				elections = new SimpleElections(0, 0, 0,0);
				election_choice = Elections::ElectionsType::SIMPLE;

			}

			else
			{
				cout << "Error with reading from infile" << endl;
				infile.close();
				exit(1);
			}

			is_succeeded = elections->load(infile);
			if (!is_succeeded)
			{
				cout << "Error with reading from infile" << endl;
				delete elections;
				exit(1);
			}

			infile.close();
		}
	}

	else if (main_choice == 3)
	{
		cout << "GOODBYE" << endl;
		exit(0);
	}

	else
	{
		cout << "ERROR: wrong input!" << endl;
		exit(1);
	}


	
	int birth_year;
	char name[MAX];

	int number_of_representatives;
	Citizen* representative;
	Citizen* chief;
	Citizen* citizen_p;
	Citizen citizen;
	District* Citizen_Home;
	int party_id, district_id, citizen_id;

	int district_type;



	cout << "Please choose a number between 1-10:" << endl; //// main menu
	cout << "1. Add district" << endl;
	cout << "2. Add citizen" << endl;
	cout << "3. Add party" << endl;
	cout << "4. Add representive to a party" << endl;
	cout << "5. Show all the districts at the country" << endl;
	cout << "6. Show all the citizens at the country" << endl;
	cout << "7. Show all the parties at the country " << endl;
	cout << "8. Make a vote" << endl;
	cout << "9. Show the elections results" << endl;
	cout << "10. Exit program" << endl;
	cout << "11. Save elections to file" << endl;
	cout << "12. Load elections from file" << endl;
	cin >> choice;
	while (choice != 10)
	{
	
		switch (choice) {
			
		case 1:
			if (election_choice == Elections::ElectionsType::REGULAR)
			{
				cout << "Please enter district name, district type: 0-for united, 1-for divided. enter also number of representatives: ";
				cin >> name >> district_type >> number_of_representatives;

				if (number_of_representatives >= 0)
				{

					if (district_type == 0)
					{
						UnitedDistrict district(name, number_of_representatives, &(elections->get_parties()));
						elections->get_districts().addDistrict(&district);
					}
					else if (district_type == 1)
					{
						DividedDistrict district(name, number_of_representatives, &(elections->get_parties()));
						elections->get_districts().addDistrict(&district);
					}

					elections->get_parties().add_district_to_parties();
				}
			}

			else if(election_choice == Elections::ElectionsType::SIMPLE)
			{
				cout << "error: This is simple elections. There are no districts!" << endl;
			}
			
			break;

		case 2:
				cout << "Please enter name, id, birth year and district num: ";
				cin >> name >> citizen_id >> birth_year >> district_id;
				if (election_choice == Elections::ElectionsType::SIMPLE)
					district_id = 0;
				Citizen_Home = elections->get_districts()[district_id];
				citizen = Citizen(name, citizen_id, birth_year, Citizen_Home, false);
				if (citizen.getBirth_Year() == -1)
					cout << "invalid birth year " << endl;
				else if (citizen.getId() == -2)
					cout << "invalid id" << endl;

				else if (election_choice == Elections::ElectionsType::REGULAR && elections->get_districts().get_logs() - 1 < district_id)
						cout << "invalid district num " << endl;
				else
				{
					elections->get_citizens().addCitizen(citizen); // add to pinkas
					elections->get_districts()[citizen.getDistrict_Citizen()->get_serial_number()]->addCitizenToDistrict(citizen);// add citizen inside district

					elections->get_districts()[citizen.getDistrict_Citizen()->get_serial_number()]->set_percent_of_all();
				}
			
			
			break;

		case 3:
			cout << "Please enter name of the party, chief party id:";
			cin >> name>>citizen_id;
			chief = elections->get_citizens().FindCitizenById(citizen_id);

			if (elections->get_parties().is_citizen_chief(citizen_id))
				cout << "This citizen is already a chief" << endl;

			else if (chief != nullptr)
			{
				elections->get_parties().addParty(name, *chief, &(elections->get_districts()));
				elections->get_districts().add_party_to_districts();
			}

			else
				cout << "This is not a citizen!" << endl;
			break;

		case 4:
			cout << "Please enter id of representative, party id, district id:";
			cin >> citizen_id >> party_id >> district_id;
	
			representative = elections->get_citizens().FindCitizenById(citizen_id);
			if (representative == nullptr)
				cout << "This is not a citizen!" << endl;
			else if (!elections->get_parties().Is_Representative(citizen_id))
				cout << "This citizen is already representative or chief" << endl;

			else if (elections->get_parties().get_logs() < party_id + 1)
				cout << "invalid party id" << endl;
			
			else if ((election_choice == Elections::ElectionsType::REGULAR) && (elections->get_districts().get_logs() < district_id + 1))
					cout << "invalid district id" << endl;

			else
			{
				if (election_choice == Elections::ElectionsType::SIMPLE)
					district_id = 0;

					elections->get_parties().getParty_by_index(party_id).addRepresentative(*representative, district_id);

			}
			break;

		case 5:
			
			if (election_choice == Elections::ElectionsType::SIMPLE)
				cout << "There are no districts in simple elections" << endl;

			else if (election_choice == Elections::ElectionsType::REGULAR)
			{
				cout << "The districts are:" << endl;
				cout << elections->get_districts();
			}
			break;

		case 6:
			cout << "The citizens are:" << endl;
			cout << elections->get_citizens();
			break;

		case 7:
			cout << "The parties are:" << endl;
			cout << elections->get_parties();
			break;

		case 8:
			cout << "Please enter your id and party id: ";
			cin >> citizen_id >> party_id;
			citizen_p = elections->get_citizens().FindCitizenById(citizen_id);
			if (citizen_p == nullptr)
				cout << "This is not a citizen!" << endl;

			else if (citizen_p->getIs_Voted())
				cout << "citizen already voted" << endl;

			else if (elections->get_parties().get_logs() < party_id + 1)
				cout << "invalid party id" << endl;

			else
			{
				citizen_p->set_isVoted(true);
				elections->get_districts()[citizen_p->getDistrict_Citizen()->get_serial_number()]->set_results(party_id);
				elections->get_districts()[citizen_p->getDistrict_Citizen()->get_serial_number()]->set_percent_of_all();

			}

			break;

		case 9:

			cout << "Date of the elections:  " << elections->get_Day() << "/" << elections->get_Month() << "/" << elections->get_Year() << endl;
			if (!elections->build_representative_list())
				cout << "cant calculate results"<<endl;

			else {
				elections->show();
				if (election_choice == Elections::ElectionsType::REGULAR)
				{
					for (int i = 0; i < elections->get_districts().get_logs(); i++)
					{
						cout << endl;
						cout << "district name: ";
						cout << elections->get_districts()[i]->get_name() << endl;
						cout << "district type: ";
						elections->get_districts()[i]->show();
						cout << endl;
						cout << "number_of_representatives:";
						cout << elections->get_districts()[i]->get_number_of_representatives() << endl;
						elections->get_districts()[i]->calc_winner_party(); 
						if (typeid(*(elections->get_districts()[i])) == typeid(UnitedDistrict))
						{
							int winner_party = dynamic_cast<UnitedDistrict*>(elections->get_districts()[i])->get_winner_party();
							cout << "name of the chief party that won in the district: ";
							cout << elections->get_parties().getParty_by_index(winner_party).get_chief_party().getName() << endl;
						}
						else if (typeid(*(elections->get_districts()[i])) == typeid(DividedDistrict))
						{
							for (int k = 0; k < elections->get_districts()[i]->get_results_size()->get_logs(); k++)
							{
								int number_of_representatives = dynamic_cast<DividedDistrict*>(elections->get_districts()[i])->get_sorted_results()[k].party_res.elected_representatives.get_logs();
								int party_id = dynamic_cast<DividedDistrict*>(elections->get_districts()[i])->get_sorted_results()[k].party_id;
								if (number_of_representatives > 0)
								{
									cout << "chief party: " << elections->get_parties().getParty_by_index(party_id).get_chief_party().getName() << endl;
									cout << "number of representatives that party took: " << number_of_representatives << endl;
								}

							}
						}

						cout << "the precent of all the voters in the district " << i << " is : ";
						elections->get_districts()[i]->set_percent_of_all();
						cout << elections->get_districts()[i]->get_percent_of_all() << endl;

						for (int j = 0; j < elections->get_districts()[i]->get_parties_number(); j++)
						{
							cout << "the elected representatives of party number " << j << " are: ";
							cout << elections->get_districts()[i]->get_results()[j].elected_representatives << endl;
							cout << "number of votes for party number " << j << " is: ";
							cout << elections->get_districts()[i]->get_results()[j].votes_number << endl;
							cout << "precent of votes for party number " << j << " is: ";

							int percent = 100 * (elections->get_districts()[i]->get_results()[j].votes_number) /
								elections->get_districts()[i]->get_number_of_voters();

							cout << percent << endl;

						}

					}
				}

					elections->init_results_sum();
					elections->sort_results_sum();

					if (election_choice == Elections::ElectionsType::SIMPLE)
					{
						cout << "the precent of all the voters:  ";
						elections->get_districts()[0]->set_percent_of_all();
						cout << elections->get_districts()[0]->get_percent_of_all() << endl;
					}

					int party_index;


					for (int i = 0; i < elections->get_parties().get_logs(); i++)
					{

						party_index = elections->get_results_sum()[i].party_id;
						cout << endl;
						cout << "The name of the party: " << elections->get_parties().getParty_by_index(party_index).get_name() << endl;
						cout << "The name of the chief: " << elections->get_parties().getParty_by_index(party_index).get_chief_party().getName() << endl;//
						cout << "sum of representatives: " << elections->get_results_sum()[i].sum_of_representatives << endl;
						cout << "sum of votes: " << elections->get_results_sum()[i].sum_of_votes << endl;
						cout << endl;
						
						if (election_choice == Elections::ElectionsType::SIMPLE)
						{
							cout << "precent of votes for the party : ";
							int percent = (100 * elections->get_results_sum()[i].sum_of_votes) / elections->get_districts()[0]->get_number_of_voters();

							cout << percent << endl;

							cout << "The elected Representatives of the party: " << endl;
							cout << elections->get_districts()[0]->get_results()[elections->get_results_sum()[i].party_id].elected_representatives << endl;;
						}
					}
				

				
					

			
			}
			break;

		case 10:
			exit(0);
			break;

		case 11:
		{
			cout << "Please enter file name: ";
			cin >> file_name;
			outfile.open(file_name, ios::binary|ios::trunc);
			if (!outfile)
			{
				cout << "Error with opening outfile. didn't save to file"<<endl;
				exit(1);
			}
			else
			{
				if (!elections->save(outfile))
				{
					cout << "Error with saving to file" << endl;
					outfile.close();
					delete elections;
					exit(1);
				}
				outfile.close();
			}
				
		}
			break;

		case 12:
		{
			cout << "Please enter file name: ";
			cin >> file_name;
			infile.open(file_name, ios::binary | ios::_Nocreate);
			if (!infile)
			{
				cout << "Error with opening infile. didn't load from file" << endl;
				exit(1);
			}
			else
			{
				delete elections;
				
				Elections::ElectionsType type;
				infile.read(rcastc(&type), sizeof(type));
				if (!infile.good())
				{
					cout << "Error with loading from file" << endl;
					infile.close();
					exit(1);
				}

				if (type == Elections::ElectionsType::REGULAR)
				{
					elections = new RegularElections(0, 0, 0);
					election_choice = Elections::ElectionsType::REGULAR;
				}

				else if (type == Elections::ElectionsType::SIMPLE)
				{
					elections = new SimpleElections(0, 0, 0, 0);
					election_choice = Elections::ElectionsType::SIMPLE;
				}

				if (!elections->load(infile))
				{
					cout << "Error with loading from file" << endl;
					infile.close();
					delete elections;
					exit(1);
				}

				infile.close();
		
			}
				
		}
			break;

		default:
			cout << "There is no option like this " << endl;
			break;			
		}
		cout << endl;
		cout << "Please choose a number between 1-10:" << endl;
		cout << "1. Add district" << endl;
		cout << "2. Add citizen" << endl;
		cout << "3. Add party" << endl;
		cout << "4. Add representive to a party" << endl;
		cout << "5. Show all the districts at the country" << endl;
		cout << "6. Show all the citizens at the country" << endl;
		cout << "7. Show all the parties at the country " << endl;
		cout << "8. Make a vote" << endl;
		cout << "9. Show the elections results" << endl;
		cout << "10. Exit program" << endl;
		cout << "11. Save elections to file" << endl;
		cout << "12. Load elections from file" << endl;
		cin >> choice;

	}
	delete elections;
	cout << "goodbye" << endl;
}
