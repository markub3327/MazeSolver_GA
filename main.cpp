#include <omp.h>
#include <math.h>
#include <time.h>

#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream> 
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <vector>

// AI
#include "GA.h"
#include "Environment.h"
#include "Player.h"
#include "Vector2.hpp"

using namespace AI;
using namespace std;

// definicie prostredia
#define VELKOST_POPULACIE						   (128)
#define POCET_GENOV								     (2)
#define POCET_KROKOV							   (100)
#define POCET_GENERACII						   (1000000)

// global variables
vector<Vector2*> start_positions;
Environment* _env = new Environment();
GA* _ai = new GA(VELKOST_POPULACIE, (_env->getWidth()*_env->getHeight()), POCET_GENOV);

// fn
void DoProgress(const char *label, int step, int total);
void trainig_loop(int individual_index);
void testing_loop(int t, FILE *f);
void read_start_points();

int main(int argc, char** argv)
{
	start_positions.push_back(new Vector2(0, 1));
	start_positions.push_back(new Vector2(9, 1));
	start_positions.push_back(new Vector2(0, 5));

	// init random
	srand((unsigned)time(NULL));

	//  Training agent's brain
	for (int generation = 0; generation < POCET_GENERACII; generation++)
	{
		// clear fitness
		_ai->clearFitness();

		// bar
		DoProgress("Training: ", generation, POCET_GENERACII);

		#pragma omp parallel for
			for (int individual_index = 0; individual_index < VELKOST_POPULACIE; individual_index++)
			{
				//cout << "Individual: " << individual_index << ", Thread ID: " << omp_get_thread_num() << endl;
				trainig_loop(individual_index);
			}

		// Najdi najlepsieho jedinca (jeho vlastnosti sa nebudu menit)
		_ai->updateBest();

		// Krizenie dvoch rodicov
		_ai->Crossover();

		// Mutovanie genov
		_ai->Mutation();
	}

	read_start_points();
	
	// statistics
	FILE *f_fitness = fopen("statistics.txt", "w");
	
	//  Testing agent
	for (int time = 0; time < 20; time++)
	{
		testing_loop(time, f_fitness);
	}

	fclose(f_fitness);
	
	return 0;
}

void DoProgress(const char *label, int step, int total)
{
    // progress width
    const int pwidth = 72;

    // calc %    
    float percent = (float)step / (float)total;
 
    // minus label len
    int width = pwidth - strlen(label);
    int pos = (int) round(percent * width);
 
    // fill progress bar with =
    cout << label << " [";
    for (int i = 0; i < pos - 1; i++)  
        cout << "=";
    cout << ">>";
 
    //fill progress bar with spaces
  	cout << std::setfill(' ') << std::setw(width - pos + 1);
	cout << "] ";
	cout << std::fixed;
    cout << std::setprecision(2);
    cout << percent * 100.0f;
	cout << "%\r";
}

void trainig_loop(int individual_index)
{
	Player _robot;
	int done;

	// pociatocny stav
	auto idx = rand() % 3;
	_robot.Move(start_positions[idx]->x, start_positions[idx]->y);

	for (int step = 0; step < POCET_KROKOV; step++)
	{
		auto oldX = _robot.getX();
		auto oldY = _robot.getY();
				
		// Vykonaj akciu
		_robot.Move(_ai->getGeneOfIndividual(individual_index, _env->getState(_robot.getX(), _robot.getY())));

		if (_robot.getX() < 0 || _robot.getX() > (_env->getWidth()-1) || _robot.getY() < 0 || _robot.getY() > (_env->getHeight()-1))
		{
			_robot.setReward(-50.0);
			_robot.Move(oldX, oldY);
			done = -1;
		}
		else
			// Zisti odmenu za akciu a[t]
			_robot.setReward(_env->getReward(_robot.getX(), _robot.getY(), &done));

		// Zaznamenaj odmenu do celkovej sily jedinca
		_ai->setFitnessOfIndividual(individual_index, _robot.getReward());

		// ak konec hry
		if (done != 0) break;
	}
}

void testing_loop(int t, FILE *f)
{
	Player _robot;
	int done, step, best_is_end = 0;

	auto t_start = chrono::high_resolution_clock::now(); 

	// clear fitness
	_ai->clearFitness();

	// pociatocny stav
	_robot.Move(start_positions[t]->x, start_positions[t]->y);

	for (step = 0; step < POCET_KROKOV; step++)
	{
		auto oldX = _robot.getX();
		auto oldY = _robot.getY();

		// Vykonaj akciu
		_robot.Move(_ai->getGeneOfIndividual(_ai->getBest(), _env->getState(_robot.getX(), _robot.getY())));

		if (_robot.getX() < 0 || _robot.getX() > (_env->getWidth()-1) || _robot.getY() < 0 || _robot.getY() > (_env->getHeight()-1))
		{
			_robot.setReward(-50.0);
			_robot.Move(oldX, oldY);
			done = -1;
		}
		else
			// Zisti odmenu za akciu a[t]
			_robot.setReward(_env->getReward(_robot.getX(), _robot.getY(), &done));

		// Zaznamenaj odmenu do celkovej sily jedinca
		_ai->setFitnessOfIndividual(_ai->getBest(), _robot.getReward());

		// nasiel vychod
		if (done == 1)
			best_is_end = 100;

		// ak konec hry
		if (done != 0) break;
	}

	auto t_stop = chrono::high_resolution_clock::now(); 
	// floating-point duration: no duration_cast needed
    chrono::duration<float, milli> fp_ms = t_stop - t_start;

	fprintf(f, "%d;%f;%d;%f;%d\n", t, _ai->getFitnessOfIndividual(_ai->getBest()), step, (fp_ms.count() * 1000), best_is_end);
}

void read_start_points()
{
	// read log file
	fstream log_startPos;

	start_positions.clear();

	log_startPos.open("log_start.txt", ios::in);
	if (log_startPos.is_open())
	{
		string line;
		string intermediate;

		while (getline(log_startPos, line))
		{
			stringstream stream1(line);

			getline(stream1, intermediate, ';');
			auto x = atof(intermediate.c_str());

			getline(stream1, intermediate, ';');
			auto y = atof(intermediate.c_str());

			start_positions.push_back(new Vector2(x, y));
		}

		log_startPos.close();
	}
	else
	{
		cout << "Error in reading file.\n";
		exit(-1);
	}
}
