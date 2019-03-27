/**
 * \file	cppbench.h
 * \author  Sinisa Susnjar <sinisa.susnjar@gmail.com>
 * \version 0.01
 */

#ifndef _cppbench_h
#define _cppbench_h

#include <functional>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <limits>
#include <cmath>
#include <map>

/**
 * \brief The cppbench> namespace contains structs, typedefs and functions to benchmark C++ code.
 */
namespace cppbench {

	/// Stores the test name and comparisons of all runtimes in percent.
	typedef struct {
		/// Test name.
		std::string name;
		/// Percent runtime differences with other tests.
		std::vector<double> pct;
	} comparison;

	/// Stores the name and code for a test.
	typedef struct {
		/// Test name.
		std::string name;
		/// Test code.
		std::function<void()> func;
	} test;

	/// Stores the results for a test.
	typedef struct {
		/// Test name.
		std::string name;
		/// Minimal recorded runtime.
		double min;
		/// Maximal recorded runtime.
		double max;
		/// Average (mean) runtime.
		double avg;
		/// Runtime variance.
		double var;
		/// Runtime standard deviation.
		double dev;
		/// Distribution of runtimes with std::chrono::high_resolution_clock resolution.
		std::map<long long, size_t> dist;
	} result;

	/// runtime_result holds runtimes of multiple tests.
	typedef std::multimap<std::chrono::high_resolution_clock::duration, result> runtime_result;
	/// compare_result holds runtime comparisons in percent for all tests.
	typedef std::multimap<std::chrono::high_resolution_clock::duration, comparison> compare_result;

	/**
	 * \brief Benchmark the given tests.
	 * \param count How many times to call the given tests.
	 * \param func A vector containing the tests to run.
	 * \return A multimap with the runtimes of all tests.
	 */
	runtime_result
	time(int count, std::vector<cppbench::test> func)
	{
		runtime_result runtimes;
		for (const auto &e : func) {
			result r;
			r.name = e.name;
			r.min = std::numeric_limits<double>::max();
			r.max = 0;
			r.avg = 0;

			// compute running mean, variance and standard deviation (TAOCP, volume 2, 3rd edition, page 232)
			double old_avg = 0, old_var = 0;

			std::chrono::high_resolution_clock::duration total = std::chrono::high_resolution_clock::duration::zero();
			for (int i=0; i < count; ++i) {
				std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
				e.func();
				std::chrono::high_resolution_clock::duration d = std::chrono::high_resolution_clock::now() - t;
				total += d;
				long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();
				r.avg += duration;
				if (duration < r.min) r.min = duration;
				if (duration > r.max) r.max = duration;
				r.dist[duration]++;
				if (i == 0) {
					old_avg = r.avg = duration;
				} else {
					r.avg = old_avg + (duration - old_avg) / (i + 1);
					r.var = old_var + (duration - old_avg) * (duration - r.avg);
					old_avg = r.avg; 
					old_var = r.var;
				}
			}
			r.var = count > 1 ? r.var / (count - 1) : 0;
			r.dev = sqrt(r.var);
			runtimes.insert(make_pair(total, r));
		}
		return runtimes;
	}

	/**
	 * \brief Compare runtimes.
	 * \param runtimes A multimap with the runtimes of all tests obtained by a call to time().
	 * \return A multimap with the runtime differences in percent of all tests.
	 */
	compare_result
	compare(const runtime_result &runtimes)
	{
		compare_result compare;
		for (const auto &a : runtimes) {
			comparison comp;
			comp.name = a.second.name;
			for (const auto &b : runtimes) {
				if (a.second.name == b.second.name) {
					comp.pct.emplace_back(0);
					continue;
				}
				double d0 = std::chrono::duration_cast<std::chrono::nanoseconds>(a.first).count();
				double d1 = std::chrono::duration_cast<std::chrono::nanoseconds>(b.first).count();
				comp.pct.emplace_back((d1 - d0) / d1 * 100.0);
			}
			compare.insert(std::make_pair(a.first, comp));
		}
		return compare;
	}

