#include "solver.h"
namespace mhle {

	solver::solver(int num)
		:m_equations(),
		m_num(num)
	{

	}
	void solver::add_equations(const std::initializer_list<fraction>& equation)
	{
		if (equation.size() != m_num + 1) {
			throw std::runtime_error("方程不完整");
			return;
		}
		auto it = equation.begin();
		while (it != equation.end() && *it == 0)++it;
		if (it == equation.end()) {
			throw std::runtime_error("谁TM写的全0方程");
			return;
		}
		fraction tmp = *it; ++it;
		while (it != equation.end() && tmp != 1) {
			if (*it!=0)tmp = gcd(tmp, *it);
			++it;
		}
		m_equations.push_back(std::vector<fraction>());
		for (auto& i : equation)m_equations.back().push_back(i / tmp);
	}
	void solver::add_equations(std::vector<fraction>::const_iterator begin,
							   std::vector<fraction>::const_iterator end)
	{
		auto it = begin;
		if (std::distance(it, end) != m_num + 1) {
			throw std::runtime_error("方程不完整");
			return;
		}
		while (it != end && *it == 0)++it;
		if (it == end) {
			throw std::runtime_error("谁TM写的全0方程");
			return;
		}
		auto tmp = *it; ++it;
		while (it != end && tmp != 1) {
			if (*it!=0)tmp = gcd(tmp, *it);
			++it;
		}
		m_equations.push_back(std::vector<fraction>());
		while (begin != end) {
			m_equations.back().push_back(*begin / tmp);
			++begin;
		}
	}
	void solver::solve(std::vector<fraction>& answer)
	{
		if (m_equations.size() == 1) {
			answer.push_back(m_equations[0][1] / m_equations[0][0]);
			return;
		}

		solver part_solver(m_num - 1);
		std::vector<fraction> ans, vtmp;

		auto it = m_equations.cbegin();
		while (it != m_equations.cend() && it->front() == 0) {
			part_solver.add_equations(it->cbegin() + 1, it->cend());
			++it;
		}
		if (it == m_equations.cend()) {
			throw std::runtime_error("方程组不合法");
			return;
		}
		auto tmp = it->front();
		auto it1 = it++;
		while (it != m_equations.cend()) {
			if (it->front()!=0) {
				auto l = lcm(tmp, it->front()),
					a = l / tmp, b = l / it->front();
				vtmp.clear();
				for (auto it2 = it1->cbegin() + 1, it3 = it->cbegin() + 1;
					 it2 != it1->cend(); ++it2,++it3) {
					vtmp.push_back((*it2) * a - (*it3) * b);
				}
				part_solver.add_equations(vtmp.cbegin(), vtmp.cend());
			}
			else part_solver.add_equations(it->cbegin() + 1, it->cend());
			++it;
		}
		try {
			part_solver.solve(ans);
			auto num = it1->back();
			for (auto it2 = it1->cbegin()+1, it3 = ans.cbegin();
				 it2 != it1->cend()-1; ++it2,++it3) {
				num -= (*it2) * (*it3);
			}
			answer.push_back(num / it1->front());
			answer.insert(answer.cend(), ans.cbegin(), ans.cend());
		}
		catch (std::exception e) {
			throw e;
			return;
		}

	}
}
