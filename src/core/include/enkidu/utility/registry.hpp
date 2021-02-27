#pragma once
// from Sean Parent CppCon 2019

#include <boost/range/algorithm/remove_if.hpp>

#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

namespace enkidu::utility {

template<typename T>
class registry
{
public:
    using Id = size_t;
    Id add(T item)
    {
        m_data.emplace_back(std::make_pair(++m_id, std::move(item)));
        return m_id;
    }
    void remove(Id id)
    {
        const auto compare = [](const auto& pair, const auto& id)
        {
            return pair.first < id;
        };
        auto itr = std::lower_bound(std::begin(m_data), std::end(m_data), id, compare);

        if (itr == std::cend(m_data) || itr->first != id)
        {
            return;
        }

        itr->second.reset();
        --m_size;

        if (m_size < m_data.size() / 2)
        {
            const auto null = [](const auto& pair) { return !pair.second; };
            m_data.erase(boost::remove_if(m_data, null), std::end(m_data));
        }
    }
    template<typename Function>
    void for_each(const Function& function)
    {
        for (const auto& element : m_data)
        {
            if (element.second)
            {
                function(*element.second);
            }
        }
    };

private:
    using data = std::vector<std::pair<Id, std::optional<T>>>;
    data m_data;
    size_t m_size{0};
    size_t m_id{0};
};

}
