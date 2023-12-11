/*
  Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
*/
#ifndef GEOMODELKERNEL_GeoIntrusivePtr_H
#define GEOMODELKERNEL_GeoIntrusivePtr_H

#include <GeoModelKernel/RCBase.h>

#include <utility>


template<typename GeoType> 
class GeoIntrusivePtr{
    public:
        template <typename GeoTypeGrp> friend class GeoIntrusivePtr;

        GeoIntrusivePtr() = default;
        // Standard constructor taking a bare pointer
        GeoIntrusivePtr(GeoType* obj):
            m_ptr{obj} {
             if (m_ptr) obj->ref();
        }
        /// Copy constructor 
        template <class GeoTypeGrp>
        GeoIntrusivePtr(const GeoIntrusivePtr<GeoTypeGrp>& other):
            GeoIntrusivePtr{other.get()} {}
        /// Assignment operator
        template <class GeoTypeGrp>
        GeoIntrusivePtr& operator=(const GeoIntrusivePtr<GeoTypeGrp>& other) {
            if (m_ptr != other.get()) {
                if(m_ptr) m_ptr->unref();
                m_ptr = other.get();
                if(m_ptr) m_ptr->ref();
            }
            return *this;
        }
        /// Move constructor
        template <class GeoTypeGrp>
        GeoIntrusivePtr(GeoIntrusivePtr<GeoTypeGrp>&& other):
            m_ptr{std::move(other.m_ptr)} { other.m_ptr = nullptr;}
        /// Move assignment operator
        template <class GeoTypeGrp>
        GeoIntrusivePtr& operator=(GeoIntrusivePtr<GeoTypeGrp>&& other) {
            if (m_ptr != other.get()) {
                if(m_ptr) m_ptr->unref();
                m_ptr = std::move(other.m_ptr);   
            } else if (m_ptr) {
                m_ptr->unref();
            }
            other.m_ptr = nullptr;
            return *this;
        }
        /// Destructor
        ~GeoIntrusivePtr() {
            if (m_ptr) m_ptr->unref();
        }
        /// Implicit conversion
        operator GeoType*() const { return m_ptr; }
        operator GeoType* const*() const { return &m_ptr; }

        /// Pointer access
        GeoType* operator->() const { return m_ptr; }
        /// Dereference the pointer
        GeoType& operator*() const { return *m_ptr; }
        /// Getter methods
        GeoType* get() const { return m_ptr; }
        /// Validity operator
        operator bool () const { return m_ptr != nullptr; }
        /// Invalidity operator
        bool operator!() const { return !m_ptr; }
        /// Comparison operator
        template <class GeoTypeGrp>
        bool operator==(const GeoIntrusivePtr<GeoTypeGrp>& other) const {
            return m_ptr == other.m_ptr;
        }
        bool operator==(GeoType* other) const {
            return m_ptr == other;
        }
        /// Odering operator
        bool operator<(const GeoIntrusivePtr& other) const {
            return m_ptr < other.m_ptr;
        }
    private:
        GeoType* m_ptr{nullptr};
};

#endif
