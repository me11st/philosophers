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

static bool	check_all_done(t_philo *philosophers, t_config *config)
{
	int	i;

	i = 0;
	while (i < config->number_of_philosophers)
	{
		if (philosophers[i].meals_eaten < config->not_to_eat)
		{
			return (false);
		}
		i++;
	}
	return (true);
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

static void	join_philosopher_threads(t_philo *philosophers,
				int number_of_philosophers)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_join(philosophers[i].thread, NULL);
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
	init_forks_and_threads(*forks, config->number_of_philosophers,
		*philosophers);
	init_philosophers(*philosophers, *forks, config, life_flag);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config		config;
	bool			life_flag;
	bool			all_done;
	t_philo			*philosophers;
	pthread_mutex_t	*forks;

	all_done = false;
	init_config(&config, argc, argv);
	if (init_simulation(&config, &philosophers, &forks, &life_flag))
		return (1);
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

// int	main(int argc, char **argv)
// {
// 	t_config		config;
// 	t_philo			*philosophers;
// 	pthread_mutex_t	*forks;
// 	pthread_t		*threads;
// 	pthread_t		monitor_thread;
// 	bool			life_flag = true;

// 	init_config(&config, argc, argv);
// 	philosophers = malloc(sizeof(t_philo) * config.number_of_philosophers);
// 	forks = malloc(sizeof(pthread_mutex_t) * config.number_of_philosophers);
// 	threads = malloc(sizeof(pthread_t) * config.number_of_philosophers);
// 	for (int i = 0; i < config.number_of_philosophers; i++)
// 		pthread_mutex_init(&forks[i], NULL);
// 	init_philosophers(philosophers, forks, &config, &life_flag);
// 	for (int i = 0; i < config.number_of_philosophers; i++)
// 		pthread_create(&threads[i], NULL, philosopher_routine, &philosophers[i]);
// 	pthread_create(&monitor_thread, NULL, monitor_philosophers, philosophers);
// 	for (int i = 0; i < config.number_of_philosophers; i++)
// 		pthread_join(threads[i], NULL);
// 	pthread_join(monitor_thread, NULL);
// 	cleanup(forks, philosophers, config.number_of_philosophers);
// 	free(threads);
// 	return (0);
// }
