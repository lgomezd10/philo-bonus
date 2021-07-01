#include "../includes/philo.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*str;

	if (s1 && s2)
	{
		len1 = ft_strlen(s1);
		len2 = ft_strlen(s2);
		str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
		if (str)
		{
			ft_strlcpy(str, s1, len1 + 1);
			ft_strlcpy(&str[len1], s2, len2 + 1);
		}
		return (str);
	}
	return (0);
}