#include "philo.h"

void init_config(config_t* config, int argc, char** argv)
{
	if (argc < 6)
	{
		fprintf(stderr, "Usage: %s n_philo to_die to_eat to_sleep must_eat\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	config->number_of_philosophers = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	config->number_of_times_each_philosopher_must_eat = (argc == 6) ? atoi(argv[5]) : -1;
	config->life_span = 0;
}

void init_philosophers(philo_t* philosophers, pthread_mutex_t* forks,
		config_t* config, bool* life_flag)
{
	int i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		philosophers[i].id = i;
		philosophers[i].meals_eaten = 0;
		philosophers[i].last_meal_time = 0;
		philosophers[i].config = config;
		philosophers[i].life_flag = life_flag;
		philosophers[i].left_fork = &forks[i];
		philosophers[i].right_fork = &forks[(i + 1) % config->number_of_philosophers];
		i++;
	}
}

static bool	has_philosopher_finished_eating(philo_t *philo)
{
	if (philo->config->number_of_times_each_philosopher_must_eat != -1 &&
		philo->meals_eaten >= philo->config->number_of_times_each_philosopher_must_eat)
	{
		printf("Philosopher %d has finished eating %d times and is leaving.\n",
			philo->id, philo->meals_eaten);
		return (true);
	}
	return (false);
}

void	*philosopher_routine(void* arg)
{
	philo_t	*philo;

	philo = (philo_t*)arg;
	while (*(philo->life_flag))
	{
		printf("Philosopher %d is thinking.\n", philo->id);
		usleep(philo->config->time_to_sleep * 1000);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		printf("Philosopher %d is eating.\n", philo->id);
		philo->last_meal_time = time(NULL);
		philo->meals_eaten++;
		usleep(philo->config->time_to_eat * 1000);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		if (has_philosopher_finished_eating(philo))
			break ;
		printf("Philosopher %d is sleeping.\n", philo->id);
		usleep(philo->config->time_to_sleep * 1000);
	}
	return (NULL);
}

void cleanup(pthread_mutex_t* forks, philo_t* philosophers, int num_philosophers)
{
	int i;

	i = 0;
	while (i < num_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(philosophers);
}
