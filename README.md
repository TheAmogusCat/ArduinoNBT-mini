# ArduinoNBT Library
### A library for arduino to work with NBT data

### Installing
1) Download the project
2) Put **nbt.h** and **nbt.cpp** into your project folder

### Usage:

Include the library (the path may be different if you place the library outside the root of your project):
```cpp
#include "nbt.h"
```

**Using Compound Tags:**
```cpp
// Create Compound Tag
// Note: The second parameter is the size of the buffer in the class, so it must be specified in bytes.
NBTCompoundTag compound("My Compound Tag", 100);

// Adding int (all methods can be seen below)
// Note: If the serialized tag size is larger than the buffer size in the class (which you specified earlier), the value will not be added, without any warning
compound.pushInt("My Int", 777);

// Serializing Compound Tag
byte buffer[100];
size_t size = compound.serialize(buffer);
```
Available methods in Compound Tag:
- pushByte
- pushShort
- pushInt
- pushLong
- pushFloat
- pushDouble

And if you've already serialized the nbt tag, you can use `pushBytes(bytesArray)` to add it to the compound tag.

**Available types:**
- NBTCompoundTag
