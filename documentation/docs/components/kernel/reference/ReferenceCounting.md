


## Reference counting


!!! warning
    The custom reference counting used in GeoModel was developed when no smart pointers were available for C++. That part will be the object of a major refactoring in the near future.


Many objects need to be allocated in the description of a complicated geometry. For efficient use of memory, these should be shared as often as possible. The extensive sharing of objects in a geometry system calls for a way of destroying the objects as soon as they are not used—by any other object that references them. The scheme for doing this is called reference counting. In the GeoModelKernel classes, it is achieved by mixing in a abstract base class, RCBase:

```cpp
//RCBase
  Constructor:
    RCBase()
  Public Methods:
    void ref ()
    void unref ()
    unsigned int refCount ()
```

`RCBase` is inherited by many of the classes in the geometry system. Reference-counted objects can only be created using operator new, and cannot be created on the stack. The methods `ref()` and `unref()` can be called to increase or decrease the reference count of an object. When the reference count decreases to zero, the object deletes itself. The accessor `refCount()` returns the current reference count.



