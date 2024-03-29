#include "p3ui.h"

#include <p3/widgets/Collapsible.h>

namespace p3::python
{

    void Definition<Collapsible>::apply(py::module& module)
    {
        py::class_<Collapsible, Node, std::shared_ptr<Collapsible>> collapsible(module, "Collapsible");
        
        collapsible.def(py::init<>([](std::optional<std::shared_ptr<Node>> content, std::optional<bool> collapsed, py::kwargs kwargs) {
            auto collapsible = std::make_shared<Collapsible>("");
            ArgumentParser<Node>()(kwargs, *collapsible);
            assign(content, *collapsible, &Collapsible::set_content);
            assign(collapsed, *collapsible, &Collapsible::set_collapsed);
            return collapsible;
        }), py::kw_only(), py::arg("content")=py::none(), py::arg("collapsed")=true);
        def_property(collapsible, "content", &Collapsible::content, &Collapsible::set_content);
        def_property(collapsible, "collapsed", &Collapsible::collapsed, &Collapsible::set_collapsed);
    }

}