#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static bool check_all_done(philo_t* philosophers, config_t* config)
{
	int i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		if (philosophers[i].meals_eaten < config->number_of_times_each_philosopher_must_eat)
		{
			return false;
		}
		i++;
	}
	return true;
}

static void init_forks(pthread_mutex_t* forks, int number_of_philosophers)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

static void create_philosopher_threads(philo_t* philosophers, int number_of_philosophers)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_create(&philosophers[i].thread,
			NULL, philosopher_routine, &philosophers[i]);
		i++;
	}
}

static void join_philosopher_threads(philo_t* philosophers, int number_of_philosophers)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
}

int main(int argc, char** argv)
{
	config_t	config;
	bool		life_flag = true;
	bool		all_done = false;
	philo_t		*philosophers;

	init_config(&config, argc, argv);
	pthread_mutex_t forks[config.number_of_philosophers];
	philosophers = malloc(sizeof(philo_t) * config.number_of_philosophers);
	if (!philosophers)
	{
		perror("malloc");
		return (1);
	}
	init_forks(forks, config.number_of_philosophers);
	init_philosophers(philosophers, forks, &config, &life_flag);
	create_philosopher_threads(philosophers, config.number_of_philosophers);
	while (!all_done)
	{
		all_done = check_all_done(philosophers, &config);
		usleep(1000);
	}
	life_flag = false;
	join_philosopher_threads(philosophers, config.number_of_philosophers);
	cleanup(forks, philosophers, config.number_of_philosophers);
	return (0);
}
