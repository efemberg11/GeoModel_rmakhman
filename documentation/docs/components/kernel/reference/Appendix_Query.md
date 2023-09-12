

## Appendix: The Query<T> template class.  


The template class `Query<T>` is designed as the return type of a query that can fail. One place we use it within this library is to return the index of a daughter volume, in other words, its position within a child list.  If the volume is not found within the daughter list, the query fails.  The failure could be handled in several ways, one of which would be to return a value of -999.  If the user were to blithely use this value without checking it first, the program could likely crash immediately or misbehave later.

The class `Query<unsigned int>` fixes ths problem. Because it has a constructor taking unsigned int as a single argument and a cast operator, it can freely convert itself to and from an unsigned int. So you can write:

```cpp
GeoPhysVol *parent, *child;
unsigned int c = parent->getIndex(child);
```

If the function succeeds, it returns `Query<unsigned int>` and a conversion takes place. But, if it fails, the conversion itself throws an exception, in this case `std::range_error`.  

One does not need to handle the exception in order to proceed past the failed query, a better way is to check the return value, and this can be done by recoding the example as follows:

```cpp
GeoPhysVol *parent, *child;
Query<unsigned int> c = parent->getIndex(child);
if (c.isValid()) {
    // Now use c like an ordinary unsigned integer:
    unsigned int d = c+1;
}
```

Now, this kind of checking is safer, but can always be skipped if the operation is guaranteed to succeed, for example, if the programmer knows the child is in the daughter list (because she put it there).  The class `Query<T>` is based on the class `Fallible<T>` from reference[^BN1].



[^BN1]: JJ.Barton and  LR.Nackmann, Scientific and Engineering C++, 1994, Addison Wesley




