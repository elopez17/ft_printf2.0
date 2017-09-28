/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_spec_x.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/01 13:42:05 by elopez            #+#    #+#             */
/*   Updated: 2017/09/27 03:30:36 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static uintmax_t    len_arg(t_flags *flag, va_list *ap)
{
	if (flag->l)
		return (va_arg(*ap, unsigned long));
	if (flag->hh)
		return ((unsigned char)va_arg(*ap, unsigned int));
	if (flag->h)
		return ((unsigned short)va_arg(*ap, unsigned int));
	if (flag->ll)
		return (va_arg(*ap, unsigned long long));
	if (flag->j)
		return (va_arg(*ap, uintmax_t));
	if (flag->z)
		return (va_arg(*ap, size_t));
	return (va_arg(*ap, unsigned int));
}

static char	*itox(uintmax_t dec)
{
	char	tmp[16];
	char	*s;
	int		i;
	int		j;
	int		remain;

	i = -1;
	j = -1;
	while (dec)
	{
		remain = dec % 16;
		tmp[++i] = (remain < 10 ? 48 : 87) + remain;
		dec /= 16;
	}
	s = ft_strnew(i + 1);
	while (i >= 0)
	{
		s[++j] = tmp[i--];
	}
	return (s);
}

static char	*add_zeros(t_flags *flag, int *len)
{
	char	*str;
	int		zeros;

	zeros = 0;
	if (flag->prec && flag->prec_num > *len)
	{
		zeros = flag->prec_num - *len;
		*len += zeros;
	}
	*len += (flag->alter ? 2 : 0);
	if (flag->zero)
	{
		flag->width -= *len;
		if (flag->width > zeros)
		{
			zeros = flag->width;
			*len += zeros;
		}
	}
	ft_memset((str = ft_strnew(zeros)), '0', zeros);
	flag->alter ? str = ft_strmer(ft_strdup("0x"), str) : NULL;
	return (str);
}

static void	print_width(t_flags *flag, t_outp *op, char *s, int *ret)
{
	flag->width -= *ret;
	if (flag->left_adj)
	{
		op->str = ft_strmer(op->str, s);
		if (flag->width)
			op->str = ft_strmer(op->str, MAKES(' ', flag->width));
		return ;
	}
	if (flag->width)
		op->str = ft_strmer(op->str, MAKES(' ', flag->width));
	op->str = ft_strmer(op->str, s);
}

void		pf_spec_x(t_flags *flag, t_outp *op, va_list *ap)
{
	uintmax_t	dec;
	int			len;
	char		*s;

	if (!(dec = len_arg(flag, ap)))
		s = ft_strdup((flag->prec && !flag->prec_num ? "" : "0"));
	else
		s = itox(dec);
	if ((len = ft_strlen(s)) && *s != '0')
		s = ft_strmer(add_zeros(flag, &len), s);
	if (flag->width > len)
		print_width(flag, op, s, &len);
	else
		op->str = ft_strmer(op->str, s);
}
