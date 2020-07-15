/**
 * @file libcomp/src/TimerManager.cpp
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Class to manage timed events.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2012-2020 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TimerManager.h"

namespace libcomp {

class TimerEvent {
  friend class TimerEventComp;
  friend class TimerManager;

 protected:
  TimerEvent();
  ~TimerEvent();

  std::chrono::steady_clock::time_point time;
  std::chrono::milliseconds period;
  libcomp::Message::Execute *msg;
  bool mIsPeriodic;
};

}  // namespace libcomp

using namespace libcomp;

TimerEvent::TimerEvent() : msg(nullptr), mIsPeriodic(false) {}

TimerEvent::~TimerEvent() { delete msg; }

bool TimerEventComp::operator()(const TimerEvent *lhs,
                                const TimerEvent *rhs) const {
  return lhs->time < rhs->time;
}

TimerManager::TimerManager() : mRunning(true), mProcessingEvents(false) {
  mRunThread = std::thread([&]() {
#if !defined(EXOTIC_PLATFORM) && !defined(_WIN32) && !defined(__APPLE__)
    pthread_setname_np(pthread_self(), "timer");
#endif  // !defined(EXOTIC_PLATFORM) && !defined(_WIN32) && !defined(__APPLE__)

    while (mRunning) {
      std::unique_lock<std::mutex> lock(mEventLock);

      ProcessEvents(lock);
      WaitForEvent(lock);
    }
  });
}

TimerManager::~TimerManager() {
  mRunning = false;

  mEventCondition.notify_all();

  mRunThread.join();

  for (TimerEvent *pEvent : mEvents) {
    delete pEvent;
  }
}

void TimerManager::ProcessEvents(std::unique_lock<std::mutex> &lock) {
  auto now = std::chrono::steady_clock::now();

  std::list<TimerEvent *> periodicals;

  mProcessingEvents = true;

  while (!mEvents.empty()) {
    auto it = mEvents.begin();

    TimerEvent *pEvent = *it;

    if (pEvent->time > now) {
      break;
    } else {
      mEvents.erase(it++);

      if (pEvent->msg) {
        // Unlock the mutex in the case the callback waits on another
        // mutex that waits on the timer or the callback tries to
        // register a new timer event. We don't like deadlocks.
        lock.unlock();
        pEvent->msg->Run();
        lock.lock();
      }

      if (pEvent->mIsPeriodic) {
        pEvent->time += pEvent->period;

        periodicals.push_back(pEvent);
      } else {
        delete pEvent;
      }
    }
  }

  for (TimerEvent *pEvent : periodicals) {
    mEvents.insert(pEvent);
  }

  mProcessingEvents = false;
}

void TimerManager::WaitForEvent(std::unique_lock<std::mutex> &lock) {
  if (mEvents.empty()) {
    // Wait for at least one event.
    mEventCondition.wait(lock);
  } else {
    // We don't care why we wake we'll check everything anyway.
    (void)mEventCondition.wait_until(lock, (*mEvents.begin())->time);
  }
}

TimerEvent *TimerManager::RegisterEvent(
    const std::chrono::steady_clock::time_point &time,
    libcomp::Message::Execute *pMessage) {
  TimerEvent *pEvent = new TimerEvent;
  pEvent->time = time;
  pEvent->msg = pMessage;

  std::unique_lock<std::mutex> lock(mEventLock);

  mEvents.insert(pEvent);
  mEventCondition.notify_all();

  return pEvent;
}

TimerEvent *TimerManager::RegisterPeriodicEvent(
    const std::chrono::milliseconds &period,
    libcomp::Message::Execute *pMessage) {
  TimerEvent *pEvent = new TimerEvent;
  pEvent->time = std::chrono::steady_clock::now() + period;
  pEvent->period = period;
  pEvent->mIsPeriodic = true;
  pEvent->msg = pMessage;

  std::unique_lock<std::mutex> lock(mEventLock);

  mEvents.insert(pEvent);
  mEventCondition.notify_all();

  return pEvent;
}

void TimerManager::CancelEvent(TimerEvent *pEvent) {
  std::unique_lock<std::mutex> lock(mEventLock);

  // If the event is being processed we can't cancel but if the event
  // is periodic we can keep it from happening next time.
  if (mProcessingEvents) {
    pEvent->mIsPeriodic = false;
    return;
  }

  auto it = mEvents.find(pEvent);

  if (mEvents.end() != it) {
    mEvents.erase(it);

    delete pEvent;
  }

  mEventCondition.notify_all();
}
