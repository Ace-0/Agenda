#include "Date.hpp"

Date::Date() {
	m_year = m_month = m_day = m_hour = m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}
Date::Date(std::string dateString) {
  	Date temp = stringToDate(dateString);
  	m_year = temp.m_year;
  	m_month = temp.m_month;
  	m_day = temp.m_day;
  	m_hour = temp.m_hour;
  	m_minute = temp.m_minute;
  }
int Date::getYear(void) const { return m_year; }

void Date::setYear(const int t_year) { this->m_year = t_year; }

int Date::getMonth(void) const { return m_month; }

void Date::setMonth(const int t_month) { this->m_month = t_month; }

int Date::getDay(void) const { return m_day; }

void Date::setDay(const int t_day) { this->m_day = t_day; }

int Date::getHour(void) const { return m_hour; }

void Date::setHour(const int t_hour) { this->m_hour = t_hour; }

int Date::getMinute(void) const { return m_minute; }

void Date::setMinute(const int t_minute) { this->m_minute = t_minute; }

bool Date::isValid(const Date t_date) {
	int run_days[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
	int ping_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if (t_date.m_year < 1000 || t_date.m_year > 9999)
		return false;
	if (t_date.m_month <= 0 || t_date.m_month > 12)
		return false;
	if ((t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0) || (t_date.m_year % 400 == 0)) {
		if (t_date.m_day <= 0 || t_date.m_day > run_days[t_date.m_month - 1])
			return false;
	} else {
		if (t_date.m_day <= 0 || t_date.m_day > ping_days[t_date.m_month - 1])
			return false;
	}
	if (t_date.m_hour < 0 || t_date.m_hour > 23)
		return false;
	if (t_date.m_minute < 0 || t_date.m_minute > 59)
		return false;
	return true;
}

Date Date::stringToDate(const std::string t_dateString) {
//	Date(string）构造函数跟StringToDate一样，
//	只关注传入字符串的格式是否符合yyyy-mm-dd/hh:mm
	bool flag = true;

	if (t_dateString.size() != 16)
		flag = false;

	if (t_dateString[4] != '-' || t_dateString[7] != '-'
		|| t_dateString[10] != '/' || t_dateString[13] != ':')
		flag = false;

    int check[12] = {0,1,2,3,5,6,8,9,11,12,14,15};
	for (int i = 0; i < 12; i++)
		if (t_dateString[check[i]] < '0' || t_dateString[check[i]] > '9')
			flag = false;
	
	if (flag == false)
		return Date(0,0,0,0,0);

	int t_year = 0;
  	for (int i = 0; i != 4; i++) {
  		t_year *= 10;
  		t_year += t_dateString[i] - '0';
  	}
  	int t_month = (t_dateString[5] - '0') * 10 + t_dateString[6] - '0';
  	int t_day = (t_dateString[8] - '0')* 10 + t_dateString[9] - '0';
  	int t_hour = (t_dateString[11] - '0') * 10 + t_dateString[12] - '0';
  	int t_minute = (t_dateString[14] - '0') * 10 + t_dateString[15] - '0';
  	Date ans(t_year, t_month, t_day, t_hour, t_minute);
  	return ans;
  	// if (isValid(ans))
  	// 	return ans;
  	// else
  	// 	return Date(0,0,0,0,0);
  }

std::string Date::dateToString(Date t_date) {
	std::string ans = "0000-00-00/00:00";
	if (isValid(t_date)) {
		int t_year = t_date.m_year;
		for (int i = 3; i > -1; i--) {
			ans[i] = t_year % 10 + '0';
			t_year /= 10;		
		}
		ans[5] = t_date.m_month / 10 + '0';
		ans[6] = t_date.m_month % 10 + '0';
		ans[8] = t_date.m_day / 10 + '0';
		ans[9] = t_date.m_day % 10 + '0';
		ans[11] = t_date.m_hour / 10 + '0';
		ans[12] = t_date.m_hour % 10 + '0';
		ans[14] = t_date.m_minute /10 + '0';
		ans[15] = t_date.m_minute % 10 + '0';
		return ans;
	} else {
		return ans;
	}
}

Date & Date::operator=(const Date &t_date) {
	if (this == &t_date) {
		return *this;
	} else {
		m_year = t_date.m_year;
		m_month = t_date.m_month;
		m_day = t_date.m_day;
		m_hour = t_date.m_hour;
		m_minute = t_date.m_minute;
		return *this;
	}
}

bool Date::operator==(const Date &t_date) const {
	return dateToString(*this) == dateToString(t_date);
}

bool Date::operator>(const Date &t_date) const {
	return dateToString(*this) > dateToString(t_date);
}

bool Date::operator<(const Date &t_date) const {
	return dateToString(*this) < dateToString(t_date);
}

bool Date::operator>=(const Date &t_date) const {
	return dateToString(*this) >= dateToString(t_date);
}

bool Date::operator<=(const Date &t_date) const {
	return dateToString(*this) <= dateToString(t_date);
}
