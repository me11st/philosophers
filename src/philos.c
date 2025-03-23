/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:18:17 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/23 13:18:21 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_config(t_config *config, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		fprintf(stderr, "Usage: %s n_philo to_die to_eat to_sleep [times_to_eat]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	config->number_of_philosophers = atoi(argv[1]);
	config->time_to_die = atoi(argv[2]);
	config->time_to_eat = atoi(argv[3]);
	config->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		config->not_to_eat = atoi(argv[5]);
	else if (argc == 5)
		config->not_to_eat = -1;
	if (config->number_of_philosophers <= 0 || config->time_to_die <= 0 ||
		config->time_to_eat <= 0 || config->time_to_sleep <= 0 ||
		(argc == 6 && config->not_to_eat <= 0))
	{
		fprintf(stderr, "Error: All input values must be positive integers.\n");
		exit(EXIT_FAILURE);
	}
	config->life_span = 0;
}

void	init_philosophers(t_philo *philosophers, pthread_mutex_t *forks,
			t_config *config, bool *life_flag)
{
	int	i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		philosophers[i].id = i + 1;
		philosophers[i].meals_eaten = 0;
		philosophers[i].last_meal_time = get_time();
		philosophers[i].config = config;
		philosophers[i].life_flag = life_flag;
		philosophers[i].left_fork = &forks[i];
		philosophers[i].right_fork = &forks[(i + 1)
			% config->number_of_philosophers];
		pthread_mutex_init(&philosophers[i].meal_lock, NULL);
		pthread_mutex_init(&philosophers[i].print_lock, NULL);
		i++;
	}
}

static bool	has_philosopher_finished_eating(t_philo *philo)
{
	if (philo->config->not_to_eat != -1
		&& philo->meals_eaten >= philo->config->not_to_eat)
	{
		printf("Philosopher %d ate %d times and is leaving.\n",
			philo->id, philo->meals_eaten);
		return (true);
	}
	return (false);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (*(philo->life_flag))
	{
		printf("%ld %d is thinking\n", get_time(), philo->id);
		pthread_mutex_lock(philo->left_fork);
        printf("%ld %d has taken left fork\n", get_time(), philo->id);
        pthread_mutex_lock(philo->right_fork);
        printf("%ld %d has taken right fork\n", get_time(), philo->id);
		pthread_mutex_lock(&philo->meal_lock);
        philo->last_meal_time = get_time();
        printf("%ld %d is eating\n", philo->last_meal_time, philo->id);
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->meal_lock);
        usleep(philo->config->time_to_eat * 1000);
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        if (has_philosopher_finished_eating(philo))
            break ;
        printf("%ld %d is sleeping\n", get_time(), philo->id);
        usleep(philo->config->time_to_sleep * 1000);
	}
	return (NULL);
}

void	cleanup(pthread_mutex_t *forks, t_philo *philosophers,
			int num_philosophers)
{
	int	i;

	i = 0;
	while (i < num_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(philosophers);
	free(forks);
}
