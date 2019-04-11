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


#define VELKOST_POPULACIE						  (64)	// pocet jedincov
#define POCET_GENOV								   (2)
#define POCET_KROKOV							  (40)
#define POCET_GENERACII						   (10000)

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

	/*for (;;)
	{
		_env->genRand(2);

		//_env->findLeaf(&x1, &y1);
		//_robot.Move(x1, y1);

		_env->NakresliHernySvet(_robot.getX(), _robot.getY());
		this_thread::sleep_for(std::chrono::milliseconds(1000));
	}*/

	// Main loop
	//for (;;)
	//{
		//  Training agent's brain
	for (int generation = 0; generation <= POCET_GENERACII; generation++)
	{
		// clear fitness
		_ai->clearFitness();

		for (int individual_index = 0; individual_index < VELKOST_POPULACIE; individual_index++)
		{
			// nove kolo hry
			done = 0;

			// init robot object	
			if (individual_index == _ai->getBest())
				_robot.setPenalties(1);

			// pociatocny stav
			_robot.Move(2, 2);

			for (int step = 0; step < POCET_KROKOV; step++)
			{
				// Vykonaj akciu
				_robot.Move(_ai->getGeneOfIndividual(individual_index, _env->getState(_robot.getX(), _robot.getY())));

				// Zisti odmenu za akciu a[t]
				_robot.setReward(_env->getReward(_robot.getX(), _robot.getY(), &done));
				if (individual_index == _ai->getBest())
					_robot.setPenalties(_robot.getPenalties() + done);

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
					std::cout << "*	 penalties_best = " << _robot.getPenalties() << std::endl;
				}

				// ak konec hry
				if (done != 0) break;
			}
		}

		//if (generation % 10 == 0)
		for (int i = 0; i < VELKOST_POPULACIE; i++)		
		{
			//std::cout << "fitness = " << _ai->getFitnessOfIndividual(_ai->getBest()) << std::endl;
			fprintf(f_fitness, "%f;", _ai->getFitnessOfIndividual(i));
		}
		fprintf(f_fitness, "\n");

		// Najdi najlepsieho jedinca (jeho vlastnosti sa nebudu menit)
		_ai->updateBest();

		// ukonci trening ak najdes ciel
		if (_robot.getPenalties() == 0)
			break;

		// Krizenie dvoch rodicov
		_ai->Crossover();

		// Mutovanie genov
		_ai->Mutation();
	}

	// Pause
	system("pause");

	//  Testing agent 
	for (int time = 0; time < 5; time++)
	{
		// pociatocny stav
		_robot.Move(2, 2);

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
	system("pause");

	//_env->genRand(10);
//}

	fclose(f_fitness);

	return 0;
}