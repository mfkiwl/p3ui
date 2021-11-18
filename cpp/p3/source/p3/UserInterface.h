/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/
#pragma once

#include "Node.h"
#include "Theme.h"
#include "OnScopeExit.h"
#include "Font.h"

#include <optional>
#include <memory>

struct ImGuiContext;
struct ImPlotContext;

namespace p3
{

    class Popup;
    class MenuBar;
    class RenderBackend;
    class Window;

    class UserInterface
        : public Node
        , public Theme::Observer
    {
    public:
        UserInterface(std::size_t width=1024, std::size_t height=768);
        ~UserInterface();

        void frame();
        using MousePosition = std::array<float, 2>;
        MousePosition mouse_position() const;
        float rem() const;

        //
        // theme / styling
        void set_theme(std::shared_ptr<Theme>);
        std::shared_ptr<Theme> const& theme() const;
        void on_theme_changed() final override;

        //
        // aggregation
        void set_content(std::shared_ptr<Node>);
        std::shared_ptr<Node> content() const;

        void set_menu_bar(std::shared_ptr<MenuBar>);
        std::shared_ptr<MenuBar> const& menu_bar() const;


        //
        // fonts
        FontAtlas font_atlas();
        Font load_font(std::string const&, float size);
        Font default_font();
        void merge_font(std::string const&, float size);
        void set_default_font(Font);

        //
        // img gui/plot context & options
        ImGuiContext& im_gui_context() const;
        ImPlotContext& im_plot_context() const;

        void set_mouse_cursor_scale(float);
        float mouse_cursor_scale() const;

        void set_anti_aliased_lines(bool);
        bool anti_aliased_lines() const;

        void set_anti_aliased_fill(bool);
        bool anti_aliased_fill() const;
        
        void set_curve_tessellation_tolerance(float);
        float curve_tessellation_tolerance() const;

        void set_circle_tessellation_maximum_error(float);
        float circle_tessellation_maximum_error() const;

        void render(Context&, float width, float height, bool) override;

        virtual void synchronize_with(Synchronizable&) override;



    protected:
        void update_content() override;
        void update_restyle(Context&, bool whole_tree=false) override;

    private:
        std::optional<p3::OnScopeExit> _theme_guard;
        Window* _window = nullptr;
        std::size_t _width = 1024;
        std::size_t _height = 768;

        std::shared_ptr<Theme> _theme;
        Theme::ApplyFunction _theme_apply_function;
        std::optional<OnScopeExit> _theme_observer;

        std::shared_ptr<Node> _content;
        std::shared_ptr<MenuBar> _menu_bar;

        std::shared_ptr<ImGuiContext> _im_gui_context;
        std::shared_ptr<ImPlotContext> _im_plot_context;
    };

}
