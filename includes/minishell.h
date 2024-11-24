/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 02:04:44 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/24 16:56:11 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include "get_next_line_bonus.h"
# include "gc.h"
# include <fnmatch.h>

/* -------------------------------------------------------------------------- */
/*                                  DEFINES                                   */
/* -------------------------------------------------------------------------- */

// COLORS
# define COLOR_ENABLED 1
# if COLOR_ENABLED
#  define DEFAULT "\033[0m"
#  define RED "\033[1;31m"
#  define GREEN "\033[1;32m"
#  define YELLOW "\033[1;33m"
#  define BLUE "\033[1;34m"
#  define MAGENTA "\033[1;35m"
#  define ROSE "\033[1;95m"
#  define CYAN "\033[1;36m"
#  define WHITE "\033[1;37m"
# else
#  define DEFAULT ""
#  define RED ""
#  define GREEN ""
#  define YELLOW ""
#  define BLUE ""
#  define MAGENTA ""
#  define ROSE ""
#  define CYAN ""
#  define WHITE ""
# endif

// MAX VALUES
# define MAX_PATHLENGTH 4096
# define MAX_TOKENS 4096
# define MAX_MATCHES 4096

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
	STDOUT_APPEND,
	FILENAME,
	HEREDOC,
	LIMITER,
	AND,
	OR,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	END
}	t_token_type;

// AST NODE TYPES ENUM
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_ARG,
	NODE_PIPE,
	NODE_REDIR,
	NODE_FILE,
	NODE_HEREDOC,
	NODE_LIMITER,
	NODE_AND,
	NODE_OR,
	NODE_GROUP
}	t_node_type;

/* -------------------------------------------------------------------------- */
/*                                 STRUCTURES                                 */
/* -------------------------------------------------------------------------- */

// TOKEN STRUCTURE
typedef struct s_token
{
	t_token_type		type;
	char				*value;
}	t_token;

// AST NODE STRUCTURE
typedef struct s_node
{
	t_node_type			type;
	char				*value;
	struct s_node		*left;
	struct s_node		*right;
	struct s_node		*next;
}	t_node;

// DATA STRUCTURE
typedef struct s_minishell
{
	int					argc;
	char				**envp;
	char				**argv;
	char				*line;
	t_token				*tokens;
	t_token_type		current_type;
	t_node				*node;
	t_gc				gc;
	int					last_exit_status;
	pid_t				pid;
	bool				stop_parenthesis_close;
}	t_minishell;

/* -------------------------------------------------------------------------- */
/*                             FUNCTION PROTOTYPES                            */
/* -------------------------------------------------------------------------- */

/* ----------------------------- Initialization ----------------------------- */
void	data_init(int argc, char **argv, char **envp, t_minishell *data);

/* --------------------------------- Prompt --------------------------------- */
char	*create_prompt(t_gc *gc);

/* --------------------------------- Lexing --------------------------------- */
t_token	*tokenize_input(char *input, t_minishell *data);
t_token	create_token(t_token_type type, char *value);
void	free_tokens(t_token *tokens);
void	skip_whitespace(char **input, int *index);
void	process_single_quotes(char *input, int *i, int *count, t_minishell *data);
void	process_double_quotes(char *input, int *i, int *count, t_minishell *data);
void	process_parentheses(char *input, int *i, int *count, t_minishell *data);
void	process_operator(char *input, int *i, int *count, t_minishell *data);
void	process_word(char *input, int *i, int *count, t_minishell *data);

/* -------------------------------- Parsing --------------------------------- */
void	expand_variables(t_minishell *data);
int		expand_env_variable(char **result, size_t *size, int i, t_minishell *data);
int		expand_wildcard(char **result, size_t *size, int i, t_minishell *data);
t_node	*parse_expression(int *i, t_minishell *data);
t_node	*parse_command(int *i, t_minishell *data);
int		parse_redirection(int *i, t_node **cmd_node, t_minishell *data);
int		parse_heredoc(int *i, t_node **cmd_node, t_minishell *data);
t_node	*create_node(t_node_type type, char *value, t_gc *gc);

/* ------------------------------- Utilities -------------------------------- */
void	strncat_realloc(char **result, char *append, size_t *size, t_gc *gc);
void	error(const char *error_msg, int status, t_gc *gc);

#endif
