/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaabdall <yaabdall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 02:04:44 by yaabdall          #+#    #+#             */
/*   Updated: 2024/12/13 19:31:48 by yaabdall         ###   ########.fr       */
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
# include <sys/wait.h>
# include <dirent.h>
# include "get_next_line_bonus.h"
# include "gc.h"
# include <fnmatch.h>
# include <termios.h>
# include <signal.h>

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

// PIPE FILE DESCRIPTORS
# define READ_END 0
# define WRITE_END 1

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
	NODE_FD,
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
	bool				quoted;
	bool				space_after;
}	t_token;

// AST NODE STRUCTURE
typedef struct s_node
{
	t_node_type			type;
	char				*value;
	int					fd;
	bool				quoted;
	bool				space_after;
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
	int					heredoc_status;
	int					last_exit_status;
	int					open_parentheses;
	pid_t				pid;
	int					here_doc[2];
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
void	skip_whitespace(char **input, int *index);
char	*process_single_quotes(char *input, int *i, t_minishell *data);
char	*process_double_quotes(char *input, int *i, t_minishell *data);
void	process_parentheses(char *input, int *i, int *count, t_minishell *data);
void	process_operator(char *input, int *i, int *count, t_minishell *data);
void	process_word(char *input, int *i, int *count, t_minishell *data);

/* -------------------------------- Parsing --------------------------------- */
char	*expand_variable(char *str, t_minishell *data);
void	expand_variables(t_node *node, t_minishell *data);
int		expand_env_variable(char **result, size_t *size, int i, char *str, t_minishell *data);
int		expand_wildcard(char **result, size_t *size, int i, char *str, t_minishell *data);
t_node	*parse_expression(int *i, t_minishell *data);
t_node	*parse_command(int *i, t_minishell *data);
int		parse_redirection(int *i, t_node **cmd_node, t_minishell *data);
int		parse_heredoc(int *i, t_node **cmd_node, t_minishell *data);
t_node	*create_node(t_node_type type, t_token token, t_gc *gc);

/* --------------------------------- Exec ----------------------------------- */
void	process_here_doc(t_node *node, t_minishell *data);
void	execute_ast(t_node *ast, t_minishell *data);
void	execute_ast(t_node *ast, t_minishell *data);
void	execute_command(t_node *cmd_node, t_minishell *data);
void	execute_builtins(t_node *current, t_minishell *data);
void	execute_pipe(t_node *ast, t_minishell *data);
void	execute_and(t_node *ast, t_minishell *data);
void	execute_or(t_node *ast, t_minishell *data);

/* ------------------------------ Environment ------------------------------- */
char	**envp_is_null(t_minishell *data);
void	free_envp(t_minishell *data);
char	*get_env_value(char *env_key, t_minishell *data);
int		get_env_index(char *env_key, t_minishell *data);
void	update_env(char *key, char *new_value, t_minishell *data);
void	add_env(char *key, char *value, t_minishell *data);

/* -------------------------------- Signal ---------------------------------- */
void	init_signal(void);
void	handle_sigint(int sig);

/* ------------------------------- Builtins --------------------------------- */
void	ft_echo(t_node *cmd_node, t_minishell *data);
void	ft_cd(t_node *cmd_node, t_minishell *data);
void	ft_pwd(t_node *cmd_node, t_minishell *data);
void	ft_env(t_node *cmd_node, t_minishell *data);
void	ft_unset(t_node *cmd_node, t_minishell *data);
void	ft_export(t_node *cmd_node, t_minishell *data);
void	ft_exit(t_node *cmd_node, t_minishell *data);

/* ------------------------------- Utilities -------------------------------- */
void	strncat_realloc(char **result, char *append, size_t *size, t_gc *gc);
void	error(const char *error_msg, int status, t_gc *gc);
int		is_number(const char *str);
int		is_redir_following(int current_index, t_minishell *data);
void	close_fd(int *fd);
void	bubble_sort(char **array, int size);

#endif
