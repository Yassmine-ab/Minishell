#ifndef MINISHELL_H
# define MINISHELL_H

/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

# include <stdio.h>
# include "ft_printf.h"
# include "gc.h"

/* -------------------------------------------------------------------------- */
/*                                   ENUMS                                    */
/* -------------------------------------------------------------------------- */

// TOKEN TYPES ENUM
typedef enum e_token_type
{
	COMMAND,
	VARIABLE,
	PIPE,
	STDIN,
	STDOUT,
	AND,
	OR
}	t_token_type;

/* -------------------------------------------------------------------------- */
/*                                 STRUCTURES                                 */
/* -------------------------------------------------------------------------- */

// DATA STRUCTURE
typedef struct s_shell
{
	int				number_of_philos;

}	t_shell;

/* -------------------------------------------------------------------------- */
/*                             FUNCTION PROTOTYPES                            */
/* -------------------------------------------------------------------------- */

/* -------------------------- Initialization ------------------------------- */
void	data_init(char **argv, t_shell *data);

/* ------------------------------ Parsing ---------------------------------- */
void	parse_input(char *input, t_shell *data);

/* ------------------------------ Utilities -------------------------------- */
// BLABLA
void	clean_up(t_shell *data);
void	error(const char *error_msg, int status, t_shell *data);

#endif
