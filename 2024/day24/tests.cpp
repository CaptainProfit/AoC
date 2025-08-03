
#include <fstream>
#include "primitives.hpp"
using namespace std;

void Test(string fname, set<svg::Element>& els) {
    svg::Document doc;
	ofstream of(fname);
	
    // doc.Add(svg::Rect().SetTL({0,0})
	// 	.SetBR({200, 200})
	// 	.SetRound(1)
	// 	.SetFillColor(svg::Rgb{45, 15, 35})
	// 	.SetStrokeColor(svg::Rgb{15, 55, 35}));

	doc.Add(svg::Rect()
			.SetFillColor(svg::Rgb{15,15,35})
			.SetStrokeColor(svg::Rgb{15,15,35})
			.SetTL({0, 0})
			.SetBR({500, 300}));
				
	svg::WireLabel wl({230, 155}, "hjk");
	svg::Wire wr(wl, els);
	wl.Emplace(doc);
    for (auto& el : els) {
        el.Emplace(doc);
    }
	wr.Emplace(doc);
	doc.Render(of);
}

void TestPrimitives() {
    set<svg::Element> els1({
                {{160, 30}, "||"},
				{{220, 80}, "&&"},
				{{280, 130}, "^"},
				{{240, 220}, "a"},
				{{400, 230}, "b"},
				{{460, 280}, "c"}});
    Test("out1.svg", els1);

    set<svg::Element> els2({
                {{60, 30}, "||"},
				{{120, 80}, "&&"},
				{{180, 130}, "^"},
				{{240, 180}, "a"},
				{{300, 230}, "b"},
				{{360, 280}, "c"}
    });
	Test("out2.svg", els2);

    set<svg::Element> els3({
	{{160, 30}, "||"},
				{{220, 80}, "&&"},
				{{280, 130}, "^"},
				{{280, 180}, "a"},
				{{220, 230}, "b"},
				{{160, 280}, "c"},
    });
    Test("out3.svg", els3);
}
