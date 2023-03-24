/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppimchan <ppimchan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 19:41:29 by ppimchan          #+#    #+#             */
/*   Updated: 2023/03/25 02:39:34 by ppimchan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// #### ALIGN


int	printlen(char *str)
{
	int		n;

	n = ft_strlen(str);
	// printf("strlen : %d and %s", n, str);

	if (n == 0)
		return (1);
	else
		return (n);
}

// char	*align_left(char *str, t_list *fmt)
// {
// 	char	*fstr;
// 	int		i;
// 	int		plen;

// 	i = 0;
// 	// นับความยาวของ string ที่จะ print 
// 	plen = printlen(str);
// 	// printf("strlen : %d", plen);

// 	// จองพื้นที่
// 	fstr = ft_calloc(sizeof(char), fmt->width + 1);
// 	// printf("fmt->width %d", fmt->width);
// 	if (!fstr)
// 		return (NULL);

// 	// อะไรวะ ?
// 	if (plen == 1 && (fmt->f_dot && fmt->precision == 0)
// 		&& (*str == '0' || *str == 0))
// 		fstr[i++] = ' ';

	
// 	while (i < fmt->width)
// 	{
// 		// printf("%c", str[i]);
// 		if (i < plen && (fmt->type == 's' && *str == 0))
// 			fstr[i] = ' ';
// 		 if (i < plen)
// 		{
// 			fstr[i] = str[i];
// 			printf("hi");
// 		}
			
// 		else
// 			fstr[i] = ' ';
// 		i++;
// 	}
// 	// free(str);
// 	return (fstr);
// }

// ##FT_IS
int	ft_isdigit(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	return (0);
}

int is_type(char c)
{
	int	i;

	i = 0;
	while(TYPES[i])
	{
		if (TYPES[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	is_flags(char c)
{
	int	i;
	
	i = 0;
	while(FLAGS[i])
	{
		if (FLAGS[i] == c)
			return (1);
		i++;
	}
	return (0);	
}


// ####################################

char	*plus_flag_str( t_list *t,char *str)
{
	int	nb;
	char *prefix;
	char *final_str;

	// number type and have + or space
	if((t->type == 'd' || t->type == 'i') && t->f_plus)
	{
		nb = ft_atoi(str);
		if(nb < 0)
			return (str); // เลขลบใช้ print ได้เลย
		prefix = ft_calloc(sizeof(char),2);
		if(prefix == NULL)
			return (NULL);
		if(t->f_plus)
			prefix[0] = '+';
		else
			prefix[0] = ' ';
		final_str = ft_strjoin(prefix,str); // DIFF from original
		if(final_str == NULL)
			return (NULL);
		free(prefix);
		free(str);
		return (final_str);
	}
	return (str);
}

// ############################################
// ################## GENERATE FINAL PRINT ####
// ############################################


// ############################################
// ################## PRINT EACH TYPE -> TO STR
// ############################################


// type c
int print_char(t_list *tp)
{
	char c;
	int	 i;
	c = va_arg(tp->ap, int);
	i = 0;

	if(tp->width && tp->f_minus) // %-2c 
	{
	
		ft_putchar_fd(c,1);
		i++;
		while(i < tp->width)
		{
			ft_putchar_fd(' ',1);
			i++;
		}
		tp->total_len += tp->width;
		return (1);
	}
	ft_putchar_fd(c,1);
	tp->total_len += 1;
	return (1);
}


char	*align_left(char *str,int width)
{
		int	i;
		int	len;
		char *p_str;

		i = 0;
		len = ft_strlen(str);
		// 	1.กรณีที่ str ยาวกว่า width
		// //  2.กรณีที่ str สั้นกว่า width
		// printf("len = %d\n",len);
		// printf("width = %d\n",width);
		if(len < width)
		{
			p_str = malloc(sizeof(char)*width+1);
			if(!p_str)
				return (NULL);
			while(i<width)
			{
				if(i < len)
					p_str[i] = str[i];
				else
					p_str[i] = ' ';
				i++;
			}
			free(str);
			return (p_str);
		}
		else
		{
			// p_str =ft_strdup(str);
			return (str);
		}
			
		
}

// type char *
int print_str(t_list *tp)
{
	char *s;
	char *f_str;
	int len;
	
	// len = 0;
	f_str = NULL;
	s = va_arg(tp->ap, char *);
	f_str = ft_strdup(s);
	if(s)
	{
		// printf("len : %zu\n",ft_strlen(s));
		if(tp->f_minus)
		{
			f_str = align_left(f_str,tp->width);
			ft_putstr_fd(f_str,1);
			tp->total_len += ft_strlen(f_str);
			free(f_str);
			return (1);
		}
			
		// printf("%zu",ft_strlen(f_str));
		ft_putstr_fd(f_str,1);
		len = ft_strlen(f_str);
		tp->total_len += len;
		free(f_str);
	} 
	else
	{
		ft_putstr_fd("(null)",4);
		len = 6;
	}
	tp->total_len += len;
	return (len);
}

// type unsigned long -> hex
int print_ptr_to_str(t_list *tp)
{
	size_t address;
	char	*address_str;
	int len;

	// len = 0;
	address = va_arg(tp->ap,size_t);
	address_str = ptr_to_str(address);
	ft_putstr_fd(address_str,1);
	len = (int)ft_strlen(address_str);
	free(address_str);
	tp->total_len += len;
	return (len);
}

// type d,i -> str (i,d)
int print_nbr_to_str(t_list *tp)
{
	int	nbr;
	char *s;
	int	len;

	// len = 0;
	nbr = va_arg(tp->ap, int);
	s = nbr_to_str_dec(nbr);
	// if(tp->f_plus)
	// {
		
	// 	s = plus_flag_str(tp,s);
	// }


	len = ft_strlen(s);
	ft_putstr_fd(s,1);
	free(s);
	tp->total_len += len;

	return (len);
}

// type unsigned int -> hex
int print_uint_to_strbase(t_list *tp, char type,unsigned int base)
{
	unsigned int nbr;
	char *s;
	int	len;
	
	nbr = va_arg(tp->ap, unsigned int);
	s = uint_to_str_base(nbr, type,base);
	len = ft_strlen(s);
	ft_putstr_fd(s,1);
	free(s);
	tp->total_len += len;
	return (len);
}

int	print_percent(t_list *tp)
{
	tp->total_len += write(1, "%",1);
	return (1);
}
void print_format(t_list *tp)
{
	char 	type;

	type = tp->type;
	// printf("type : %c\n",type);
	if(type == 'c')
			print_char(tp);
	else if(type == 's')
				print_str(tp);
	else if(type == 'p')
			print_ptr_to_str(tp);
	else if(type == 'd' || type == 'i')
			print_nbr_to_str(tp);
	else if(type == 'u')
			print_uint_to_strbase(tp,'d',10);
	else if(type == 'x')
			print_uint_to_strbase(tp,'x',16);
	else if(type == 'X')
			print_uint_to_strbase(tp,'X',16);
	else if(type == '%')
			print_percent(tp);
	
}

// #####################################################################################
// #####################################################################################
// #####################################################################################



// ############################################
// ################## table-print #############
// ############################################


void t_list_start(t_list *tp)
{
	tp->total_len = 0;
	tp->type = 0;
}
void t_list_set_type(t_list *tp,char type)
{
	tp->type = type;
}

void t_list_set_flag(t_list *tp, char flag)
{
	if(flag == '-')
		tp->f_minus = 1;
	else if(flag == '0')
		tp->f_zero = 1;
	else if(flag == '.')
		tp->f_dot = 1;
	else if(flag == '#')
		tp->f_hash = 1;
	else if(flag == ' ')
		tp->f_space = 1;
	else if(flag == '+')
		tp->f_plus = 1;
}
void t_list_reset_flags (t_list *tp)
{
	tp->f_minus = 0;
	tp->f_zero = 0;
	tp->f_dot = 0;
	tp->f_hash = 0;
	tp->f_space = 0;
	tp->f_plus = 0;
	tp->width = 0;
	tp->type = 0;
}

// FLAGS HANDLE

// DONE : PLUS and SPACE

// ############################################
// ################## core-function #############
// ############################################

int	ft_printf(const char *format, ...)
{
	int	i;
	int total_len;
	va_list ap; // ap : argument process
	t_list *tp;
	char flag;
	char d_str;
	char type;

	i = 0; 
	total_len = 0;
	tp = (t_list *)calloc(sizeof(t_list),1);
	if (!tp)
		return (-1);
	t_list_start(tp);
	va_start(tp->ap, format);

	while(*(format + i))
	{
		if(*(format + i) == '%')
		{
			i++;
			// printf("i = %d\n",i);
			while (tp->type == 0 && !is_type(*(format+i)))
			{	
				// printf("I = %d\n",i);
				if(is_flags(*(format+i)))
				{
					flag = *(format + i);
					t_list_set_flag(tp,flag);
				}
				else if(ft_isdigit(*(format + i)))
				{
					d_str = *(format+i);
					if(tp->f_dot)
						tp->precision = (tp->precision * 10) + (d_str - '0');
					else
						tp->width = (tp->width * 10) + (d_str - '0');
				}
				i++;
			}
			
			if(is_type(*(format + i)))
			{
				// printf("HI\n");
				type = *(format + i);
				t_list_set_type(tp,type);
				print_format(tp);	
				t_list_reset_flags(tp);
			}
		}
		else 
			total_len += write(1, format + i, 1);
		i++;
	}
	va_end(ap);
	total_len += tp->total_len;
	return (total_len);
}

// int main()
// {
	// int len;


	// # MAN-CHAR
	// printf("%c", '0');
	// printf("%d",ft_printf(" %c ", '0'));
	// printf(" %c", '0' - 256);
	// printf("%c ", '0' + 256);
	// printf(" %c %c %c ", '0', 0, '1');
	// printf(" %c %c %c ", ' ', ' ', ' ');
	// printf(" %c %c %c ", '1', '2', '3');
	// printf(" %c %c %c ", '2', '1', 0);
	// printf(" %c %c %c ", 0, '1', '2');

	// # MAN-STRING
	// char *s2 = "Mussum Ipsum, cacilds vidis litro abertis. Posuere libero varius. Nullam a nisl ut ante blandit hendrerit. Aenean sit amet nisi. Atirei o pau no gatis, per gatis num morreus.";
	
	// ft_printf("%s", "");
	// ft_printf(" %s", "");
	// ft_printf("%s ", "");
	// ft_printf(" %s ", "");
	// ft_printf(" %s ", "-");
	// ft_printf(" %s %s ", "", "-");
	// ft_printf(" %s %s ", " - ", "");
	// ft_printf(" %s %s %s %s %s", " - ", "", "4", "", s2);
	// ft_printf(" %s %s %s %s %s ", " - ", "", "4", "", "2 ");
	// ft_printf(" NULL %s NULL ", NULL);

	// ft_printf(" %p ",-1);
		// ft_printf(" %-s :\n", "-");
		// printf(" %-s ", "-");
		// printf("%d",len);
		// printf(" %-s ", "-");

	// TEST(1, print("%-1c", '0'));


	// int len = 0;
// 	len = ft_printf("%-1c:%-2c:%-3c:", '0', 0, '1');
// 	printf("len = %d\n",len);
// // TEST(5, print());
// 	len = printf("%-1c:%-2c:%-3c:", '0', 0, '1'); // 4sp + '0' ' 1 '
// 	printf("len = %d\n",len);

	// int len = 0;
	// len = ft_printf(":%1s", "");
	// printf("len = %d\n",len);
// TEST(5, print());
	// len = printf(":%1s", "");
	// printf("len = %d\n",len); 

	// TEST(11, print("%-1s", ""));


	// TEST(3, print(" %-3c", '0' - 256));
	// TEST(4, print("%-4c ", '0' + 256));
	// TEST(5, print(" %-1c %-2c %-3c ", '0', 0, '1'));
	// TEST(6, print(" %-1c %-2c %-3c ", ' ', ' ', ' '));
	// TEST(7, print(" %-1c %-2c %-3c ", '1', '2', '3'));
	// TEST(8, print(" %-1c %-2c %-3c ", '2', '1', 0));
	// TEST(9, print(" %-1c %-2c %-3c ", 0, '1', '2'));

//  }