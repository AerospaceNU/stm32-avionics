// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license in the root directory of this project.

#include "sim_timing.h"

#include <stdint.h>

#include <atomic>
#include <chrono>

static std::atomic<uint64_t> programStartTime{0};
static std::atomic<uint64_t> programPauseTime{0};
static std::atomic<uint64_t> programStepTime{0};

uint64_t NowDefault();

void timing::RestartTiming() {
  programStartTime = NowDefault();
  programStepTime = 0;
  if (programPauseTime != 0) {
    programPauseTime = programStartTime.load();
  }
}

void timing::PauseTiming() {
  if (programPauseTime == 0) {
    programPauseTime = NowDefault();
  }
}

void timing::ResumeTiming() {
  if (programPauseTime != 0) {
    programStartTime += NowDefault() - programPauseTime;
    programPauseTime = 0;
  }
}

bool timing::IsTimingPaused() { return programPauseTime != 0; }

void timing::StepTimingAsync(uint64_t delta) { 
    programStepTime += delta; 
}

// offset in microseconds
static uint64_t time_since_epoch() noexcept {
#ifdef _WIN32
  FILETIME ft;
  uint64_t tmpres = 0;
  // 100-nanosecond intervals since January 1, 1601 (UTC)
  // which means 0.1 us
  GetSystemTimeAsFileTime(&ft);
  tmpres |= ft.dwHighDateTime;
  tmpres <<= 32;
  tmpres |= ft.dwLowDateTime;
  tmpres /= 10u;  // convert to us
  // January 1st, 1970 - January 1st, 1601 UTC ~ 369 years
  // or 11644473600000000 us
  static const uint64_t deltaepoch = 11644473600000000ull;
  tmpres -= deltaepoch;
  return tmpres;
#else
  // 1-us intervals
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
#endif
}

static uint64_t timestamp() noexcept {
#ifdef _WIN32
  LARGE_INTEGER li;
  QueryPerformanceCounter(&li);
  // there is an imprecision with the initial value,
  // but what matters is that timestamps are monotonic and consistent
  return static_cast<uint64_t>(li.QuadPart);
#else
  // 1-us intervals
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
#endif
}

static const uint64_t zerotime_val = time_since_epoch();
static const uint64_t offset_val = timestamp();

uint64_t NowDefault() {
#ifdef _WIN32
  assert(offset_val > 0u);
  assert(frequency_val > 0u);
  uint64_t delta = timestamp() - offset_val;
  // because the frequency is in update per seconds, we have to multiply the
  // delta by 1,000,000
  uint64_t delta_in_us = delta * 1000000ull / frequency_val;
  return delta_in_us + zerotime_val;
#else
  return zerotime_val + timestamp() - offset_val;
#endif
}

uint64_t timing::GetProgramTimeMicros() {
  uint64_t curTime = programPauseTime;
  if (curTime == 0) {
    curTime = NowDefault();
  }
  return curTime + programStepTime - programStartTime;
}

uint64_t timing::GetProgramTimeMillis() {
  return GetProgramTimeMicros() * 1.0e-3;
}

std::atomic<uint64_t> tickNumber(0);

uint64_t timing::GetTickNumber() {
    return tickNumber.load();
}

void timing::SetTickCompleted(uint64_t tick) {
    tickNumber.store(tick);
}