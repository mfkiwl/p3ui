#include "constant.h"
#include "Text.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>


namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{ std::nullopt, 0.f, 0.f };
                return initial;
            }
            LayoutLength const& initial_width() override
            {
                static auto initial = LayoutLength{ std::nullopt, 0.f, 0.f };
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& Text::style_strategy() const
    {
        return _style_strategy;
    }

    Text::Text(std::string value, std::optional<std::string> label)
        : Node("Text")
        , _value(std::move(value))
    {
        set_label(std::move(label));
    }

    void Text::render_impl(Context& context, float width, float height)
    {
        if (label())
        {
            ImGui::SetNextItemWidth(label() ? width * GoldenRatio : width);
            ImGui::LabelText(label().value().c_str(), _value.c_str());
        }
        else
        {
            ImGui::Text(_value.c_str());
        }
        render_absolute(context);
    }

    void Text::set_value(std::string value)
    {
        _value = std::move(value);
        set_needs_update();
        std::optional<float> x;
    }

    std::string const& Text::value() const
    {
        return _value;
    }

    void Text::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        const ImVec2 label_size = ImGui::CalcTextSize(_value.c_str(), NULL, true);
        _automatic_height = font_size;
        _automatic_width = label_size.x;
        if (label())
        {
            _automatic_width += context_ptr->Style.ItemInnerSpacing.x;
            _automatic_width += ImGui::CalcTextSize(label().value().c_str(), NULL, true).x;
            _automatic_height += context_ptr->Style.FramePadding.y * 2;
        }
    }

}
