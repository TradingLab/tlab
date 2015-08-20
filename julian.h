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
// http://codereview.stackexchange.com/questions/38893/calculating-julian-dates
#ifndef JULIAN_H
#define JULIAN_H
#include <ctime>

class julian {
public:
     /* Default constructor: today's date */
     julian();
// 
//     Julian(int year, int month, int day) {
//         // TODO
//     }

    int calc_julianToday();
    int calc_juliandate(int, int, int);
    int dates_diff(int, int, int);
    
private:
    long date;
};

#endif // JULIAN_H
