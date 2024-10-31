## Reference counting

Many objects need to be allocated in the description of a complicated geometry. For efficient use of memory, these should be shared as often as possible. The extensive sharing of objects in a geometry system calls for a way of destroying the objects as soon as they are not used by any other object that references them. The scheme for doing this is called reference counting. In the GeoModelKernel classes, it is achieved by mixing in a abstract base class, `RCBase`.

### RCBase

```cpp
//RCBase
  Constructor:
    RCBase()
  Public Methods:
    void ref ()
    void unref ()
    unsigned int refCount ()
```

`RCBase` is inherited by many of the classes in the geometry system. Reference-counted objects can only be created using operator `new` (i.e. they cannot be created on stack). The methods `ref()` and `unref()` can be called to increase or decrease the reference count of an object. When the reference count decreases to zero, the object deletes itself. The accessor `refCount()` returns the current reference count.

While it is possible for the client code to operate with `RCBase` API directly, doing so is **not recommended**. Instead it is advisable to wrap the pointers to reference-counted objects into special smart pointers: `GeoIntrusivePtr`.

### GeoIntrusivePtr

`GeoIntrusivePtr` implements smart pointer semantics for objects with embedded reference counts similarly to how it is implemented by Boost [`intrusive_ptr`](https://www.boost.org/doc/libs/1_86_0/libs/smart_ptr/doc/html/smart_ptr.html#intrusive_ptr).

```cpp
template<typename GeoType> class GeoIntrusivePtr{
    public:
	// Standard constructor taking a bare pointer
	GeoIntrusivePtr(GeoType* obj) noexcept;
	// Copy constructor
	GeoIntrusivePtr(const GeoIntrusivePtr& other) noexcept;
	// Move constructor
	GeoIntrusivePtr(GeoIntrusivePtr&& other) noexcept;
	// Assignment operator
	GeoIntrusivePtr& operator=(const GeoIntrusivePtr& other) noexcept;
	GeoIntrusivePtr& operator=(GeoType* other) noexcept;
	// Move assignment operator
	GeoIntrusivePtr& operator=(GeoIntrusivePtr&& other) noexcept;
	// Reset the pointer
	void reset(GeoType* ptr = nullptr);
	// Destructor
	~GeoIntrusivePtr();
	// Implicit conversion
	operator GeoType*() const;
	operator GeoType* const*() const;
	// Pointer access
	GeoType* operator->() const;
	// Dereference the pointer
	GeoType& operator*() const;
	// Getter methods
	GeoType* get() const;
	// Validity operator
	operator bool() const;
	// Comparison operator
	template <class GeoTypeGrp>
	bool operator==(const GeoIntrusivePtr<GeoTypeGrp>& other) const;
	bool operator==(GeoType* other) const;
	// Odering operator
	bool operator<(const GeoIntrusivePtr& other) const;

...
};
```

The objects of the class `GeoIntrusivePtr` increase the reference count of the managed `RCBase` object at the construction and decrease it at the destruction. Hence, intrusive pointers provide a mechanism for automatic memory management for reference-counted GeoModel objects, and eliminate the need for the client code to call the `ref()` and `unref()` methods explicitly.