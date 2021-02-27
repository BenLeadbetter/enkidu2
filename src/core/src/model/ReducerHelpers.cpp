#include <enkidu/model/ReducerHelpers.hpp>

namespace enkidu::model::reducer_helpers {

Node makeNode()
{
    Ports inputs{};
    for (int i = 0; i != 3; ++i) inputs = inputs.push_back(Port{});
    Ports outputs{};
    for (int i = 0; i != 3; ++i) outputs = outputs.push_back(Port{});
    return Node{{}, inputs, outputs};
}

Document update(Document doc, Nodes nodes)
{
    return Document{doc.connections, nodes};
}

Document update(Document doc, Connections connections)
{
    return Document{connections, doc.nodes};
}

CoreModel update(CoreModel model, Document document)
{
    return CoreModel{model.name, model.version, document};
}


}
