/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 15:04:10 by dojannin          #+#    #+#             */
/*   Updated: 2023/04/19 13:22:22 by dojannin         ###   ########.fr       */
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
	g_data.token = tmp;
}

t_token	*terminator(void)
{
	t_token	*tmp;
	char	*stock;
	char	*res;
	int		i;
	int		flag;
	int		start;
	

	i = 0;
	start = 0;
	flag = 0;
	stock = NULL;
	tmp = g_data.token;
	res = NULL;
	while (tmp)
	{
		stock = tmp->value;
		if (ft_strlen(stock) > 1)
		{
			while (stock[i])
			{
				if (stock[i] == ' ')
					return (NULL);
				i++;
			}
			i = 0;
			if (!tmp->next)
				return (g_data.token);
			tmp = tmp->next;
			start++;
			stock = tmp->value;
		}
		if (ft_strlen(stock) == 1 && tmp->value[0] == '-')
		{
			tmp = tmp->next;
			i++;
			start++;
		}
		if (ft_strlen(stock) == 1 && tmp->t_type == 1)
		{
			flag = 2;
			if (tmp->next)
				tmp = tmp->next;
		}
		stock = tmp->value;
		if (ft_strlen(stock) == 1 && tmp->t_type != 10 && tmp->t_type != 0)
		{
			if (!tmp->next)
				return (g_data.token);
			if (tmp->value[0] == '-' && !tmp->next)
				return (NULL);
			if (!tmp->next && ft_strlen(tmp->value) == 1 && tmp->value[0] != '-')
				i++;
			while (ft_strlen(tmp->value) == 1 && tmp->value[0] != '-' && tmp)
			{
				i++;
				if (tmp->next)
					tmp = tmp->next;
				else
					break ;
			}
			res = malloc(sizeof(char) * (i + 1));
			tmp = g_data.token;
			while (tmp && ft_strlen(tmp->value) > 1)
				tmp = tmp->next;
			if (tmp->value[0] == '-')
				tmp = tmp->next;
			i = 0;
			while (ft_strlen(tmp->value) == 1 && tmp->value[0] != '-' && tmp != NULL)
			{
				res[i] = tmp->value[0];
				i++;
				if (tmp->next == NULL)
				{
					flag = 1;
					break;
				}
				tmp = tmp->next;
			}
			res[i] = '\0';
			destroyer(start, i + start);
			if (start == 0)
			{
				ft_token_addf(&g_data.token, ft_token_new(WORD, res));
				start = 1;
			}
			else if (ft_strlen(res) > 1)
				ft_token_add(&g_data.token, ft_token_new(WORD, res));
			if (flag == 1)
				return (g_data.token);
		}
		if (tmp->next)
			tmp = tmp->next;
		else
			return (g_data.token);
		start++;
	}
	return (NULL);
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
	// if (terminator() == NULL)
	// 	return (NULL);
	// if (reunificator() == NULL)
	// 	return (NULL);
	free(tab);
	printf("fin lexer\n");
	afflist(list, "lexer");
	return (list);
}
