#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <string_view>

namespace svg {
    struct Rgb{
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
    };

    struct Rgba{
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        double opacity = 1.0;
    };

using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;
inline const Color NoneColor{"none"};
std::ostream& operator<<(std::ostream& out, const Color& color);
enum class StrokeLineCap {
    BUTT,
    ROUND,
    SQUARE,
};
std::ostream& operator<<(std::ostream& out, const StrokeLineCap& cap);
enum class StrokeLineJoin {
    ARCS,
    BEVEL,
    MITER,
    MITER_CLIP,
    ROUND
};
std::ostream& operator<<(std::ostream& out, const StrokeLineJoin& join);
struct Point {
    Point() = default;
    Point(double x, double y)
        : x(x)
        , y(y) {
    }
    double x = 0;
    double y = 0;
    Point& operator+=(const Point& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }
};

bool operator<(const Point& lhs, const Point& rhs);
bool operator>(const Point& lhs, const Point& rhs);
Point operator+(Point lhs, const Point& rhs);
Point operator-(Point lhs, const Point& rhs);

template <typename Owner>
class PathProps {
public:
    Owner& SetFillColor(Color color) {
        fill_color_ = std::move(color);
        return AsOwner();
    }
    Owner& SetStrokeColor(Color color) {
        stroke_color_ = std::move(color);
        return AsOwner();
    }
    Owner& SetStrokeWidth(double width) {
        stroke_width_ = width;
        return AsOwner();
    }
    Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
        stroke_linecap_ = line_cap;
        return AsOwner();
    }
    Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
        stroke_linejoin_ = line_join;
        return AsOwner();
    }



protected:
    virtual ~PathProps() = default;

    // Метод RenderAttrs выводит в поток общие для всех путей атрибуты fill и stroke
    
    void RenderAttrs(std::ostream& out) const {
        using namespace std::literals;
        bool any = false;

        if (fill_color_) {
            out << " fill=\""sv << *fill_color_ << "\""sv;
            any = true;
        }
        if (stroke_color_) {
            out << " stroke=\""sv << *stroke_color_ << "\""sv;
            any = true;
        }
        if (stroke_width_) {
            out << " stroke-width=\""sv << *stroke_width_ << "\""sv;
            any = true;
        }
        if (stroke_linecap_) {
            out << " stroke-linecap=\""sv << *stroke_linecap_ << "\""sv;
            any = true;
        }
        if (stroke_linejoin_) {
            out << " stroke-linejoin=\""sv << *stroke_linejoin_ << "\""sv;
            any = true;
        }
        if (!any) {
            out << " ";
        }
    }

private:
    Owner& AsOwner() {
        // static_cast безопасно преобразует *this к Owner&,
        // если класс Owner — наследник PathProps
        return static_cast<Owner&>(*this);
    }

    std::optional<Color> fill_color_;
    std::optional<Color> stroke_color_;
    std::optional<double> stroke_width_;
    std::optional<StrokeLineCap> stroke_linecap_;
    std::optional<StrokeLineJoin> stroke_linejoin_;
};
/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out)
        : out(out) {
    }

    RenderContext(std::ostream& out, int indent_step, int indent = 0)
        : out(out)
        , indent_step(indent_step)
        , indent(indent) {
    }

    RenderContext Indented() const {
        return {out, indent_step, indent + indent_step};
    }

    void RenderIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
public:
    void Render(const RenderContext& context) const;

    virtual ~Object() = default;

protected:
    virtual void RenderObject(const RenderContext& context) const = 0;
};

class ObjectContainer {
public:

    /*
     Метод Add добавляет в svg-документ любой объект-наследник svg::Object.
     Пример использования:
     Document doc;
     doc.Add(Circle().SetCenter({20, 30}).SetRadius(15));
    */
    template<typename ObjType>
    void Add(const ObjType& obj) {
        objects_.emplace_back(std::make_unique<ObjType>(std::move(obj)));
    }

    // Добавляет в svg-документ объект-наследник svg::Object
    // void AddPtr(std::unique_ptr<Object>&& obj);
    //  error: no matching function for call to ‘svg::Circle::Circle(svg::Circle*)’
    // why?
    void virtual AddPtr(std::unique_ptr<Object>&& obj ) = 0;

protected:
    std::vector<std::unique_ptr<Object>> objects_;
};

class Drawable{
public:
    void virtual Draw(ObjectContainer& container) const = 0;
    virtual ~Drawable() = default;
};

/*
 * Класс Circle моделирует элемент <circle> для отображения круга
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
 */
class Circle final : public Object, public PathProps<Circle> {
public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);
    ~Circle() override = default;
private:
    void RenderObject(const RenderContext& context) const override;

    Point center_{0, 0};
    double radius_ = 1.0;
};

/*
 * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
 */
class Polyline final : public Object, public PathProps<Polyline>  {
public:
    // Добавляет очередную вершину к ломаной линии
    Polyline& AddPoint(Point point);
private:
    void RenderObject(const RenderContext& context) const override;

    std::vector<Point> points_;
};

/*
 * Класс Line моделирует элемент <Line> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/Line
 */
class Line final : public Object, public PathProps<Line>  {
public:
    // Добавляет очередную вершину к ломаной линии
    Line& AddPoint(Point point);
    Line& SetArrow(bool);
private:
    void RenderObject(const RenderContext& context) const override;

    bool arrow = false;
    std::vector<Point> points_;
};

/*
 * Класс Rect моделирует элемент <rect> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/rect
 */
class Rect final : public Object, public PathProps<Rect>  {
public:
    // Добавляет очередную вершину к ломаной линии
    Rect& SetTL(Point point);
    Rect& SetBR(Point point);
    Rect& SetRound(double r);
    Rect& Move(Point point);
private:
    void RenderObject(const RenderContext& context) const override;

    double r = 0;
    Point tl, br;
};

/*
 * Класс Text моделирует элемент <text> для отображения текста
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
 */
class Text final : public Object, public PathProps<Text>  {
public:
    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(Point pos);

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(Point offset);

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontSize(uint32_t size);

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family);

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight);

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data);

    // Прочие данные и методы, необходимые для реализации элемента <text>
private:
    void RenderObject(const RenderContext& context) const override;

    Point pos_{0, 0};
    Point offset_{0, 0};
    size_t size_=1;
    std::string font_family_;
    std::string font_weight_;
    std::string data_;
};





class Document: public ObjectContainer {
public:

    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;
    void AddPtr(std::unique_ptr<Object>&& obj) override {
        objects_.emplace_back(std::move(obj));
    }

};

}  // namespace svg
