#include "p3ui.h"
#include <p3/Node.h>


namespace p3::python
{

    struct NodeLocker
    {
        NodeLocker(Synchronizable *synchonizable)
            : synchonizable(synchonizable)
        {
        }

        void enter()
        {
            py::gil_scoped_release release;
            lock = synchonizable->lock();
        }

        void exit(py::args)
        {
            lock.reset();
        }

        std::unique_ptr<std::lock_guard<std::recursive_mutex>> lock;
        Synchronizable *synchonizable;
    };

    void ArgumentParser<Node>::operator()(py::kwargs const& kwargs, Node& node)
    {
        ArgumentParser<StyleBlock>()(kwargs, *node.style());
        assign(kwargs, "label", node, &Node::set_label);
        assign(kwargs, "visible", node, &Node::set_visible);
        assign(kwargs, "disabled", node, &Node::set_disabled);
        assign(kwargs, "on_resize", node, &Node::set_on_resize);
        assign(kwargs, "on_mouse_enter", node, &Node::set_on_mouse_enter);
        assign(kwargs, "on_mouse_move", node, &Node::set_on_mouse_move);
        assign(kwargs, "on_mouse_leave", node, &Node::set_on_mouse_leave);
        //
        // TODO: add setter and use assign
        if (kwargs.contains("children"))
        {
            auto children = kwargs["children"].cast<std::vector<std::shared_ptr<Node>>>();
            for (auto& child : children)
                node.add(child);
        }
    }

    void Definition<Node>::apply(py::module& module)
    {
        //
        // by value, no need for sync
        py::class_<Node::MouseEvent> mouse_event(module, "MouseEvent");
        mouse_event.def_property_readonly("source", [](Node::MouseEvent& e) {
            return e.source()->shared_from_this();
        });
        mouse_event.def_property_readonly("x", &Node::MouseEvent::x);
        mouse_event.def_property_readonly("y", &Node::MouseEvent::y);
        mouse_event.def_property_readonly("global_x", &Node::MouseEvent::global_x);
        mouse_event.def_property_readonly("global_y", &Node::MouseEvent::global_y);

        //
        // NodeLocker
        auto node_locker = py::class_<NodeLocker, std::shared_ptr<NodeLocker>>(module, "NodeLocker");
        node_locker.def("__enter__", &NodeLocker::enter);
        node_locker.def("__exit__", &NodeLocker::exit);

        //
        // Node, synced
        py::class_<Node, std::shared_ptr<Node>> node(module, "Node");
        node.def("node_count", &Node::node_count);
        def_property_readonly(node, "parent", &Node::shared_parent);
        def_property_readonly(node, "children", &Node::children);
        def_property_readonly(node, "style", &Node::style);
        def_property(node, "visible", &Node::visible, &Node::set_visible);
        def_property(node, "disabled", &Node::disabled, &Node::set_disabled);
        def_property(node, "label", &Node::label, &Node::set_label);
        def_property(node, "on_resize", &Node::on_resize, &Node::set_on_resize);
        def_property(node, "on_mouse_enter", &Node::on_mouse_enter, &Node::set_on_mouse_enter);
        def_property(node, "on_mouse_move", &Node::on_mouse_move, &Node::set_on_mouse_move);
        def_property(node, "on_mouse_leave", &Node::on_mouse_leave, &Node::set_on_mouse_leave);
        def_method(node, "redraw", &Node::redraw);
        def_method(node, "add", &Node::add);
        def_method(node, "remove", &Node::remove);
        node.def_property_readonly("lock", [](Node& node) {
            return std::make_shared<NodeLocker>(&node);
        });
    }

}
