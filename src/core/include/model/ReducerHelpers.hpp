#pragma once

#include <model/Model.hpp>

namespace enkidu::model::reducer_helpers {

Node makeNode(Ports, Ports);

Document update(Document, Nodes);
Document update(Document, Connections);

CoreModel update(CoreModel model, Document document);
template<typename T>
CoreModel update(CoreModel model, T t)
{
    return update(model, update(model.document, t));
}

}
