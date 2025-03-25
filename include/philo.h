/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mestefan <mestefan@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 13:25:58 by mestefan          #+#    #+#             */
/*   Updated: 2025/03/23 13:26:01 by mestefan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_config {
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	not_to_eat;
	int	life_span;
	pthread_mutex_t		print_lock;
    pthread_mutex_t		death_mutex;
}		t_config;

// Philosopher structure
typedef struct s_philosopher {
	int					id;
	int					meals_eaten;
	long				last_meal_time;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		meal_lock;
	t_config			*config;
	bool				*life_flag;
}		t_philo;

// Function prototypes
void	init_config(t_config *config, int argc, char **argv);
void	init_philosophers(t_philo *philosophers, pthread_mutex_t *forks,
			t_config *config, bool *life_flag);
void	*philosopher_routine(void *arg);
void	cleanup(pthread_mutex_t *forks, t_philo *philosophers,
			int num_philosophers);
long	get_time(void);
int		sync_printf(t_philo *philo, const char *format, ...);

#endif
