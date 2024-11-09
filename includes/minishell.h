#ifndef MINISHELL_H
# define MINISHELL_H

/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include "ft_printf.h"
# include "gc.h"

/* -------------------------------------------------------------------------- */
/*                                  DEFINES                                   */
/* -------------------------------------------------------------------------- */

// COLORS
# define DEFAULT "\033[0m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define ROSE "\033[1;95m"
# define CYAN "\033[1;36m"
# define WHITE "\033[1;37m"

/* -------------------------------------------------------------------------- */
/*                                   ENUMS                                    */
/* -------------------------------------------------------------------------- */

// TOKEN TYPES ENUM
typedef enum e_token_type
{
	COMMAND,
	ARGUMENT,
	PIPE,
	STDIN,
	STDOUT,
	HEREDOC,
	STDOUT_APPEND,
	ENV_VARIABLE,
	QUOTED_STRING,
	AND,
	OR,
	WILDCARD,
	SIGNAL,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE
}	t_token_type;

/* -------------------------------------------------------------------------- */
/*                                 STRUCTURES                                 */
/* -------------------------------------------------------------------------- */

// TOKEN STRUCTURE
typedef struct s_token
{
	t_token_type		type;
	char				*value;
}	t_token;

// DATA STRUCTURE
typedef struct s_minishell
{
	char				*line;
	char				**envp;

}	t_minishell;

/* -------------------------------------------------------------------------- */
/*                             FUNCTION PROTOTYPES                            */
/* -------------------------------------------------------------------------- */

/* ----------------------------- Initialization ----------------------------- */
void	data_init(char **argv, t_minishell *data);

/* --------------------------------- Prompt --------------------------------- */
char	*create_prompt(void);

/* -------------------------------- Parsing --------------------------------- */
void	parse_input(char *input, t_minishell *data);

/* ------------------------------- Utilities -------------------------------- */
// BLABLA
void	ft_free(void *ptr);
void	clean_up(t_minishell *data);
void	error(const char *error_msg, int status, t_minishell *data);

#endif
