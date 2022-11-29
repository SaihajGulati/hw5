

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t dayIndex,
    size_t workerIndex
);

bool WorkerValid(
    const size_t maxShifts,
    DailySchedule& sched,
    size_t person);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    //initalize first vector in first row so can work with in helper function without seg fault
    vector<Worker_T> temp;
    sched.push_back(temp);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, 0, 0);
}

/**
 * @brief 
 * does the main work of making the schedule
 * I had to make a helper function to include the last two parameters
 * @param avail: availability matrix
 * @param dailyNeed: amount need per day
 * @param maxShifts : max shifts allowed for each employee
 * @param sched : the current schedule as it is built out
 * @param dayIndex : index of the day we are working with in sched matrix
 * @param workerIndex : index of which worker start at in AVAILABILITY ARRAY
 * @return boolean over whether was successful or not 
 */

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t dayIndex,
    size_t workerIndex
)
{
    //if the dayIndex is the last row and the we are at the last possible column spot on that last row, we are done and so return true
    if (dayIndex == avail.size()-1 && sched[dayIndex].size() == dailyNeed)
    {
				return true;
    }

    //if we are at the last possible column spot for the row we're looking at, 
    //then we have reached end of sched row and so need to move onto the first of next row 
    if (sched[dayIndex].size() == dailyNeed)
    {
        dayIndex++;
        workerIndex = 0;
    }

  //for loop to try every option for every worker for this day in availability matrix
  for (size_t  i = workerIndex; i < avail[dayIndex].size(); i++)
  {
    //check if the worker is available on this day, and then if can be scheduled for another shift
    if (avail[dayIndex][i] && WorkerValid(maxShifts, sched, i))
    {
				//check if dayIndex is the size of sched, usually because moved above
				//useful to put it here only when will be used, so don't have to worry about deleting row
				//otherwise with it above, when recurse in, already created it and so when recurse and come out, when get back to that dayIndex, you'll end up adding a row again
				if (dayIndex == sched.size())
				{
					vector<Worker_T> temp;
					sched.push_back(temp);
				}
				//add the worker number to the schedule for now
        sched[dayIndex].push_back(i);
				//recursively check if end up with a valid combination
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, dayIndex, workerIndex+1))
        {
            return true;
        }
        //if get here, did not find a way and so need to backtrack and try next number, so change back to way it was before with smaller vector
        sched[dayIndex].pop_back();
    }
  }
  return false;
}

//helper function to return whether can add a shift for this employee (returns true only if the number of person's shifts in the sched are less than maxShifts)
//takes in maxShifts employees can have, current schedule, and the person (employee) we're thinking of adding in
bool WorkerValid(
    const size_t maxShifts,
    DailySchedule& sched,
    size_t person)
{
    //variable to count occurences of employee in sched (because need less than maxShifts)
    size_t count = 0;
    //outer for loop goes through the days in the array
    for (size_t day = 0; day < sched.size(); day++)
    {
        //inner for loop goes through the workers
        for (size_t worker = 0; worker < sched[day].size(); worker++)
        {
            if(sched[day][worker] ==  person)
            {
                count++;
            }
        }

        //as soon as figure out that have more than needed, exit
        if (count >= maxShifts)
        {
            return false;
        }
    }

    //if get here, did not find enough occurences of employee in the current schedu (count < maxShifts), 
    //so return true as it is ok if a shift for this worker is added
    return true;
}


