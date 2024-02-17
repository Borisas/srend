#pragma once

#include <vector>
#include <functional>

namespace srend {

    template<typename T, typename... Args>
    class _funcBase {
    public:

        virtual ~_funcBase() {};
    
        virtual bool same(_funcBase<T,Args...>*) const = 0;
        virtual void invoke(Args... a) const  = 0;
    };


    template<typename T, typename... Args>
    class _anonFunc : public _funcBase<T,Args...> {

    public:

        _anonFunc(T(*func)(Args...) ) : func(func) {}

        bool same(_funcBase<T,Args...>* b) const override {
            if ( typeid(this) != typeid(b) ) return false;
            auto casted = dynamic_cast<_anonFunc*>(b);
            if ( !casted ) return false;
            return casted->func == func;
        }

        void invoke(Args... a) const override {
            func(a...);
        }

    private:
        T(*func)(Args...);
    };

    template<typename O, typename T, typename... Args>
    class _memberFunc : public _funcBase<T,Args...> {

    public:
        _memberFunc(T(O::*func)(Args...) , O* owner) : owner(owner), func(func){}

        bool same(_funcBase<T,Args...>* b) const override {
            if ( typeid(this) != typeid(b) ) return false;
            auto casted = dynamic_cast<_memberFunc*>(b);
            if ( !casted ) return false;
            return casted->owner == owner && casted->func == func;
        }

        void invoke(Args... a) const override {
            (owner->*func)(a...);
        }

    private:
        O* owner;
        T(O::*func)(Args...);
    };

    // template<typename T, typename... Args>
    // class _lambdaFunc : public _funcBase<T,Args...> {

    // public:
    //     _lambdaFunc(std::function<T(Args...)> func) : func(func){}

    //     bool same(_funcBase<T,Args...>* b) const override {
    //         return false;
    //         // if ( typeid(this) != typeid(b) ) return false;
    //         // auto casted = dynamic_cast<_lambdaFunc*>(b);
    //         // if ( !casted ) return false;
    //         // return casted->func == func;
    //     }

    //     void invoke(Args... a) const override {
    //         func(a...);
    //     }

    // private:
    //     std::function<T(Args...)> func;
    // };
    

    class Action {
    public:


        template<typename T, typename... Args>
        static _funcBase<T,Args...>* bind(T(*func)(Args...)) {
            return new _anonFunc(func);
        }

        // template<typename T, typename... Args>
        // static _funcBase<T,Args...>* bind1(T&& func) {
        //     return new _lambdaFunc(func);
        // }

        template<typename O, typename T, typename... Args>
        static _funcBase<T,Args...>* bind(T(O::*func)(Args...) , O* owner) {
            return new _memberFunc(func,owner);
        }
    };

    template<typename T, typename... Args>
    class Event {

    public:

        void operator+=(_funcBase<T,Args...>* f) {
            functions.push_back(std::unique_ptr<_funcBase<T,Args...> >(f));
        }

        void operator-=(_funcBase<T,Args...>* f) {
            for ( int i = 0; i < functions.size(); i++ ) {
                if ( functions[i]->same(f) ) {
                    functions.erase(functions.begin() + i);
                    i--;
                }
            }
        }

        void invoke(Args... a) {
            for ( int i = 0; i < functions.size(); i++ ) {
                functions[i]->invoke(a...);
            }
        }

        void clearAnonymous() {
            for ( int i = 0; i < functions.size(); i++ ) {
                auto casted = dynamic_cast<_anonFunc<T,Args...>*>(functions[i].ptr);
                if ( !casted ) {
                    continue;
                    //for lambdas
                    // auto casted = dynamic_cast<_lambdaFunc<T,Args...>*>(functions[i].ptr);
                    // if ( !casted ) continue;
                }
                functions.erase(functions.begin() + i);
                i--;
            }
        }

        void clearAll() {
            functions.clear();
        }

    private:
        std::vector<std::unique_ptr<_funcBase<T,Args...> > > functions;
    };
}