#pragma once

#include <iostream>
#include <math.h>

#include "Individual.h"

namespace AI
{
	class GA
	{
	private:
		float mutation_rate;
		int population_size;

		Individual** populations;
		int best;
	public:
		GA(int _population_size, int _chromosome_size, int _gene_set);
		~GA();

		void Crossover();
		void Mutation();
		void clearFitness();
		int getGeneOfIndividual(int idxIndiv, int idxGene);
		void setFitnessOfIndividual(int idxIndiv, float reward);
		float getFitnessOfIndividual(int idxIndiv);
		void updateBest();
		int getBest();
		int RouletteWheelMechanism(int* idxA, int* idxB);
	};

	GA::GA(int _population_size, int _chromosome_size, int _gene_set)
	{
		this->population_size = _population_size;

		this->populations = new Individual*[_population_size];
		for (int i = 0; i < this->population_size; i++)
			this->populations[i] = new Individual(_chromosome_size, _gene_set);

		this->mutation_rate = 0.20f;	// 20%

		// 0. generacia nema este hodnotenie
		this->best = 0;
	}

	void GA::Crossover()
	{
		int slicing_point, _parrentA=0, _parrentB=0;
		int i, j;

		for (i = 0; i < this->population_size; i++)
		{
			if (i != this->best)
			{
				// Random pick 2 parents
				// Vyber
				RouletteWheelMechanism(&_parrentA, &_parrentB);

				//parrentA = (rand() % this->population_size);
				//parrentB = (rand() % this->population_size);

				// Random slicing point
				slicing_point = (rand() % this->populations[i]->getChromosomeSize());

				// first part of the child's chromosome contains the parrentA genes
				for (j = 0; j < slicing_point; j++)
				{
					this->populations[i]->setGene(j, this->populations[_parrentA]->getGene(j));
				}

				// second part of the child's chromosome contains the parrentB genes
				for (j = slicing_point; j < this->populations[i]->getChromosomeSize(); j++)
				{
					this->populations[i]->setGene(j, this->populations[_parrentB]->getGene(j));
				}
			}
		}
	}

	void GA::Mutation()
	{
		int i, j;

		for (i = 0; i < this->population_size; i++)
		{
			if (i != this->best)
			{
				for (j = 0; j < this->populations[i]->getChromosomeSize(); j++)
				{
					if (((float)rand() / (float)RAND_MAX) < this->mutation_rate)
					{
						this->populations[i]->setGene(j, (rand() % (1 << this->populations[i]->getGeneSet())));
					}
				}
			}
		}
	}

	void GA::clearFitness()
	{
		int i;

		for (i = 0; i < this->population_size; i++)
		{
			this->populations[i]->clrFitness();
		}
	}

	int GA::getGeneOfIndividual(int idxIndiv, int idxGene)
	{
		return this->populations[idxIndiv]->getGene(idxGene);
	}

	void GA::setFitnessOfIndividual(int idxIndiv, float reward)
	{
		this->populations[idxIndiv]->setFitness(reward);
	}

	float GA::getFitnessOfIndividual(int idxIndiv)
	{
		return this->populations[idxIndiv]->getFitness();
	}

	void GA::updateBest()
	{
		int i;

		this->best = 0;
		for (i = 1; i < this->population_size; i++)
		{
			if (this->populations[i]->getFitness() > this->populations[this->best]->getFitness())
				this->best = i;
		}
	}

	int GA::getBest()
	{
		return this->best;
	}

	int GA::RouletteWheelMechanism(int* idxA, int* idxB)
	{
		float* probability = new float[this->population_size];
		float sum = 0.0;

		// Softmax function
		/*********************************************************/
		for (int i = 0; i < this->population_size; i++)
		{
			sum += exp(this->populations[i]->getFitness());
		}
		for (int i = 0; i < this->population_size; i++)
		{
			probability[i] = exp(this->populations[i]->getFitness()) / sum;
			//std::cout << "*	probability = " << probability[i] << std::endl;
			//std::cout << "fitness = " << this->populations[i]->getFitness() << std::endl;
		}
		//std::cout << std::endl;
		/*********************************************************/

		// Pseudorandom selection
		/*********************************************************/
		float a = ((float)rand() / (float)RAND_MAX);
		float b = ((float)rand() / (float)RAND_MAX);

		float min = 0, max = probability[0];
		for (int i = 0; i < (this->population_size-1); i++)
		{
			// if is a value in range
			if (min <= a && a < max)
			{
				//std::cout << "a = " << a << std::endl;
				//std::cout << "interval = <" << min << "; " << max << ")" << std::endl;
				//std::cout << "index_of_selectedA = " << i << std::endl;
				//std::cout << "probability_of_selectedA = " << probability[i] << std::endl;
				//std::cout << "fitness_of_selectedA = " << this->populations[i]->getFitness() << std::endl;
				//break;

				*idxA = i;
			}

			// if is b value in range
			if (min <= b && b < max)
			{
				//std::cout << "b = " << b << std::endl;
				//std::cout << "interval = <" << min << "; " << max << ")" << std::endl;
				//std::cout << "index_of_selectedB = " << i << std::endl;
				//std::cout << "probability_of_selectedB = " << probability[i] << std::endl;
				//std::cout << "fitness_of_selectedB = " << this->populations[i]->getFitness() << std::endl;
				//break;

				*idxB = i;
			}

			min = max;
			max += probability[i+1];
		}
		//std::cout << std::endl;

		return 0;
	}

	GA::~GA()
	{
		delete populations;
	}
}