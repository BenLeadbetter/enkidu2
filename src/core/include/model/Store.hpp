#pragma once

#include <model/Reducer.hpp>

#include <boost/range/algorithm/find_if.hpp>

#include <functional>
#include <optional>

namespace enkidu::model {

template<typename Model, typename Action, typename Factory>
class Store
{
public:
    void dispatch(Action action)
    {
        auto oldModel = m_model;
        m_model = std::move(m_reducer(m_model, action));
        for (const auto& callback : m_callbacks)
        {
            callback(oldModel, m_model);
        }
    }

    struct Callback
    {
        operator bool() const
        {
            return static_cast<bool>(fn);
        }
        void operator()(Model oldModel, Model newModel) const
        {
            fn(oldModel, newModel);
        }
        std::function<void(Model, Model)> fn;
    };

    template<typename Function>
    const Callback& addCallback(Function fn)
    {
        m_callbacks.emplace_back(Callback{fn});
        return m_callbacks.back();
    }

    bool removeCallback(const Callback& callback)
    {
        auto match = [&callback](const auto& c) { return &callback == &c; };
        auto itr = boost::find_if(m_callbacks, match);
        if (itr != m_callbacks.cend())
        {
            m_callbacks.erase(itr);
            return true;
        }
        return false;
    }

    const Model model() const
    {
        return m_model;
    }

private:
    Model m_model;
    std::vector<Callback> m_callbacks;
    Reducer<Model, Action, Factory> m_reducer;
};

}
