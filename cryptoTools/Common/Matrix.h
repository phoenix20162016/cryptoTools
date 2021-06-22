#pragma once

#include <cryptoTools/Common/Defines.h>
#ifndef ENABLE_FULL_GSL
#include <cryptoTools/gsl/gls-lite.hpp>
#endif
#include <cryptoTools/Common/MatrixView.h>
#include <cstring>
#include <type_traits>
namespace osuCrypto
{
    enum class AllocType
    {
        Uninitialized,
        Zeroed
    };

    template<typename T>
    class Matrix : public MatrixView<T>
    {
        u64 mCapacity = 0;
        using Storage = u8[sizeof(T)];
    public:
        Matrix() = default;

        Matrix(u64 rows, u64 columns, AllocType t = AllocType::Zeroed)
        {
            mCapacity = rows * columns;
            Storage* ptr;
            if (t == AllocType::Zeroed)
            {
                if (std::is_trivially_constructible<T>::value)
                    ptr = new Storage[mCapacity]();
                else
                {
                    ptr = new Storage[mCapacity];
                    auto iter = (T*)ptr;
                    for (u64 i = 0; i < mCapacity; ++i)
                    {
                        new (iter++)T();
                    }
                }
            }
            else
            {
                ptr = new Storage[mCapacity];
            }

            *((MatrixView<T>*)this) = MatrixView<T>((T*)ptr, rows, columns);
        }

        template<typename T2,
            typename std::enable_if<
            std::is_same<T, T2>::value&&
            std::is_copy_constructible<T2>::value, int>::type = 0>
            Matrix(const Matrix<T2>& copy)
            : Matrix(static_cast<const MatrixView<T2>&>(copy))
        { }

        template<typename T2,
            typename std::enable_if<
            std::is_same<T, T2>::value&&
            std::is_copy_constructible<T2>::value, int>::type = 0>
            Matrix(const MatrixView<T2>& copy)
            : mCapacity(copy.size())
        {
            auto ptr = new Storage[copy.size()];
            *((MatrixView<T>*)this) = MatrixView<T>((T*)ptr, copy.rows(), copy.cols());

            if (std::is_trivially_copyable<T>::value)
                memcpy(MatrixView<T>::mView.data(), copy.data(), copy.mView.size_bytes());
            else
            {
                auto iter = MatrixView<T>::mView.data();
                for (u64 i = 0; i < copy.size(); ++i)
                {
                    new ((char*)iter++)T(copy(i));
                }
            }
        }

        Matrix(Matrix<T>&& copy)
            : MatrixView<T>(copy.data(), copy.bounds()[0], copy.stride())
            , mCapacity(copy.mCapacity)
        {
            copy.mView = span<T>();
            copy.mStride = 0;
            copy.mCapacity = 0;
        }


        ~Matrix()
        {
            if (std::is_trivially_destructible<T>::value == false)
            {
                for (u64 i = 0; i < size(); ++i)
                    (data() + i)->~T();
            }

            delete[](Storage*)(MatrixView<T>::mView.data());
        }


        const Matrix<T>& operator=(const Matrix<T>& copy)
        {
            resize(copy.rows(), copy.stride());
            auto b = copy.begin();
            auto e = copy.end();

            std::copy(b, e, MatrixView<T>::mView.begin());
            return copy;
        }

        void resize(u64 rows, u64 columns, AllocType type = AllocType::Zeroed)
        {
            if (rows * columns > mCapacity)
            {
                auto old = MatrixView<T>::mView;
                mCapacity = rows * columns;

                if (std::is_trivially_constructible<T>::value)
                {
                    if (type == AllocType::Zeroed)
                        MatrixView<T>::mView = span<T>((T*)new Storage[mCapacity](), mCapacity);
                    else
                        MatrixView<T>::mView = span<T>((T*)new Storage[mCapacity], mCapacity);

                    std::copy(old.begin(), old.end(), MatrixView<T>::mView.begin());
                }
                else
                {
                    MatrixView<T>::mView = span<T>((T*)new Storage[mCapacity], mCapacity);

                    auto iter = MatrixView<T>::mView.data();
                    for (u64 i = 0; i < old.size(); ++i)
                    {
                        new (iter++) T(std::move(old[i]));
                    }

                    if (type == AllocType::Zeroed)
                    {
                        for (u64 i = old.size(); i < mCapacity; ++i)
                        {
                            new (iter++) T;
                        }
                    }
                }


                if (std::is_trivially_destructible<T>::value == false)
                {
                    for (u64 i = 0; i < old.size(); ++i)
                        (old.data() + i)->~T();
                }

                delete[](Storage*)old.data();
            }
            else
            {
                auto newSize = rows * columns;
                if (MatrixView<T>::size() &&
                    newSize > MatrixView<T>::size() &&
                    type == AllocType::Zeroed)
                {
                    auto b = MatrixView<T>::data() + MatrixView<T>::size();
                    auto e = b + newSize - MatrixView<T>::size();

                    if (std::is_trivially_constructible<T>::value)
                    {
                        std::memset(b, 0, (e - b) * sizeof(T));
                    }
                    else
                    {
                        for (auto i = b; i < e; ++i)
                            new(i)T;
                    }
                }

                MatrixView<T>::mView = span<T>(MatrixView<T>::data(), newSize);
            }

            MatrixView<T>::mStride = columns;
        }


        // return the internal memory, stop managing its lifetime, and set the current container to null.
        T* release()
        {
            auto ret = MatrixView<T>::mView.data();
            MatrixView<T>::mView = span<T>(nullptr, 0);
            mCapacity = 0;
            return ret;
        }


        bool operator==(const Matrix<T>& m) const
        {
            if (m.rows() != MatrixView<T>::rows() || m.cols() != MatrixView<T>::cols())
                return false;
            auto b0 = m.begin();
            auto e0 = m.end();
            auto b1 = MatrixView<T>::begin();
            return std::equal(b0, e0, b1);
        }
    };


}
