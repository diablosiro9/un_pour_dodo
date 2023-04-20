/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imac21 <imac21@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 15:04:10 by dojannin          #+#    #+#             */
/*   Updated: 2023/04/19 22:53:24 by imac21           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/include.h"

/*
    Mimi
    Déplacement de la fonction lexer dans le dossier/.c dédié ./src/lexer/lexer.c
    simplement par claireté et norme

*/


t_token	*recuperator(void)
{
	t_token	*tmp;

	tmp = g_data.token;
	while (tmp)
	{
		if (tmp->t_type == 9)
		{
			if (tmp->next && tmp->next->t_type == 3)
				tmp->next->t_type = 10;
		}
		tmp = tmp->next;
	}
	return (NULL);
}


t_token	*modificator(void)
{
	//TRANSFORME LES WORD EN EOF SI PRESENCE HERE_DOC PREALABLE
	t_token	*tmp;

	tmp = g_data.token;
	while (tmp)
	{
		if (tmp->t_type == 9)
		{
			if (tmp->next)
			{
				if (tmp->next->t_type != 0 && tmp->next->t_type != 1
					&& tmp->next->t_type != 2 && tmp->next->t_type != 9)
					tmp->next->t_type = 10;
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	destroyer(int start, int i)
{
	//DETRUIT LES TOKENS ENTRE START ET I
	t_token *tmp;
	int		compare;

	tmp = g_data.token;
	compare = 0;
	while (compare < i)
	{
		if (compare >= start && compare < i)	
		{
			tmp = delete_token(tmp, compare);
			compare = compare - 1;
			i = i - 1;
			start = start - 1;
		}
		compare++;
	}
	if (tmp)
		g_data.token = tmp;
	else
		g_data.token = NULL;
}

t_token	*terminator(void)
{
	t_token	*tmp;
	char	*res;
	int		i;
	int		start;
	

	i = 0;
	start = 0;
	tmp = g_data.token;
	res = NULL;
	while (tmp)
	{
		if (tmp->t_type == 4 || tmp->t_type == 5)
		{
			if (ft_strlen(tmp->value) == 3)
			{
				if (start == 0)
					start = i;
				while (ft_strlen(tmp->value) == 3 && tmp->value[1] != '-' && tmp)
				{
					i++;
					if (tmp->next)
						tmp = tmp->next;
					else
						break ;
				}
				res = malloc(sizeof(char) * (i + 1));
				tmp = g_data.token;
				while (tmp && (ft_strlen(tmp->value) != 3 || tmp->t_type != 4 || tmp->t_type != 5))
				{
					printf("tmp->value = %s\n", tmp->value);
					tmp = tmp->next;
				}
				if (tmp)
					if (tmp->value[0] == '-')
						tmp = tmp->next;
				i = 0;
				tmp = g_data.token;
				while (ft_strlen(tmp->value) == 3 && tmp->value[1] != '-' && tmp != NULL)
				{
					res[i] = tmp->value[1];
					i++;
					if (tmp->next == NULL)
					{
						break;
					}
					tmp = tmp->next;
				}
				res[i] = '\0';
				if (start == 0)
					destroyer(start, i);
				printf("RESSSSS = %s\n", res);
				printf("i = %d\n", start);
				ft_token_add(&g_data.token, ft_token_new(WORD, res));
				tmp = g_data.token;
				break ;
			}
		}
		if (tmp->next)
			tmp = tmp->next;
		i++;
	}
	return (g_data.token);
}

t_token	*reunificator()
{
	t_token	*tmp;
	char	*stock = NULL;
	int		i;
	int		start;
	char	*res;

	res = NULL;
	i = 0;
	start = 0;
	tmp = g_data.token;
	while (tmp)
	{
		stock = tmp->value;
		if (ft_strlen(stock) == 1 && tmp->value[0] == '-')
		{
			tmp = tmp->next;
			stock = tmp->value;
			while (stock[i])
				i++;
			res = malloc(sizeof(char) * i + 2);
			i = 0;
			res[0] = '-';
			while (stock[i])
			{
				res[i + 1] = stock[i];
				i++;
			}
			res[i + 1] = '\0';
			tmp = g_data.token;
			while (tmp)
			{
				if (ft_strlen(tmp->value) == 1 && tmp->value[0] == '-')
				{	
					destroyer(start, i + 1);
					tmp = g_data.token;
				}
				if (!tmp && !tmp->next)
					break;
				tmp = tmp->next;
				i++;
			}
			ft_token_add(&g_data.token, ft_token_new(OPTION, res));
		}
		if (!tmp || !tmp->next || tmp->next == NULL)
			break ;
		tmp = tmp->next;
		start++;
	}
	return (g_data.token);
}

void	ft_tab_delete(char **tab)
{
	// char	*tmp;
	int		i;

	i = 0;
	while (tab[i] != NULL)
	{
		// printf("tab[i] = %s\n", tab[i]);
		// tmp = tab[i];
		// tab[i] = tab[i + 1];
		free(tab[i]);
		i++;
	}
	free(tab);
}


t_list	*ft_lexer(char *str)
{
	int		i;
	t_list	*list;
	char	**tab;

	i = 0;
	list = NULL;
	tab = ft_split(str);
	if (!tab || tab[0] == NULL)
	{
		// printf("Error : malloc failed() in ft_lexer.\na");
		return (NULL);
	}
	while (tab[i])
	{
		ft_lstadd(&list, ft_lstnew(tab[i]));
		i++;
	}
	afflist(list, "lexer");
	tokenizateur(list);
	recuperator();
	modificator();
	afflist(list, "lexer");
	if (terminator() == NULL)
		return (NULL);
	// if (reunificator() == NULL)
	// 	return (NULL);
	free(tab);
	printf("fin lexer\n");
	afflist(list, "lexer");
	return (list);
}
