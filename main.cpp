#include <iostream>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include <windows.h>

// AI
#include "GA.h"
#include "Environment.h"
#include "Player.h"

using namespace AI;
using namespace std;


#define VELKOST_POPULACIE						  (128) 	// pocet jedincov
#define POCET_GENOV								    (2)
#define POCET_KROKOV							  (100)
#define POCET_GENERACII						   (1000000)

int main(int argc, char** argv)
{
	Player _robot;
	Environment* _env = new Environment();
	int done;
	float best_duration;
	int best_is_end;
	FILE *f_fitness;

	f_fitness = fopen("fitness.txt", "w");

	//  AI
	GA* _ai = new GA(VELKOST_POPULACIE, (_env->getWidth()*_env->getHeight()), POCET_GENOV);

	// init random
	srand((unsigned)time(NULL));

	int startPositionsX[3] = { 0, 9, 0 };
	int startPositionsY[3] = { 1, 1, 5 };

	//  Training agent's brain
	for (int generation = 0; generation <= POCET_GENERACII; generation++)
	{
		// clear fitness
		_ai->clearFitness();

		for (int individual_index = 0; individual_index < VELKOST_POPULACIE; individual_index++)
		{
			// pociatocny stav
			auto idx = rand() % 3;
			_robot.Move(startPositionsX[idx], startPositionsY[idx]);

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

		// Najdi najlepsieho jedinca (jeho vlastnosti sa nebudu menit)
		_ai->updateBest();

		// Krizenie dvoch rodicov
		_ai->Crossover();

		// Mutovanie genov
		_ai->Mutation();
	}

	//  Testing agent 
	for (int time, step = 0; time < 1000; time++)
	{
		auto t_start = std::chrono::high_resolution_clock::now(); 

		// clear fitness
		_ai->clearFitness();
		best_is_end = 0;

		// pociatocny stav
		auto idx = rand() % 3;
		_robot.Move(startPositionsX[idx], startPositionsY[idx]);

		// Nakresli herny svet
		//_env->NakresliHernySvet(_robot.getX(), _robot.getY());

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

			//std::cout << "Generation: " << time << std::endl;
			//std::cout << "Individual: " << _ai->getBest() << std::endl;
			//std::cout << "*	 robot.X = " << _robot.getX() << std::endl;
			//std::cout << "*	 robot.Y = " << _robot.getY() << std::endl;
			//std::cout << "*	 action = " << _ai->getGeneOfIndividual(_ai->getBest(), _env->getState(_robot.getX(), _robot.getY())) << std::endl;
			//std::cout << "*	 step = " << step << std::endl;
			//std::cout << "*	 done = " << done << std::endl;
			//std::cout << "*	 fitness = " << _ai->getFitnessOfIndividual(_ai->getBest()) << std::endl;
			//std::cout << "*	 mutation_rate = " << _ai->getMutationRate() << std::endl;
			//std::cout << "*	 time = " << best_duration << " us" << std::endl;

			// ak konec hry
			if (done != 0) break;
		}

		auto t_stop = std::chrono::high_resolution_clock::now(); 
		// floating-point duration: no duration_cast needed
    	std::chrono::duration<float, std::milli> fp_ms = t_stop - t_start;
		best_duration = fp_ms.count() * 1000;

		fprintf(f_fitness, "%d;%f;%d;%f;%d\n", time, _ai->getFitnessOfIndividual(_ai->getBest()), step, best_duration, best_is_end);
	}

	fclose(f_fitness);

	return 0;
}