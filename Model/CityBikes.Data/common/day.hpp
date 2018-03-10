#pragma once

#include <string>
#include <sstream>
#include <iomanip>

namespace CityBikes::Data::Common
{
	struct Day
	{
		int day;
		int month;
		int year;

		Day()
		{ }

		Day(tm time) :
			day(time.tm_mday),
			month(time.tm_mon + 1),
			year(time.tm_year + 1900)
		{ }

		bool operator<(const Day& second) const
		{
			if (year != second.year)
				return year < second.year;

			if (month != second.month)
				return month < second.month;

			return day < second.day;
		}

		bool operator!=(const Day& second) const
		{
			return day != second.day || month != second.month || year != second.year;
		}

		bool operator==(const Day& second) const
		{
			return day == second.day && month == second.month && year == second.year;
		}

		operator tm() const
		{
			tm time;

			time.tm_mday = day;
			time.tm_mon = month - 1;
			time.tm_year = year - 1900;

			std::mktime(&time);

			return time;
		}

		static std::string to_string(Day day)
		{
			std::stringstream dayString;
			dayString << day.year;
			dayString << "-";
			dayString << std::setw(2) << std::setfill('0') << day.month;
			dayString << "-";
			dayString << std::setw(2) << std::setfill('0') << day.day;

			return dayString.str();
		}
	};
}