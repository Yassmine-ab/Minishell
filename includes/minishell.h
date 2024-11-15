/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 02:04:44 by yaabdall          #+#    #+#             */
/*   Updated: 2024/11/15 20:44:59 by yaabdall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include "ft_printf.h"
# include "get_next_line.h"
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

// MAX VALUES
# define MAX_PATHLENGTH 4096
# define MAX_TOKENS 1024

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
	WILDCARD,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	END
}	t_token_type;

// AST NODE TYPES ENUM
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_ARGUMENT,
	NODE_REDIRECTION,
	NODE_OPERATOR,
	NODE_GROUP
}	t_node_type;

// LEXER STATES ENUM
typedef enum e_lexer_state
{
	GENERAL,
	IN_WORD,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE,
	AFTER_OPERATOR
}	t_lexer_state;

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
	char				**envp;
	char				*line;
	t_token				*tokens;
	t_token_type		current_type;
	t_node				*node;
	t_gc				gc;
	t_lexer_state		state;
	int					last_exit_status;
}	t_minishell;

/* -------------------------------------------------------------------------- */
/*                             FUNCTION PROTOTYPES                            */
/* -------------------------------------------------------------------------- */

/* ----------------------------- Initialization ----------------------------- */
void	data_init(char **argv, char **envp, t_minishell *data);

/* --------------------------------- Prompt --------------------------------- */
char	*create_prompt(void);

/* --------------------------------- Lexing --------------------------------- */
t_token	*tokenize_input(char *input, t_minishell *data);
t_token	create_token(t_token_type type, char *value);
void	free_tokens(t_token *tokens);
void	skip_whitespace(char **input, int *index);
void	process_single_quotes(char *input, int *i, int *count, t_minishell *data);
void	process_double_quotes(char *input, int *i, int *count, t_minishell *data);
void	process_parentheses(char *input, int *i, int *count, t_minishell *data);
void	process_operator(char *input, int *i, int *count, t_minishell *data);
void	process_command(char *input, int *i, int *count, t_minishell *data);
void	process_file(char *input, int *i, int *count, t_minishell *data);
void	process_wildcard(char *input, int *i, int *count, t_minishell *data);
void	process_limiter(char *input, int *i, int *count, t_minishell *data);
void	process_env_variable(char *input, int *i, int *count, t_minishell *data);

/* -------------------------------- Parsing --------------------------------- */
t_node	*parse_tokens(t_token *tokens, t_gc *gc);
t_node	*create_ast_node(t_node_type type, char *value, t_gc *gc);

/* ------------------------------- Utilities -------------------------------- */
void	clean_up(t_minishell *data);
void	error(const char *error_msg, int status, t_gc *gc);

#endif
