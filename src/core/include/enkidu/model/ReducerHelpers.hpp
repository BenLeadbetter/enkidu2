#pragma once

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <enkidu/model/Model.hpp>

namespace enkidu::model::reducer_helpers {

Node makeNode();

Document update(Document, Nodes);
Document update(Document, Connections);

CoreModel update(CoreModel model, Document document);
template<typename T>
CoreModel update(CoreModel model, T t)
{
    return update(model, update(model.document, t));
}

template<typename T, typename Pred>
std::vector<typename T::Id> getIdsIf(
    immer::map<typename T::Id, T> map,
    const Pred& p)
{
    const auto toid = [](const auto& pair)
    {
        return pair.first;
    };
    return boost::copy_range<std::vector<typename T::Id>>(
        map |
        boost::adaptors::filtered(p) |
        boost::adaptors::transformed(toid)
    );
}

}
