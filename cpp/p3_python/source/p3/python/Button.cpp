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

#include "p3ui.h"
#include <p3/Button.h>

namespace p3::python
{

    void Definition<Button>::parse(py::kwargs const& kwargs, Button& button)
    {
        Definition<Node>::parse(kwargs, button);
        if (kwargs.contains("on_click"))
            button.set_on_click([f=kwargs["on_click"].cast<py::function>()]() {
                py::gil_scoped_acquire acquire;
                f();
            });
    }

    void Definition<Button>::apply(py::module& module)
    {
        py::class_<Button, Node, std::shared_ptr<Button>> button(module, "Button");
        button.def(py::init<>([](py::kwargs kwargs) {
            auto button = std::make_shared<Button>();
            parse(kwargs, *button);
            return button;
        }));
        button.def_property("on_click", &Button::on_click, [](Button& button, py::function f) {
            button.set_on_click([f{std::move(f)}]() {
                py::gil_scoped_acquire acquire;
                f();
            });
        });
    }

}