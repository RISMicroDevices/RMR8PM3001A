#pragma once

namespace VMC {

    template<typename _T>
    class Variable {
    private:
        _T*     ptr;
        bool    ref;

    public:
        Variable();
        Variable(_T val);
        Variable(_T* ref);
        Variable(const Variable<_T>& obj);
        ~Variable();

        _T      Get();
        void    Set(_T val);

        _T&     operator*();
        void    operator=(const Variable<_T>& obj);
        bool    operator==(const Variable<_T>& obj);
    };

    //
    using IntVariable  = Variable<int>;

    using BoolVariable = Variable<bool>;

}


// class VMC::Variable
namespace VMC {
    /*
    _T*     ptr;
    bool    ref;
    */

    template<typename _T>
    Variable<_T>::Variable()
        : ptr(new _T)
        , ref(false)
    { }

    template<typename _T>
    Variable<_T>::Variable(_T val)
        : ptr(new _T)
        , ref(false)
    { 
        *ptr = val;
    }

    template<typename _T>
    Variable<_T>::Variable(_T* ref)
        : ptr(ref)
        , ref(true)
    { }

    template<typename _T>
    Variable<_T>::Variable(const Variable<_T>& obj)
        : ptr(obj.ref ? obj.ptr :new _T)
        , ref(obj.ref)
    {
        *ptr = *(obj.ptr);
    }

    template<typename _T>
    Variable<_T>::~Variable()
    {
        if (!ref)
            delete ptr;
    }

    template<typename _T>
    inline _T Variable<_T>::Get()
    {
        return *ptr;
    }

    template<typename _T>
    inline void Variable<_T>::Set(_T val)
    {
        *ptr = val;
    }

    template<typename _T>
    _T& Variable<_T>::operator*()
    {
        return *ptr;
    }

    template<typename _T>
    void Variable<_T>::operator=(const Variable<_T>& obj)
    {
        if (!ref)
            delete ptr;

        ptr = obj.ref ? obj.ptr : new _T;
        ref = obj.ref;

        *ptr = *(obj.ptr);
    }

    template<typename _T>
    bool Variable<_T>::operator==(const Variable<_T>& obj)
    {
        return *ptr == *(obj.ptr);
    }
}