	/**
	 * \brief Print runtimes to console.
	 * \param result A multimap with the runtimes of all tests obtained by a call to time().
	 * \param prec Numerical precision for floating value, default is 2.
	 * \param w Minimum column width in characters, default is 10.
	 * \tparam T An optional template parameter used to cast to the desired time resolution.
	 */
	template<typename T = std::chrono::microseconds>
	void print(const runtime_result &result, int prec = 2, size_t w = 10)
	{
		for (const auto &a : result)
			w = a.second.name.size() > w ? a.second.name.size()+1 : w;
		std::cout << std::setw(w) << " " << std::setw(w) << std::right << "runtime"
				<< std::setw(w) << "min" << std::setw(w) << "max" << std::setw(w) << "avg"
				<< std::setw(w) << "var" << std::setw(w) << "dev" << std::endl;
		double conv = std::chrono::high_resolution_clock::period::den/T::period::den;
		for (const auto &a : result)
			std::cout << std::setw(w) << std::right << a.second.name
				<< std::setw(w) << std::chrono::duration_cast<T>(a.first).count()
				<< std::setw(w) << std::fixed << std::setprecision(prec) << (a.second.min/conv)
				<< std::setw(w) << std::fixed << std::setprecision(prec) << (a.second.max/conv)
				<< std::setw(w) << std::fixed << std::setprecision(prec) << (a.second.avg/conv)
				<< std::setw(w) << std::fixed << std::setprecision(prec) << (a.second.var/(conv*conv))
				<< std::setw(w) << std::fixed << std::setprecision(prec) << (a.second.dev/conv)
				<< std::endl;
	}

	/**
	 * \brief Write runtimes to a set of files using parameter "base" as root of the file names.
	 * \param base The base name to use.
	 * 		Final filename(s) will be named base + "-" + test_name + ".txt". This file will
	 * 		have a format of "runtime min max avg variance std-dev".
	 * 		A second file containing the actual distribution of runtimes will be named
	 * 		base + "-" + test_name + "-dist.txt", this file will have the format
	 * 		"runtime no-of-occurences", where "runtime" is measured in
	 * 		std::chrono::high_resolution_clock resolution.
	 * \param result A multimap with the runtimes of all tests obtained by a call to time().
	 * \param delim What delimiter character to use, default is tab.
	 * \param prec Numerical precision for floating value, default is 5.
	 * \return true on success, false if the files could not be written.
	 * \tparam T An optional template parameter used to cast to the desired time resolution.
	 */
	template<typename T = std::chrono::microseconds>
	bool write(const std::string &base, const runtime_result &result, char delim = '\t', int prec = 5)
	{
		double conv = std::chrono::high_resolution_clock::period::den / T::period::den;
		for (const auto &a : result) {
			std::ofstream res(base + "-" + a.second.name + ".txt");
			res << std::chrono::duration_cast<T>(a.first).count() << delim
				<< std::fixed << std::setprecision(prec) << (a.second.min/conv) << delim
				<< std::fixed << std::setprecision(prec) << (a.second.max/conv) << delim
				<< std::fixed << std::setprecision(prec) << (a.second.avg/conv) << delim
				<< std::fixed << std::setprecision(prec) << (a.second.var/conv) << delim
				<< std::fixed << std::setprecision(prec) << (a.second.dev/conv)
				<< std::endl;
			if (!res.good()) return false;
			std::ofstream dist(base + "-" + a.second.name + "-dist.txt");
			for (const auto &s : a.second.dist) {
				dist << s.first << delim << s.second << std::endl;
				if (!dist.good()) return false;
			}
		}
		return true;
	}

	/**
	 * \brief Print runtime comparisons to console.
	 * \param compare A multimap with the runtime differences in percent for all tests.
	 * \tparam T An optional template parameter used to cast to the desired time resolution.
	 */
	template<typename T = std::chrono::microseconds>
	void print(const compare_result &compare)
	{
		size_t w = 10;
		for (const auto &a : compare)
			w = a.second.name.size() > w ? a.second.name.size()+1 : w;

		std::cout << std::setw(w) << " " << std::setw(w) << std::right << "runtime";
		for (const auto &a : compare)
			std::cout << std::setw(w) << std::right << a.second.name;
		std::cout << std::endl;

		for (const auto &a : compare) {
			std::cout << std::setw(w) << std::right << a.second.name << std::setw(w) << std::chrono::duration_cast<T>(a.first).count();
			for (const auto &p : a.second.pct) {
				if (p == 0) {
					std::cout << std::setw(w) << "--";
					continue;
				}
				std::cout << std::setw(w-1) << std::fixed << std::setprecision(2) << p << "%" << std::setprecision(0);
			}
			std::cout << std::endl;
		}
	}

}

#endif // _cppbench_h
