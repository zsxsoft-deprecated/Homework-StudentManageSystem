#include "stdafx.h"

int is_year_leap(int year) {
	return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

int max_day_of_month(int year, int month) {
	switch (month) {
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		return is_year_leap(year) ? 29 : 28;
	}
	return 31;
}


int is_date_legal(int year, int month, int day) {
	if (year > (2016 - 18) || year < (2016 - 60) || month <= 0 || month > 12 || day <= 0) return 0;
	return !(day > max_day_of_month(year, month));
}