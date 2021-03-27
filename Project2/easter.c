/***
This file calculates the date for Easter on any given year between the years
1582 and 39999. It can take an input from the terminal, or it can read a file
input and return the date for each year in the file.
***/

#include <stdio.h>

int calculate_Easter_date(int year);

/***
main is a function that will run the calculate_Easter_date function on a
given input. It will print "year - month day" where the year is the input
and the month - day represent the date of Easter. For an input file, it will
do this for every year listed in the file on its own line.
***/


int main(void)
{
  int year, date, result;
  while (1)
  {
    result = scanf("%d", &year);
    if (result == EOF)
    {
      break;
    }
    date = calculate_Easter_date(year);
    if (date == 0)
    {
      fprintf(stderr, "input year out of range!\n");
    }
    else
    {
      if (date < 0)
      {
        date = -date;
        printf("%d - March %d\n", year, date);
        continue;
      }
      if (date > 0)
      {
        printf("%d - April %d\n", year, date);
      }
    }
  }
}

/***
calculate_Easter_date is a function that uses Donald Knuth's algorithm in
order to calculate the date of Easter on a given year.

Arguments:

year --> the year for which the date of Easter will be calculated

Return:

int --> the day on which easter will take place, negative values denote dates
        in March and positive values denote dates in April
***/

int calculate_Easter_date(int year)
{
  /***
  This function uses a number of variables in order to carry out Knuth's
  algorithm

  Variables:

  gold = the Golden Year in the 19-year Metonic cycle
  century = which century the input year is in
  leap = the number of skipped leap years
  corr = a correction factor that accounts for the moon's orbit
  sunday = number that tells which day in March will be a Sunday
  epact = number that tells when a full moon occurs
  days = number of days after March first that Easter will take place

  Note: Easter day always takes place on the first Sunday after the first
        full moon after the vernal equinox.

  The function uses the input year to calculate the above variables, which give
  enough information to determine when Easter will take place.
  ***/
  int gold, century, leap, corr, sunday, epact, days;
  if (year < 1582 || year > 39999)
  {
    return 0;
  }
  gold = (year % 19) + 1;
  century = (year / 100) + 1;
  leap = (3 * century / 4) - 12;
  corr = ((8 * century + 5) / 25) - 5;
  sunday = (5 * year / 4) - leap - 10;
  epact = (11 * gold + 20 + corr - leap) % 30;
  epact = (epact + 30) % 30
  if (epact == 25 && gold > 11)
  {
    epact++;
  }
  if (epact == 24)
  {
    epact++;
  }
  days = 44 - epact;
  if (days < 21)
  {
    days += 30;
  }
  days = days + 7 - ((sunday + days) % 7) - 31;
  if (days > 0)
  {
    return days;
  }
  else
  {
    return -(31 + days);
  }
}
