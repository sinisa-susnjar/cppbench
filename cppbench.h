/**
 * \file
 * \author  Sinisa Susnjar <sinisa.susnjar@gmail.com>
 * \version 0.01
 */

#ifndef _cppbench_h
#define _cppbench_h

#include <functional>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <chrono>
#include <map>

/**
 * \brief The <code>cppbench</code> namespace contains structs, typedefs and functions to benchmark C++ code.
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

	/// runtime_result holds runtimes of multiple tests.
	typedef std::multimap<std::chrono::steady_clock::duration, std::string> runtime_result;
	/// compare_result holds runtime comparisons in percent for all tests.
	typedef std::multimap<std::chrono::steady_clock::duration, comparison> compare_result;

	/**
	 * \brief Benchmark the given tests.
	 * \param count how many times to call the given tests.
	 * \param func a vector containing the tests to run.
	 * \return A multimap with the runtimes of all tests.
	 */
	runtime_result
	time(int count, std::vector<cppbench::test> func)
	{
		runtime_result runtimes;
		for (auto const &e : func) {
			std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
			for (int i=0; i < count; e.func(), ++i);
			runtimes.insert(make_pair(std::chrono::steady_clock::now()-t, e.name));
		}
		return runtimes;
	}

	/*!
	 * \brief Compare runtimes.
	 * \param runtimes a multimap with the runtimes of all tests obtained by a call to time().
	 * \return A multimap with the runtime differences in percent of all tests.
	 */
	compare_result
	compare(const runtime_result &runtimes)
	{
		compare_result compare;
		for (auto const &a : runtimes) {
			comparison comp;
			comp.name = a.second;
			for (auto const &b : runtimes) {
				if (a.second == b.second) {
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

	/*!
	 * \brief Print runtimes to console.
	 * \param result a multimap with the runtimes of all tests obtained by a call to time().
	 * \tparam T an optional template parameter used to cast to the desired time granularity.
	 * \return A multimap with the runtime differences in percent for all tests.
	 */
	template<typename T = std::chrono::microseconds>
	void print(const runtime_result &result)
	{
		size_t w = 10;
		for (auto const &a : result)
			w = a.second.size() > w ? a.second.size()+1 : w;
		std::cout << std::setw(w) << " " << std::setw(w) << std::right << "runtime" << std::endl;
		for (auto const &a : result)
			std::cout << std::setw(w) << std::right << a.second
				<< std::setw(w) << std::chrono::duration_cast<T>(a.first).count() << std::endl;
	}

	/*!
	 * \brief Print runtime comparisons to console.
	 * \param compare a multimap with the runtime differences in percent for all tests.
	 * \tparam T an optional template parameter used to cast to the desired time granularity.
	 */
	template<typename T = std::chrono::microseconds>
	void print(const compare_result &compare)
	{
		size_t w = 10;
		for (auto const &a : compare)
			w = a.second.name.size() > w ? a.second.name.size()+1 : w;

		std::cout << std::setw(w) << " " << std::setw(w) << std::right << "runtime";
		for (auto const &a : compare)
			std::cout << std::setw(w) << std::right << a.second.name;
		std::cout << std::endl;

		for (auto const &a : compare) {
			std::cout << std::setw(w) << std::right << a.second.name << std::setw(w) << std::chrono::duration_cast<T>(a.first).count();
			for (auto const &p : a.second.pct) {
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
