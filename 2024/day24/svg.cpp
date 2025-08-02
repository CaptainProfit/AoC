#include "svg.h"
namespace svg {

using namespace std;
using namespace std::literals;

struct OstreamColorPrinter {
    ostream& out;

    void operator()(monostate) const {
        out << "none"sv;
    }
    void operator()(string color) const {
        out << color;
    }
    void operator()(Rgb color) const {
        out << "rgb("sv << (int)color.red << "," << (int)color.green << "," << (int)color.blue << ")"sv;
    }
    void operator()(Rgba color) const {
        out << "rgba("sv << (int)color.red << "," << (int)color.green << "," << (int)color.blue << "," << color.opacity << ")"sv;
    }
};

ostream& operator<<(ostream& out, const Color& color) {
    visit(OstreamColorPrinter{out}, color);
    return  out;
}

ostream& operator<<(ostream& out, const StrokeLineCap& cap) {
    switch(cap) {
        case StrokeLineCap::BUTT:      out << "butt";      break;
        case StrokeLineCap::ROUND:     out << "round";     break;
        case StrokeLineCap::SQUARE:    out << "square";    break;
    }
    return out;
}
ostream& operator<<(ostream& out, const StrokeLineJoin& join) {
    switch(join) {
        case StrokeLineJoin::ARCS: out << "arcs" ; break;
        case StrokeLineJoin::BEVEL: out << "bevel" ; break;
        case StrokeLineJoin::MITER: out << "miter" ; break;
        case StrokeLineJoin::MITER_CLIP: out << "miter-clip" ; break;
        case StrokeLineJoin::ROUND: out << "round" ; break;
    }
    return out;
}

bool operator<(const Point& lhs, const Point& rhs) {
    if (lhs.x < rhs.x) 
        return true;
    if (lhs.x > rhs.x) 
        return false;
    return (lhs.y < rhs.y);
}
bool operator>(const Point& lhs, const Point& rhs) {
    return rhs < lhs;
}

Point operator+(Point lhs, const Point& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Point operator-(Point lhs, const Point& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);
    

    context.out << endl;
}

// ---------- Circle ------------------
Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\""sv;
    RenderAttrs(out);
    out << "/>"sv;
}

// ---------- Polyline ------------------
// Добавляет очередную вершину к ломаной линии
Polyline& Polyline::AddPoint (Point pt) {
    points_.push_back(pt);
    return *this;
}

void Polyline::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    bool is_first = true;
    out << "<polyline points=\""sv;
    for (auto& pt: points_) {
        if (!is_first) {
            out << " "sv;
        }
        out << pt.x <<","sv << pt.y;
        is_first = false;
    }
    out << "\"";
    RenderAttrs(out);
    out << "/>"sv;
}


// ---------- Line ------------------
// Добавляет очередную вершину к ломаной линии
Line& Line::AddPoint (Point pt) {
    points_.push_back(pt);
    return *this;
}

Line& Line::SetArrow (bool flag) {
    arrow = true;
    return *this;
}

void Line::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    bool is_first = true;
    out << "<path d=\""sv;
    for (auto& pt: points_) {
        if (!is_first) {
            out << " "sv;
            out << pt.x <<","sv << pt.y;
        }
        else {
            out << "M" << pt.x <<","sv << pt.y;
            out << "\nC";
        }
        is_first = false;
    }
    out << "\"";
    RenderAttrs(out);
    if (arrow)
        out << " marker-end=\"url(#arrowhead)\"";
    out << "/>"sv;
}

// ---------- Rect ------------------
// Задаёт координаты левой верхней точки (атрибуты x и y)
Rect& Rect::SetTL(Point point) {
	tl = point;
	return *this;
}

// ---------- Rect ------------------
// Задаёт координаты нижней правой точки (атрибуты x и y)
Rect& Rect::SetBR(Point point) {
	br = point;
	return *this;
}

// ---------- Rect ------------------
// Задаёт закругление углов
Rect& Rect::SetRound(double r_) {
	r = r_;
	return *this;
}

// ---------- Rect ------------------
// Перемещает прямоугольник.
Rect& Rect::Move(Point point) {
	br.x += - tl.x + point.x;
	br.y += - tl.y + point.y;
	tl = point;
	return *this;
}

// ---------- Rect ------------------
// Перемещает прямоугольник.
void Rect::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<rect"sv;
    RenderAttrs(out);
    out << " x=\""sv << tl.x << "\" y=\""sv << tl.y << "\""sv;
    out << " width=\""sv << (br-tl).x << "\" height=\""sv << (br-tl).y << "\""sv;
    out << " rx=\""sv << r <<"\">";
    out << "</rect>"sv;
}

// ---------- Text ------------------
// Задаёт координаты опорной точки (атрибуты x и y)
Text& Text::SetPosition(Point pos) {
    pos_ = pos;
    return *this;
}

// Задаёт смещение относительно опорной точки (атрибуты dx, dy)
Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

// Задаёт размеры шрифта (атрибут font-size)
Text& Text::SetFontSize(uint32_t size) {
    size_ = size;
    return *this;
}

// Задаёт название шрифта (атрибут font-family)
Text& Text::SetFontFamily(string font_family) {
    font_family_ = font_family;
    return *this;
}

// Задаёт толщину шрифта (атрибут font-weight)
Text& Text::SetFontWeight(string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

// Задаёт текстовое содержимое объекта (отображается внутри тега text)
Text& Text::SetData(string data) {
    data_ = data;
    return *this;
}

// Прочие данные и методы, необходимые для реализации элемента <text>
void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text";
    RenderAttrs(out);
    out << " text-anchor=\"middle\"";
    out << " x=\""sv << pos_.x <<"\" y=\""sv << pos_.y << "\" "sv;
    out << "dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\" "sv;
    out << "font-size=\""sv << size_ << "\""sv;
    if (!font_family_.empty()) {
        out << " font-family=\""sv << font_family_ << "\""sv;
    }
    if (!font_weight_.empty()) {
        out << " font-weight=\""sv << font_weight_ << "\""sv;
    }    
    
    out << ">"sv;
    //size_t left = 0;
    size_t data_begin = data_.find_first_not_of(" ");
    size_t data_end = data_.find_last_not_of(" ");
    if (data_begin == data_.npos) {
        data_begin = 0;
    }
    if (data_end == data_.npos) {
        data_end = data_.size();
    }
    else {
        data_end++;
    }
    for (size_t right = data_begin; right < data_end; right++) {
        // right = data_.size();//FindNext(data_, right);
        // out << data_.substr(left, right - left - 1);
        switch(data_[right]) {
            case '\'': out << "&apos;"sv; break;
            case '"': out << "&quot;"sv; break;
            case '<': out << "&lt;"sv; break;
            case '>': out << "&gt;"sv; break;
            case '&': out << "&amp;"sv; break;
            default: out << data_[right];
        }
    }
    out << "</text>"sv;
}

void Document::Render(ostream& out) const {
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << endl;
    out << "<svg width=\"5000\" height=\"3000\" style=\"background: rgb(15,15,35)\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << endl;
    RenderContext ctx(out, 2, 2);
    for (auto& object: objects_) {
        object->Render(ctx);
    }
    out << " <defs>\n";
    out << "   <marker id=\"arrowhead\" markerWidth=\"10\" markerHeight=\"7\"\n";
    out << "           refX=\"9\" refY=\"3.5\" orient=\"auto\">\n";
    out << "     <polygon points=\"0 0, 10 3.5, 0 7\" fill=\"#0FC00F\" />\n";
    out << "   </marker>\n";
    out << " </defs>\n";
    out << "</svg>"sv;
}

}  // namespace svg
