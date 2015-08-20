/*
 * Copyright 2015 Jordi Estrada <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "julian.h"

julian::julian() {
}

int julian::calc_julianToday()
{
    int todDay, todMonth, todYear, julToday;

    time_t t = time(0);
    struct tm *now = localtime( &t );
    todDay = now->tm_mday;
    todMonth = now->tm_mon+1;
    todYear = now->tm_year+1900;

    int at = (14 - todMonth) / 12;
    int yt = todYear + 4800 - at;
    int mt = todMonth + 12 * at - 3;
    if (todYear > 1582 || (todYear == 1582 && todMonth > 10) ||
            (todYear == 1582 && todMonth == 10 && todDay >= 15))
        return julToday =  todDay + (153 * mt + 2) / 5 + 365 * yt
                           + yt / 4 - yt / 100 + yt / 400 - 32045;
    else
        return julToday = todDay + (153 * mt + 2) / 5 + 365 * yt + yt / 4 - 32083;

}
int julian::calc_juliandate(int day, int month, int year)
{
    int julStart;

    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    if (year > 1582 || (year == 1582 && month > 10) ||
            (year == 1582 && month == 10 && day >= 15))
        return julStart = day + (153 * m + 2) / 5
                          + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    else
        return julStart = day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083;

}

int julian::dates_diff(int day, int month, int year)
{
    int start = calc_juliandate(day, month, year);
    int today = calc_julianToday();
    int differ = today-start;
    return differ;
}

