#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <time.h>

// Configuration structure
typedef struct {
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	int life_span;
} config_t;

// Philosopher structure
typedef struct {
	int					id;
	int					meals_eaten;
	long				last_meal_time;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	config_t			*config;
	bool				*life_flag;
} philo_t;

// Function prototypes
void	init_config(config_t* config, int argc, char** argv);
void	init_philosophers(philo_t* philosophers, pthread_mutex_t* forks, config_t* config, bool* life_flag);
void	*philosopher_routine(void* arg);
void	cleanup(pthread_mutex_t* forks, philo_t* philosophers, int num_philosophers);

#endif
