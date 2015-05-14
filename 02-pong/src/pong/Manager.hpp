#pragma once

#include <vector>
#include <map>
#include <memory>
#include <typeinfo>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include <pong/make_unique.hpp>
#include <pong/Entity.hpp>

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>>        entities;
    std::map<std::size_t, std::vector<Entity*>> groupedEntities;

public:
    template<typename T, typename... TArgs> 
    T& create(TArgs&&... mArgs)
    {
        static_assert(std::is_base_of<Entity, T>::value, "'T' must be derived from 'Entity'");

        auto uPtr = make_unique<T>(std::forward<TArgs>(mArgs)...);
        auto ptr  = uPtr.get();

        groupedEntities[typeid(T).hash_code()].emplace_back(ptr);
        entities.emplace_back(std::move(uPtr));

        return *ptr;
    }   

    void refresh() 
    {
        using std::remove_if;
        using std::begin;
        using std::end;
        using std::unique_ptr;

        for (auto& pair : groupedEntities) {
            auto& vector = pair.second;
            vector.erase(remove_if(begin(vector),
                                   end(vector),
                                   [](Entity* mPtr){return mPtr->destroyed;}),
                         end(vector)
            );
        }

        entities.erase(remove_if(begin(entities),
                                 end(entities),
                                 [](const unique_ptr<Entity>& mUPtr) {return mUPtr->destroyed;}),
                       end(entities)
        );
    }

    void clear() 
    { 
        entities.clear(); 
    }

    void update()                           
    { 
        for (auto& e : entities) 
            e->update(); 
    }

    void draw(sf::RenderWindow& mTarget)    
    { 
        for (auto& e : entities) 
            e->draw(mTarget); 
    }

    template<typename T> 
    const std::vector<Entity*>& getAll()
    { 
        return groupedEntities[typeid(T).hash_code()]; 
    }

    template<typename T, typename TFunc> 
    void forEach(const TFunc& mFunc) 
    {
        for (auto ptr : getAll<T>()) 
            mFunc(*reinterpret_cast<T*>(ptr));
    }
};