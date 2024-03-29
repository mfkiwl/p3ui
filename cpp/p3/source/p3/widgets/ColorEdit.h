#pragma once

#include <string>
#include <functional>

#include <p3/Node.h>
#include <p3/Color.h>

namespace p3
{

    class ColorEdit : public Node
    {
    public:
        using OnChange = std::function<void(Color)>;

        ColorEdit();

        StyleStrategy& style_strategy() const override;        
        void render_impl(Context&, float width, float height) override;
        void update_content() override;

        void set_on_change(OnChange);
        OnChange on_change() const;

        void set_value(Color);
        Color const& value() const;
    protected:
        void dispose() override;

    private:
        float _value[4];
        Color _color;
        OnChange _on_change;
    };

}
