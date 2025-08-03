#pragma once
#include "svg.hpp"
#include <vector>
#include <map>
#include <set>
#include <string>
#include  <type_traits>
#include  <cassert>

namespace svg {
class Base {
public:
	virtual void Emplace(ObjectContainer& cont) const = 0;
	Point GetCenter() const {
		return center;
	}
	void SetCenter(Point pt) {
		center = pt;
	}
protected:
	Point center;
};

class WireLabel : public Base{
	std::string label;
public:
	WireLabel(Point pt,std::string name) {
		center = pt;
		label = name;
	}
	WireLabel() {
		std::cerr << "WireLabel() why im here?" << std::endl;
		assert(0);
	}
	
	void Emplace(ObjectContainer& cont) const override {
		cont.Add(Rect()
			.SetFillColor(Rgb{16, 16, 26})
			.SetStrokeColor(Rgb{180, 180, 180})
			.SetStrokeWidth(2)
			.SetTL(center - Point{25,15})
			.SetBR(center + Point{25,15})
			.SetRound(3));
		cont.Add(Text()
				.SetStrokeColor(Rgb{180, 180, 180})
				.SetFillColor(Rgb{180, 180, 180})
				.SetFontSize(16)
				.SetPosition(center - Point({0, -5}))
				.SetData(label));
	}
};

class Element : public Base{
	mutable std::string label;
public:
	Element(Point pt, std::string name) {
		label = name;
		center = pt;
	}
	Element() {
		std::cerr << "Element() why im here?" << std::endl;
		assert(0);
	}
	
	void Emplace(ObjectContainer& cont) const override {
		using namespace std::string_literals;
		cont.Add(Circle()
				.SetFillColor(Rgb{16, 16, 26})
				.SetStrokeColor(Rgb{180, 180, 180})
				.SetStrokeWidth(2)
				.SetCenter(center)
				.SetRadius(20));
		if (label == "AND"s) {
			label = "&&";
		}
		else if (label == "OR"s) {
			label = "||";
		}
		else if (label == "XOR"s) {
			label = "^";
		}
		cont.Add(Text()
				.SetStrokeColor(Rgb{180, 180, 180})
				.SetFillColor(Rgb{180, 180, 180})
				.SetFontSize(16)
				.SetPosition(center - Point{0, -5.0})
				.SetData(label));
	}
	friend inline bool operator<(const Element& lhs, const Element& rhs);
};

inline bool operator<(const Element& lhs, const Element& rhs) {
	if (lhs.center < rhs.center) 
		return true;
	if (lhs.center > rhs.center) 
		return false;
	return lhs.label < rhs.label;
}

class Wire{
	Point start;
	Point rightmost;
	std::multimap<Point, Point> edges;
	// int in_w;
	// int out_w;
	const int tr_w = 30;
public:
	template<typename Entity, typename Container>
	Wire(const Entity& in, const Container& outs) {
		start = in.GetCenter();
		bool need_back = false;
		constexpr int out_w = (std::is_same<Entity, WireLabel>::value)?
			25:20;
		constexpr int in_w = (std::is_same<typename Container::value_type, WireLabel>::value)?
			25:20;
		start.x += out_w;
		rightmost = start;
		//rightmost.x += 10;

		for (const auto& el : outs) {
			double joint_ptx = el.GetCenter().x - in_w - tr_w;
			rightmost.x = rightmost.x > joint_ptx? rightmost.x: joint_ptx;
		}
		
		for (const auto& el : outs) {
			Point pt = el.GetCenter();
			pt.x -= in_w;
			Point joint(pt);
			joint.x -= tr_w;
			joint.y = start.y;
			if (joint.x < start.x + 10) {
				need_back = true;
				joint.x = start.x + 10;
			}
			edges.emplace(joint, pt);
		}
		if (rightmost.x < start.x + 10 && need_back) {
			rightmost.x = start.x + 10;
		}
	}
	
	void Emplace(ObjectContainer& cont) const {
		cont.Add(Polyline()
			.AddPoint(start)
			.AddPoint(rightmost)
			.SetStrokeColor(Rgb{15, 180, 15})
			.SetStrokeWidth(1));
		
		for (auto edge : edges) {
			if(edge.second.x > edge.first.x - 10) {
				Point lr(edge.first);
				Point tb(edge.second);
				auto tmp = lr.x;
				lr.x = tb.x;
				tb.x = tmp;
				cont.Add(Line()
					.AddPoint(edge.first)
					.AddPoint(lr)
					.AddPoint(tb)
					.AddPoint(edge.second)
					.SetStrokeColor(Rgb{16, 180, 15})
					.SetFillColor(NoneColor)
					.SetStrokeWidth(1)
					.SetArrow(true));
			}
			else {
				double ofsy = (edge.second.y - edge.first.y)/2;
				double ofsx = ofsy;
				/*if (edge.first.y > edge.second.y) {
					ofsy = -ofsy;
				}*/
				Point a1 = edge.first + Point{ofsx, 0};
				Point a2 = edge.first + Point{ofsx, ofsy};
				Point a3 = edge.first + Point{0, ofsy};
				Point a4 = edge.first + Point{-ofsx, ofsy};

				cont.Add(Line()
					.AddPoint(edge.first)
					.AddPoint(a1)
					.AddPoint(a2)
					.AddPoint(a3)
					.SetStrokeColor(Rgb{15, 180, 15})
					.SetFillColor(NoneColor)
					.SetStrokeWidth(1));

				Point b1 = edge.second + Point{0, -ofsy};
				Point b2 = edge.second + Point{-ofsx, -ofsy};
				Point b3 = edge.second + Point{-ofsx, 0};
				Point b0 = edge.second + Point{ofsx, -ofsy};
				cont.Add(Line()
					.AddPoint(b1)
					.AddPoint(b2)
					.AddPoint(b3)
					.AddPoint(edge.second)
					.SetStrokeColor(Rgb{15, 180, 15})
					.SetFillColor(NoneColor)
					.SetStrokeWidth(1)
					.SetArrow(true));
				
				cont.Add(Line()
					.AddPoint(a3)
					.AddPoint(a4)
					.AddPoint(b0)
					.AddPoint(b1)
					.SetStrokeColor(Rgb{15, 180, 15})
					.SetFillColor(NoneColor)
					.SetStrokeWidth(1));
			}
		}
	}
	
};

}  // namespace svg
