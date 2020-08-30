// MazeSolver_v3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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


#define VELKOST_POPULACIE						 (1000) 	// pocet jedincov
#define POCET_GENOV								    (2)
#define POCET_KROKOV							  (100)
#define POCET_GENERACII						   (100000)

int main(int argc, char** argv)
{
	Player _robot;
	Environment* _env = new Environment();
	int done;
	//int x1, y1;
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
					done = 1;
				}
				else
					// Zisti odmenu za akciu a[t]
					_robot.setReward(_env->getReward(_robot.getX(), _robot.getY(), &done));

				// Zaznamenaj odmenu do celkovej sily jedinca
				_ai->setFitnessOfIndividual(individual_index, _robot.getReward());

				if ((generation % 100 == 0) && (individual_index == _ai->getBest()))
				{
					// Nakresli herny svet
					_env->NakresliHernySvet(_robot.getX(), _robot.getY());

					std::cout << "Generation: " << generation << std::endl;
					std::cout << "Individual: " << individual_index << std::endl;
					std::cout << "*	 robot.X = " << _robot.getX() << std::endl;
					std::cout << "*	 robot.Y = " << _robot.getY() << std::endl;
					std::cout << "*	 action = " << _ai->getGeneOfIndividual(individual_index, _env->getState(_robot.getX(), _robot.getY())) << std::endl;
					std::cout << "*	 step = " << step << std::endl;
					std::cout << "*	 done = " << done << std::endl;
					std::cout << "*	 fitness = " << _ai->getFitnessOfIndividual(individual_index) << std::endl;
					std::cout << "*	 mutation_rate = " << _ai->getMutationRate() << std::endl;
				}

				// ak konec hry
				if (done != 0) break;
			}
		}

		auto score = _ai->getFitnessOfIndividual(_ai->getBest());
		if (generation % 100 == 0 && score > -50.0)
			fprintf(f_fitness, "%f\n", score);

		// Najdi najlepsieho jedinca (jeho vlastnosti sa nebudu menit)
		_ai->updateBest();

		// Krizenie dvoch rodicov
		_ai->Crossover();

		// Mutovanie genov
		_ai->Mutation();
	}

	// Pause
	//system("pause");

	//  Testing agent 
	/*for (int time = 0; time < 5; time++)
	{
		// pociatocny stav
		auto idx = rand() % 3;
		_robot.Move(startPositionsX[idx], startPositionsY[idx]);

		// Nakresli herny svet
		_env->NakresliHernySvet(_robot.getX(), _robot.getY());

		do {
			// Vykonaj akciu
			_robot.Move(_ai->getGeneOfIndividual(_ai->getBest(), _env->getState(_robot.getX(), _robot.getY())));

			// Zisti odmenu za akciu a[t]
			_env->getReward(_robot.getX(), _robot.getY(), &done);

			// casova medzera pre usera
			this_thread::sleep_for(std::chrono::milliseconds(250));
			_env->NakresliHernySvet(_robot.getX(), _robot.getY());

			std::cout << "robot.X = " << _robot.getX() << std::endl;
			std::cout << "robot.Y = " << _robot.getY() << std::endl;
			std::cout << "action = " << _ai->getGeneOfIndividual(_ai->getBest(), _env->getState(_robot.getX(), _robot.getY())) << std::endl;

		} while (done != -1);
	}

	// Pause
	system("pause");*/

	fclose(f_fitness);

	return 0;
}