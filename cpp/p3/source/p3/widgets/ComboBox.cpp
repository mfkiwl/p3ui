#include "ComboBox.h"

#include <p3/constant.h>

#include <imgui.h>
#include <imgui_internal.h>


namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{ std::nullopt, 0.f, 1.f };
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& ComboBox::style_strategy() const
    {
        return _style_strategy;
    }

    ComboBox::ComboBox()
        : Node("ComboBox")
    {
    }

    void ComboBox::render_impl(Context&, float width, float height)
    {
        if (label())
            ImGui::SetNextItemWidth(width * GoldenRatio);
        else
            ImGui::SetNextItemWidth(width);
        auto& selected_option = _selected_index >= 0 ? _options[_selected_index] : _hint;
        if (ImGui::BeginCombo(imgui_label().c_str(), selected_option.c_str()))
        {
            for (std::size_t i = 0; i < _options.size(); ++i)
            {
                ImGui::PushID((void*)&_options[i]);
                if (ImGui::Selectable(_options[i].c_str(), i == _selected_index))
                {
                    if (_on_change)
                        postpone([f = _on_change, index = int(i)]() { f(index); });
                    _selected_index = int(i);
                }
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
        update_status();
    }

    void ComboBox::set_options(std::vector<std::string> options)
    {
        _options = std::move(options);
    }

    std::vector<std::string> const& ComboBox::options() const
    {
        return _options;
    }

    void ComboBox::set_on_change(OnChange on_change)
    {
        _on_change = on_change;
    }

    ComboBox::OnChange ComboBox::on_change() const
    {
        return _on_change;
    }

    int ComboBox::selected_index() const
    {
        return _selected_index;
    }

    void ComboBox::set_selected_index(int selected_index)
    {
        _selected_index = selected_index;
    }

    std::string const& ComboBox::hint() const
    {
        return _hint;
    }

    void ComboBox::set_hint(std::string hint)
    {
        _hint = std::move(hint);
    }

    void ComboBox::dispose()
    {
        _on_change = nullptr;
        Node::dispose();
    }

    void ComboBox::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = font_size + 2 * frame_padding.y;
        _automatic_width = DefaultItemWidthEm * font_size;
        if (label())
        {
            auto const inner_spacing = context_ptr->Style.ItemInnerSpacing;
            const ImVec2 label_size = ImGui::CalcTextSize(label().value().c_str(), NULL, true);
            _automatic_width += label_size.x + inner_spacing.x;
        }
    }

}
