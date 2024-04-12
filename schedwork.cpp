#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool isValid(int row, int schedCol, const size_t maxShifts, DailySchedule& sched, const size_t dailyNeed, unordered_map<int,int>& worker_shifts, unordered_map<int,int>& day_shifts);

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int rowIdx,
    int colIdx,
     unordered_map<int,int>& worker_shifts,
     unordered_map<int,int>& day_shifts);
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
    sched.resize(avail.size());
    // map between worker ids and the number of shifts they get are scheduled
   unordered_map<int, int> worker_shifts;
   int numWorkers = avail[0].size();
   for(int i = 0; i < numWorkers; i++){
    worker_shifts[i] = 0;
   }
   // map between day number and the number of shifts on that day
   unordered_map<int, int> day_shifts;
   for(int i = 0; i < avail.size(); i++){
    day_shifts[i] = 0;
   }
    // Add your code below
   return scheduleHelper(avail, dailyNeed, maxShifts, sched, 0, 0, worker_shifts, day_shifts);
    
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int rowIdx,
    int colIdx, 
     unordered_map<int,int>& worker_shifts,
     unordered_map<int,int>& day_shifts){


  // if full return true 
  if(rowIdx == avail.size()){
    
    return true;
  }

  // number of workers to check (how many different workers there are)
  unsigned int numWorkers = avail[0].size();
  // iterate through all these workers
  for(unsigned int i =0; i < numWorkers; i++){
    
    // worker not available so don't even bother trying them
    if(avail[rowIdx][i] == 0){
        continue;
    }
    // worker already max scheduled so also don't bother
    // if(worker_shifts[i] > maxShifts){
    //      continue;
    //  }
    // //  skip because max reached for daily need
    //  if(day_shifts[rowIdx] >= dailyNeed){
    //      continue;
    //  }
    
    // add worker to the schedule
    sched[rowIdx].push_back(i);
    worker_shifts[i] += 1;
    day_shifts[rowIdx] += 1;
    // check if this was a valid addition
    if(isValid(rowIdx, colIdx, maxShifts, sched, dailyNeed, worker_shifts, day_shifts)){
      // move on to next row
      int nextRow, nextCol;
      // determine next parameters
      if(colIdx == dailyNeed - 1){
        nextRow = rowIdx + 1;
        nextCol = 0;
      } else {
        nextCol = colIdx + 1; 
        nextRow = rowIdx;
      }
      // recurse
      bool res = scheduleHelper(avail, dailyNeed, maxShifts, sched, nextRow, nextCol, worker_shifts, day_shifts);
      if(res){
        return true;
      }
    }
    // remove worker from schedule and try next worker
    sched[rowIdx].pop_back();
    worker_shifts[i] -= 1;
    day_shifts[rowIdx] -= 1;
  }
  return false;
}

// check if adding worker id to schedule is valid 
// based on maxShifts, dailyNeed and availability
bool isValid(int row, int schedCol, const size_t maxShifts, DailySchedule& sched, const size_t dailyNeed, unordered_map<int,int>& worker_shifts, unordered_map<int,int>& day_shifts){
  
   // figure out worker's id number
  int worker_id = sched[row][schedCol];
  // extract that worker from the map
  int numShifts = worker_shifts[worker_id];
  // determine if that worker is scheduled too many times
  if(numShifts > maxShifts){
    return false;
  }
  // determine if there are too many shifts on that day
  if(day_shifts[row] > dailyNeed){
    return false;
  }
  
  // all conditions passed, return true
  return true;

}

