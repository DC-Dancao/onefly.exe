 
 

#ifndef BASE_TIME_H
#define BASE_TIME_H

#include <chrono>
#include <cstdint>
#include <ctime>

 \
\
\
\
\
\


 \
\
\
\
\
\


 \
\
\
\
\
\

void set_new_tick();

 \
\
\
\
\
\

std::chrono::nanoseconds time_get_nanoseconds();

 \
\
\
\
\
\
\
\
\
\

int64_t time_get_impl();

 \
\
\
\
\
\
\
\
\
\
\
\

int64_t time_get();

 \
\
\
\

constexpr int64_t time_freq()
{
	using namespace std::chrono_literals;
	return std::chrono::nanoseconds(1s).count();
}

 \
\
\
\
\
\

int64_t time_timestamp();

 \
\
\
\
\
\

int time_houroftheday();

 \
\
\
\

enum class ETimeSeason
{
	SPRING,
	SUMMER,
	AUTUMN,
	WINTER,
	EASTER,
	HALLOWEEN,
	XMAS,
	NEWYEAR,
};

 \
\
\
\
\
\

ETimeSeason time_season();

 \
\
\
\
\
\
\
\
\

void str_timestamp(char *buffer, int buffer_size);

 \
\
\
\
\
\
\
\
\
\
\

[[gnu::format(strftime, 3, 0)]] void str_timestamp_format(char *buffer, int buffer_size, const char *format);

 \
\
\
\
\
\
\
\
\
\
\
\

[[gnu::format(strftime, 4, 0)]] void str_timestamp_ex(time_t time, char *buffer, int buffer_size, const char *format);

 \
\
\
\
\
\
\
\
\
\

[[gnu::format(strftime, 2, 0)]] bool timestamp_from_str(const char *string, const char *format, time_t *timestamp);

 \
\
\
\
\
\
\
\

namespace TimestampFormat
{
	inline const char *const TIME = "%H:%M:%S";
	inline const char *const SPACE = "%Y-%m-%d %H:%M:%S";
	inline const char *const NOSPACE = "%Y-%m-%d_%H-%M-%S";
}

 \
\
\
\
\
\
\

enum class ETimeFormat
{
	DAYS,
	HOURS,
	MINS,
	HOURS_CENTISECS,
	MINS_CENTISECS,
	SECS_CENTISECS,
};

 \
\
\
\
\
\
\
\
\
\
\

int64_t time_milliseconds_from_seconds(float seconds);

 \
\
\
\
\
\
\
\
\
\
\

int str_time(int64_t centisecs, ETimeFormat format, char *buffer, int buffer_size);

 \
\
\
\
\
\
\
\
\
\
\
\
\

int str_time_float(float secs, ETimeFormat format, char *buffer, int buffer_size);

#endif
