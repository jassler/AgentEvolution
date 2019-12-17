#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef POPULATION
#define POPULATION 1024
#endif
#define POPULATION_STR QUOTE(POPULATION)

#ifndef GENSIZE
#define GENSIZE 3
#endif
#define GENSIZE_STR QUOTE(GENSIZE)

#ifndef PHENSIZE
#define PHENSIZE 3
#endif
#define PHENSIZE_STR QUOTE(PHENSIZE)

#ifndef PAYOFF
#define PAYOFF {{0,1,-1},{-1,0,1},{1,-1,0}}
#endif
#define PAYOFF_STR QUOTE(PAYOFF)

#ifndef AGENTSTART
#define AGENTSTART {0.25,0.25,0.5}
#endif

#ifndef GENERATIONS
#define GENERATIONS 10000
#endif
#define GENERATIONS_STR QUOTE(GENERATIONS)

#ifndef GENOME_PROB
#define GENOME_PROB 0.0001
#endif
#define GENOME_PROB_STR QUOTE(GENOME_PROB)

#ifndef MATRIX_PROB
#define MATRIX_PROB 0
#endif
#define MATRIX_PROB_STR QUOTE(MATRIX_PROB)

#ifndef AGENTCONDITION
#define AGENTCONDITION agent.get_genome()[0]==1.0/3.0
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG 1
#endif

#ifndef LOG_PROGRESS
#define LOG_PROGRESS 1
#endif
