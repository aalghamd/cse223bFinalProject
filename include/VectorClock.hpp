/*
 * VectorClock.hpp
 *
 *  Created on: May 19, 2013
 *      Author: aasghari
 */

#ifndef VECTORCLOCK_HPP_
#define VECTORCLOCK_HPP_
#include <string>
#include <iostream>
#include <map>
class VectorClock;
inline std::ostream& operator<<(std::ostream &out, const VectorClock &clock);

class VectorClock
{
	friend std::ostream& operator<<(std::ostream &out, const VectorClock &clock);
	std::string myid;
	std::map<std::string, int> clock;
public:
	VectorClock(const std::string& myid) :
			myid(myid)
	{
		this->clock[myid];
	}

	bool operator>(const VectorClock& other) const
	{
		VectorClock & nonConstMe=(VectorClock&)(*this);
		bool allGreaterOrEqu = true; //are all other <= mine
		bool strictGreater = false; //is at least one strictly less?
		//loop over other so if its bigger, we pick up its values
		for (std::map<std::string, int>::const_iterator it = other.clock.begin(); it != other.clock.end(); it++)
		{
			const std::string& clkid = it->first;
			int myval = nonConstMe.clock[clkid];
			int otherval = it->second;
			if (myval == 0)
			{
				//never seen this one before, copy its value
				nonConstMe.clock[clkid] = myval = otherval;
			}

			if (myval > otherval)
			{
				strictGreater = true;
			}

			if (myval < otherval)
			{
				allGreaterOrEqu = false;
			}
		}
		return strictGreater && allGreaterOrEqu;
	}
	bool operator<(const VectorClock &other) const
	{
		VectorClock & nonConstMe=(VectorClock&)(*this);
		bool allLessOrEqu = true;        //are all other <= mine
		bool strictLess = false; //is at least one strictly less?
		//loop over other so if its bigger, we pick up its values
		for (std::map<std::string, int>::const_iterator it = other.clock.begin(); it != other.clock.end(); it++)
		{
			const std::string& clkid = it->first;
			int myval = nonConstMe.clock[clkid];
			int otherval = it->second;

			if (myval < otherval)
			{
				strictLess = true;
			}

			if (myval > otherval)
			{
				allLessOrEqu = false;
			}
		}
		return strictLess && allLessOrEqu;
	}

	VectorClock& incMyClock()
	{
		this->clock[this->myid]++;
		return *this;
	}
	VectorClock& operator++()
	{
		// do actual increment
		return this->incMyClock();
	}
	VectorClock operator++(int)
	{
		VectorClock tmp(*this);
		operator++();
		return tmp;
	}
	void merge(VectorClock& other)
	{
		if (other.myid.size() <= 0)
			return; //this is a degenerate clock
		for (std::map<std::string, int>::const_iterator it = other.clock.begin(); it != other.clock.end(); it++)
		{
			const std::string& clkid = it->first;
			int myval = this->clock[clkid];
			int otherval = it->second;
			if (myval < otherval)
			{
				//never seen this one before, copy its value
				this->clock[clkid] = otherval;
			}

		}
	}
};

inline std::ostream& operator<<(std::ostream &out, const VectorClock &clock)
{
	// Since operator<< is a friend of the Point class, we can access
	// Point's members directly.
	out << "VecClock:" << clock.myid << " [";
	for (std::map<std::string, int>::const_iterator it = clock.clock.begin(); it != clock.clock.end(); it++)
	{
		out << " " << it->first << ":" << it->second;
	}
	out << " ]";

	return out;
}
#endif /* VECTORCLOCK_HPP_ */
