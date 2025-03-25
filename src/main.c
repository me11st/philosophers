/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:17:14 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/23 13:17:18 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static bool	check_philosopher_status(t_philo *philosophers, t_config *config)
{
	int		i;
	long	current_time;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		pthread_mutex_lock(&philosophers[i].meal_lock);
		current_time = get_time();
		if (current_time - philosophers[i].last_meal_time > config->time_to_die)
		{
			pthread_mutex_lock(&philosophers[i].print_lock);
			printf("%ld %d died\n", current_time, philosophers[i].id);
			*philosophers[i].life_flag = false;
			pthread_mutex_unlock(&philosophers[i].print_lock);
			pthread_mutex_unlock(&philosophers[i].meal_lock);
			usleep(1000);
			exit(0);
		}
		pthread_mutex_unlock(&philosophers[i].meal_lock);
		if (config->not_to_eat != -1 && 
			philosophers[i].meals_eaten < config->not_to_eat)
			return (false);
		i++;
	}
	return (config->not_to_eat != -1);
}

static void	init_forks_and_threads(pthread_mutex_t *forks,
				int number_of_philosophers, t_philo *philosophers)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_create(&philosophers[i].thread,
			NULL, philosopher_routine, &philosophers[i]);
		i++;
	}
}

static int	init_simulation(t_config *config, t_philo **philosophers,
	pthread_mutex_t **forks, bool *life_flag)
{
	*life_flag = true;
	*philosophers = malloc(sizeof(t_philo) * config->number_of_philosophers);
	if (!*philosophers)
	{
		perror("malloc");
		return (1);
	}
	*forks = malloc(sizeof(pthread_mutex_t) * config->number_of_philosophers);
	if (!*forks)
	{
		free(*philosophers);
		perror("malloc");
		return (1);
	}
	init_philosophers(*philosophers, *forks, config, life_flag);
	init_forks_and_threads(*forks, config->number_of_philosophers,
		*philosophers);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config		config;
	bool			life_flag;
	t_philo			*philosophers;
	pthread_mutex_t	*forks;
	int				i;

	i = 0;
	init_config(&config, argc, argv);
	if (init_simulation(&config, &philosophers, &forks, &life_flag))
		return (1);
	while (!check_philosopher_status(philosophers, &config))
		usleep(1000);
	life_flag = false;
	while (i < config.number_of_philosophers)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
	cleanup(forks, philosophers, config.number_of_philosophers);
	return (0);
}
