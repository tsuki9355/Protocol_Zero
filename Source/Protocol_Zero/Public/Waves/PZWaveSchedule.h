// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Release timing for one wave's spawn groups, with no engine dependencies so it can be
 * unit-tested. Each group releases a fixed delay after the previous one (the first group's
 * delay counts from the wave start), so enemies arrive in small timed groups rather than
 * all at once. Time is passed in by the caller.
 *
 * Example:
 *   FPZWaveSchedule Schedule({ 0.f, 4.f, 4.f });
 *   Schedule.Start(Now);
 *   TArray<int32> Due;
 *   Schedule.ConsumeDueGroups(Now, Due); // call every tick; spawn each index in Due
 */
struct PROTOCOL_ZERO_API FPZWaveSchedule
{
	/** @param InGroupDelays Seconds each group waits after the previous group (or the wave start). */
	explicit FPZWaveSchedule(TArray<float> InGroupDelays = {});

	/** Starts the wave at Now. Calling it again restarts the schedule. */
	void Start(double Now);

	/**
	 * Appends the index of every group due by Now that has not been released yet, in order.
	 * Several groups can be due at once after a long frame; none are skipped.
	 * @return Number of groups released by this call.
	 */
	int32 ConsumeDueGroups(double Now, TArray<int32>& OutDueGroups);

	/** True once every group has been released. A wave with no groups is finished at start. */
	bool IsFinished() const { return bStarted && NextGroup >= GroupDelays.Num(); }

	/** Number of groups in the wave. */
	int32 NumGroups() const { return GroupDelays.Num(); }

	/** Seconds after the wave start at which the given group releases. */
	double GetReleaseOffset(int32 GroupIndex) const;

private:
	TArray<float> GroupDelays;
	double StartTime = 0.0;
	double NextReleaseTime = 0.0;
	int32 NextGroup = 0;
	bool bStarted = false;
};
