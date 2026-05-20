#pragma once
#include <vector>
#include <algorithm>
#include <memory>

namespace o2
{
    template<class T, typename Pred> T* singleOrNull(const std::vector<T*>& vec, Pred pred)
    {
        T* result = nullptr;
        for (const auto& item : vec) {
            if (pred(item)) {
                if (result != nullptr) throw std::runtime_error("More than one element found");
                result = item;
            }
        }

        return result;
    }

    template<class T, typename Pred> T* firstOrDefault(const std::vector<T*>& vec, Pred pred)
    {
        T* result = nullptr;
        auto it = std::find_if(vec.begin(), vec.end(), pred);

        return it == vec.end() ? nullptr : *it;

        //for (const auto& item : vec) {
        //    if (pred(item)) {
        //        if (result != nullptr) throw std::runtime_error("More than one element found");
        //        result = item;
        //    }
        //}

        //return result;
    }

    template<class T> void deleteAll(const std::vector<T*>& vec)
    {
        for (auto* t : vec) {
            delete(t);
            t = nullptr;
        }
    }

}