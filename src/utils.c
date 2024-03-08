#include "malloc.h"

void	ft_bzero(void *s, size_t n)
{
	char	*str;

	str = s;
	while (n-- > 0)
		*str++ = '\0';
}


int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (ft_putstr("(null)"));
	while (str[i])
		i++;
	if (i > 0)
		write(1, str, i);
	return (i);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*tmp_dest;
	const char	*tmp_src;

	if (!dest && !src)
		return (NULL);
	tmp_dest = dest;
	tmp_src = src;
	while (n--)
		*tmp_dest++ = *tmp_src++;
	return (dest);
}

int	ft_putchar(int c)
{
	int	size;

	size = (int)write(1, &c, 1);
	return (size);
}

int	ft_putnbr_unsigned(unsigned int nbr)
{
	int	len;

	len = 0;
	if (nbr >= 10)
		len += ft_putnbr_unsigned(nbr / 10);
	nbr %= 10;
	nbr += '0';
	len += ft_putchar(nbr);
	return (len);
}

unsigned long	ft_putnbr_long_unsigned(unsigned long nbr)
{
	unsigned long	len;

	len = 0;
	if (nbr >= 10)
		len += ft_putnbr_long_unsigned(nbr / 10);
	nbr %= 10;
	nbr += '0';
	len += ft_putchar(nbr);
	return (len);
}

int	ft_putnbr(int nbr)
{
	int	len;

	len = 0;
	if (nbr <= -2147483648)
	{
		len = ft_putstr("-2147483648");
		return (len);
	}
	if (nbr < 0)
	{
		len += ft_putchar((int) '-');
		nbr *= -1;
	}
	if (nbr >= 10)
		len += ft_putnbr(nbr / 10);
	nbr %= 10;
	nbr += '0';
	len += ft_putchar(nbr);
	return (len);
}

int	ft_puthex(int option, unsigned long long nbr)
{
	char	*tab_hex;
	char	current;
	int		len;

	tab_hex = "0123456789abcdef";
	len = 0;
	if (nbr >= 16)
		len += ft_puthex(option, nbr / 16);
	nbr %= 16;
	current = tab_hex[nbr];
	if (option && current >= 'a' && current <= 'f' )
		current -= 32;
	len += ft_putchar(current);
	return (len);
}



int	ft_putptr(unsigned long long ptr)
{
	int	len;

	len = ft_putstr("0x");
	len += ft_puthex(0, ptr);
	return (len);
}





int	ft_print_format(va_list va, char format)
{
	int	len;

	len = 0;
	if (format == 'c')
		len = ft_putchar((int)va_arg(va, int));
	else if (format == 's')
		len = ft_putstr((char *)va_arg(va, char *));
	else if (format == 'p')
		len = ft_putptr((unsigned long long)va_arg(va, unsigned long long));
	else if (format == 'd' || format == 'i')
		len = ft_putnbr((int)va_arg(va, int));
	else if (format == 'u')
		len = ft_putnbr_unsigned((unsigned long long)va_arg(va, unsigned int));
	else if (format == 'l')
		len = ft_putnbr_long_unsigned((unsigned long long)va_arg(va, unsigned long));
	else if (format == 'x')
		len = ft_puthex(0, (unsigned long long)va_arg(va, unsigned int));
	else if (format == 'X')
		len = ft_puthex(1, (unsigned long long)va_arg(va, unsigned int));
	else if (format == '%')
		len = ft_putchar((int) '%');
	return (len);
}

int	 ft_printf(const char *format, ...)
{
	va_list	va;
	size_t	i;
	int		len_write;

	va_start(va, format);
	i = 0;
	len_write = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			len_write += ft_print_format(va, format[i + 1]);
			i++;
		}
		else
			len_write += ft_putchar(format[i]);
		i++;
	}
	va_end(va);
	return (len_write);
}