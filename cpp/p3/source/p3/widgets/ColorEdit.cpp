#include "ColorEdit.h"

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_width() override
            {
                static auto initial = LayoutLength{30|em, 0.f, 0.f};
                return initial;
            }
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{std::nullopt, 0.f, 0.f};
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& ColorEdit::style_strategy() const
    {
        return _style_strategy;
    }

    ColorEdit::ColorEdit()
        : Node("ColorEdit")
        , _value{ 0.f, 0.f, 0.f, 1.f }
        , _color(0, 0, 0, 255)
    {
    }

    void ColorEdit::render_impl(Context&, float width, float height)
    {
        
        ImGui::SetNextItemWidth(width);
        if (ImGui::ColorEdit4(imgui_label().c_str(), _value))
        {
            _color = p3::Color(
                std::uint8_t(_value[0] * 255.f),
                std::uint8_t(_value[1] * 255.f),
                std::uint8_t(_value[2] * 255.f),
                std::uint8_t(_value[3] * 255.f)
            );
            if (_on_change)
            {
                postpone([color = _color, f = _on_change]() {
                    f(std::move(color));
                });
            }
        }
        update_status();
    }

    void ColorEdit::update_content()
    {
        
        auto const& context = *ImGui::GetCurrentContext();
        auto const font_size = context.FontSize;
        _automatic_height = font_size + 2 * context.Style.FramePadding.y;
        _automatic_width = font_size * 30.f;
    }

    void ColorEdit::set_on_change(OnChange on_change)
    {
        _on_change = on_change;
    }

    ColorEdit::OnChange ColorEdit::on_change() const
    {
        return _on_change;
    }

    void ColorEdit::set_value(Color color)
    {
        std::swap(_color, color);
        _value[0] = _color.red() / 255.f;
        _value[1] = _color.green() / 255.f;
        _value[2] = _color.blue() / 255.f;
        _value[3] = _color.alpha() / 255.f;
    }

    Color const& ColorEdit::value() const
    {
        return _color;
    }

    void ColorEdit::dispose()
    {
        _on_change = nullptr;
        Node::dispose();
    }

}
