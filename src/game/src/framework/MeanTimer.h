#pragma once

#include <chrono>
#include <sstream>

// I wrote this as I was getting wildly different performance figures for the same lot of raycast calculations each frame
// to try and smooth out the results. It seemed to be an issue with v-sync (maybe the OS switching contexts while waiting?)
// So this isn't needed any more, but it might be handy to have around at some point. So I will leave it here just in case. 

template<size_t NumIterations>
class MeanTimer
{
public:
	MeanTimer() = default;

	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	// return time as miliseconds
	float Stop()
	{
		std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
		results[counter++] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f;

		// wrap
		if (NumIterations == counter)
		{
			counter = 0;

			// recalc cache result
			cacheResult = 0.f;
			for (size_t i = 0; i < NumIterations; ++i)
			{
				cacheResult += results[i];
			}
			cacheResult /= NumIterations;
		}
		return cacheResult;
	}
	std::string AsString(size_t dp)
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(dp) << cacheResult;
		std::string ret = ss.str();
		return ret;
	}

private:
	size_t counter = 0;
	float results[NumIterations] = { 0.f };
	float cacheResult = 0.f;
	std::chrono::steady_clock::time_point start;
};