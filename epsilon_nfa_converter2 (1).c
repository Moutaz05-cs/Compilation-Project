#include <stdio.h>

#define STATE_LIMIT 10
#define SYMBOL_LIMIT 5

// Configuration parameters
int state_count, symbol_count;
char symbols[SYMBOL_LIMIT];

// Automaton specification
int start_state;
int accepting_states[STATE_LIMIT];

// Transition tables
int delta[STATE_LIMIT][SYMBOL_LIMIT][STATE_LIMIT];
int epsilon_delta[STATE_LIMIT][STATE_LIMIT];
int closure[STATE_LIMIT][STATE_LIMIT];
int result_delta[STATE_LIMIT][SYMBOL_LIMIT][STATE_LIMIT];

/**
 * Reset all arrays to initial state
 */
void reset_arrays(void)
{
    int i, j, k;
    
    for(i = 0; i < STATE_LIMIT; i++)
    {
        accepting_states[i] = 0;
        
        for(j = 0; j < SYMBOL_LIMIT; j++)
        {
            for(k = 0; k < STATE_LIMIT; k++)
            {
                delta[i][j][k] = 0;
                result_delta[i][j][k] = 0;
            }
        }
        
        for(j = 0; j < STATE_LIMIT; j++)
        {
            epsilon_delta[i][j] = 0;
            closure[i][j] = 0;
        }
    }
}

/**
 * Collect automaton details from user
 */
void collect_input(void)
{
    int state, count, i;
    char ch;

    printf("Number of states: ");
    scanf("%d", &state_count);

    printf("Number of symbols: ");
    scanf("%d", &symbol_count);

    printf("Alphabet symbols: ");
    for(i = 0; i < symbol_count; i++)
        scanf(" %c", &symbols[i]);

    printf("Initial state: ");
    scanf("%d", &start_state);

    printf("Number of final states: ");
    scanf("%d", &count);

    printf("Final states: ");
    for(i = 0; i < count; i++)
    {
        scanf("%d", &state);
        accepting_states[state] = 1;
    }

    printf("Symbol transitions (-1 to stop):\n");
    for(;;)
    {
        int src, dest, idx;
        scanf("%d", &src);
        
        if(src == -1) break;

        scanf(" %c %d", &ch, &dest);

        for(idx = 0; idx < symbol_count; idx++)
        {
            if(symbols[idx] == ch)
                delta[src][idx][dest] = 1;
        }
    }

    printf("Epsilon transitions (-1 to stop):\n");
    for(;;)
    {
        int src, dest;
        scanf("%d", &src);
        
        if(src == -1) break;

        scanf("%d", &dest);
        epsilon_delta[src][dest] = 1;
    }
}

/**
 * Calculate epsilon-closure using Floyd-Warshall approach
 */
void calculate_closure(void)
{
    int changed, i, j, k;

    // Base case: reflexive property
    for(i = 0; i < state_count; i++)
        closure[i][i] = 1;

    // Iterate until no more changes
    do
    {
        changed = 0;
        
        for(i = 0; i < state_count; i++)
        {
            for(j = 0; j < state_count; j++)
            {
                if(closure[i][j])
                {
                    for(k = 0; k < state_count; k++)
                    {
                        if(epsilon_delta[j][k] && !closure[i][k])
                        {
                            closure[i][k] = 1;
                            changed = 1;
                        }
                    }
                }
            }
        }
    } while(changed);
}

/**
 * Convert epsilon-NFA to standard NFA
 */
void perform_conversion(void)
{
    int s, c, sym, mid, d;
    
    for(s = 0; s < state_count; s++)
    {
        for(c = 0; c < state_count; c++)
        {
            if(closure[s][c])
            {
                for(sym = 0; sym < symbol_count; sym++)
                {
                    for(mid = 0; mid < state_count; mid++)
                    {
                        if(delta[c][sym][mid])
                        {
                            for(d = 0; d < state_count; d++)
                            {
                                if(closure[mid][d])
                                    result_delta[s][sym][d] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * Adjust final states considering epsilon-closure
 */
void adjust_final_states(void)
{
    int i, j;
    
    for(i = 0; i < state_count; i++)
    {
        for(j = 0; j < state_count; j++)
        {
            if(closure[i][j] && accepting_states[j])
                accepting_states[i] = 1;
        }
    }
}

/**
 * Print the converted automaton
 */
void print_automaton(void)
{
    int i, j, k;
    
    printf("\n===== ε-free NFA =====\n");
    printf("Start state: %d\n", start_state);

    printf("Final states: ");
    for(i = 0; i < state_count; i++)
    {
        if(accepting_states[i])
            printf("%d ", i);
    }
    printf("\n");

    printf("Transitions:\n");
    for(i = 0; i < state_count; i++)
    {
        for(j = 0; j < symbol_count; j++)
        {
            for(k = 0; k < state_count; k++)
            {
                if(result_delta[i][j][k])
                    printf("%d --%c--> %d\n", i, symbols[j], k);
            }
        }
    }
}

/**
 * Entry point
 */
int main(void)
{
    reset_arrays();
    collect_input();
    calculate_closure();
    perform_conversion();
    adjust_final_states();
    print_automaton();
    
    return 0;
}