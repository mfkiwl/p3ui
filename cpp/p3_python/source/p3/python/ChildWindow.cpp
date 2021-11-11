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
#include <p3/ChildWindow.h>

namespace p3::python
{

    void Definition<ChildWindow>::apply(py::module& module)
    {
        py::class_<ChildWindow, Node, std::shared_ptr<ChildWindow>> window(module, "ChildWindow");

        window.def(py::init<>([](std::shared_ptr<Node> content, bool resizeable, bool moveable, py::kwargs kwargs) {
            auto window = std::make_shared<ChildWindow>();
            ArgumentParser<Node>()(kwargs, *window);
            assign(kwargs, "on_close", *window, &ChildWindow::set_on_close);
            window->set_content(std::move(content));
            window->set_resizeable(resizeable);
            window->set_moveable(moveable);
            return window;
        }), py::kw_only(), py::arg("content")=py::none(), py::arg("resizeable")=true, py::arg("moveable")=true);
        def_property(window, "resizeable", &ChildWindow::resizeable, &ChildWindow::set_resizeable);
        def_property(window, "moveable", &ChildWindow::moveable, &ChildWindow::set_moveable);
        def_property(window, "on_close", &ChildWindow::on_close, &ChildWindow::set_on_close);
    }

}