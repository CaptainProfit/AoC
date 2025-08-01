#pragma once
#include "svg.h"
#include <vector>
#include <map>
#include <set>
#include <string>

namespace svg {
class Base {
public:
	virtual void Emplace(ObjectContainer& cont) = 0;
	Point GetCenter() {
		return center;
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
	
	void Emplace(ObjectContainer& cont) override {
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
				.SetPosition(center - Point({15, -5}))
				.SetData(label));
	}
};

class Element : public Base{
	std::string label;
public:
	Element(Point pt, std::string name) {
		label = name;
		center = pt;
	}
	
	void Emplace(ObjectContainer& cont) override {
		using namespace std::string_literals;
		cont.Add(Circle()
				.SetFillColor(Rgb{16, 16, 26})
				.SetStrokeColor(Rgb{180, 180, 180})
				.SetStrokeWidth(2)
				.SetCenter(center)
				.SetRadius(20));
		cont.Add(Text()
				.SetStrokeColor(Rgb{180, 180, 180})
				.SetFillColor(Rgb{180, 180, 180})
				.SetFontSize(16)
				.SetPosition(center - Point({label.compare("&&"s)?5.0:12.0, -5.0}))
				.SetData(label));
	}
};

class Wire{
	Point start;
	Point rightmost;
	std::multimap<Point, Point> edges;
public:
	Wire(const Point& in, const std::set<Point>& outs) {
		start = in;
		start.x += 25;
		rightmost = in;
		for (auto& pt : outs) {
			rightmost.x = rightmost.x > pt.x? rightmost.x: pt.x;
		}
		rightmost.x -= 70;
		//edges.emplace(start, rightmost);
		double old_x = start.x;
		for (Point pt : outs) {
			Point joint(in);
			pt.x -= 20;
			joint.x = pt.x - 50;
			if (start.x > pt.x) {
				pt.x = old_x;
			}
			edges.emplace(joint, pt);
			old_x = joint.x;
		}
	}
	
	void Emplace(ObjectContainer& cont) {
		cont.Add(Polyline()
			.AddPoint(start)
			.AddPoint(rightmost)
			.SetStrokeColor(Rgb{15, 180, 15})
			.SetStrokeWidth(1));
		
		for (auto edge : edges) {
			if(start.x < edge.first.x) {
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
					.SetStrokeColor(Rgb{15, 180, 15})
					.SetFillColor(NoneColor)
					.SetStrokeWidth(1)
					.SetArrow(true));
			}
			else {
				double ofsy = 10;
				if (edge.first.y > edge.second.y) {
					ofsy = -10;
				}
				Point a1(edge.first);
				Point a2(edge.first);
				Point a3(edge.first);
				a1.x += 10;
				a2.x += 10;
				a2.y += ofsy;
				a3.y += ofsy;

				Point a4(a3);
				a4.x -= 10;
				cont.Add(Line()
					.AddPoint(edge.first)
					.AddPoint(a1)
					.AddPoint(a2)
					.AddPoint(a3)
					.SetStrokeColor(Rgb{15, 180, 15})
					.SetFillColor(NoneColor)
					.SetStrokeWidth(1));

				Point b1(edge.second);
				Point b2(edge.second);
				Point b3(edge.second);
				b1.y -= ofsy;
				b2.x -= 10;
				b2.y -= ofsy;
				b3.x -= 10;
				Point b0(b1);
				b0.x += 10;
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
