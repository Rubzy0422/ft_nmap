/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcoetzer <rcoetzer@student.wethinkcode.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 14:39:40 by rcoetzer          #+#    #+#             */
/*   Updated: 2020/11/03 15:38:27 by rcoetzer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ft_nmap.h> 

int binary_search(int *a, int item, int low, int high) 
{
	int mid;
	if (high <= low)
		return (item > a[low])?  (low + 1): low; 
	mid = (int)(low + high)/2;
	if(item == a[mid])
		return mid+1; 
	if(item > a[mid])
		return binary_search(a, item, mid+1, high); 
	return binary_search(a, item, low, mid-1); 
}

void	binary_sort(int *a, int n) 
{ 
	int i;
	int loc;
	int j;
	int selected; 

	for (i = 1; i < n; ++i)
	{ 
		j = i - 1; 
		selected = a[i]; 
		loc = binary_search(a, selected, 0, j); 
		while (j >= loc) 
		{
			a[j+1] = a[j];
			j--; 
		} 
		a[j+1] = selected; 
	} 
}

int remove_duplicates(int *arr, int n) 
{
	int j = 0;

	for (int i = 0; i < n-1; i++) 
	{
		if (arr[i] != arr[i+1]) 
		{
			arr[j] = arr[i];
			j++;
		}
	}
	arr[j++] = arr[n-1];
	return j;
}
